# Week 13：高性能算子核函数 — Java vs C++ 差异对照

## 一、向量化与数值计算

```java
// Java — Project Panama / Vector API (Incubator, JDK 16+)
// 向量化是最近才加入的，API 不稳定
// import jdk.incubator.vector.*;
//
// FloatVector va = FloatVector.fromArray(SPECIES_256, a, i);
// FloatVector vb = FloatVector.fromArray(SPECIES_256, b, i);
// FloatVector vc = va.add(vb);
// vc.intoArray(c, i);
//
// 限制：需 JDK 16+，API 在 incubator 模块，需 --add-modules jdk.incubator.vector
//       实际是否生成 SIMD 指令取决于 JIT（C2 编译器）判断
//       无法显式对齐数据、无法控制向量宽度
```

```cpp
// C++ — 编译期自动向量化 + 手写 intrinsics 完全控制
// 1. 编译器自动向量化（-O3 自动）
template<typename T>
void elementWiseAdd(const T* a, const T* b, T* c, size_t n) {
    for (size_t i = 0; i < n; ++i)
        c[i] = a[i] + b[i];        // -O3 自动生成 SIMD 指令
}

// 2. 手写 intrinsics（精确控制）
#include <immintrin.h>
void addAVX(const float* a, const float* b, float* c, size_t n) {
    for (size_t i = 0; i < n; i += 8) {   // AVX: 一次 8 个 float
        __m256 va = _mm256_loadu_ps(&a[i]);
        __m256 vb = _mm256_loadu_ps(&b[i]);
        __m256 vc = _mm256_add_ps(va, vb);
        _mm256_storeu_ps(&c[i], vc);
    }
}

// 3. 对齐要求：alignas(32) 确保 AVX 对齐加载
alignas(32) float a[1024];  // _mm256_load_ps 要求 32 字节对齐
```

| 特性 | Java (Vector API) | C++ |
|------|-------------------|-----|
| SIMD 可用性 | JDK 16+ incubator | 任何编译器（x86/ARM） |
| 控制粒度 | 高级抽象，JIT 决策 | 自动向量化或手写 intrinsics |
| 数据对齐 | 不可控（JVM 管理内存） | `alignas` + `posix_memalign` |
| 寄存器宽度 | 自动适配（运行时探测） | 编译期指定，可写多版本 |
| 稳定性 | Incubator → 可能变更 | 稳定（intrinsics 兼容性高） |
| 性能确定性 | JIT 预热后稳定 | 编译后即确定 |

## 二、内存布局与缓存控制

```java
// Java — 矩阵通常是数组的数组（jagged array）
// 无法保证行连续存储
double[][] matrix = new double[rows][cols];
// 每一行是独立的堆对象 → 各行可能在内存中相距很远
// matrix[i][j] 需要两次解引用：找到行对象 → 找到元素

// 二维数组访问：cache 不友好
for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
        matrix[i][j] *= 2;       // 每行内连续，但行间跳跃
```

```cpp
// C++ — 平坦化（flatten）一维数组管理二维矩阵
// 所有元素连续存储
std::vector<double> matrix(rows * cols);

// 索引：matrix[i * cols + j]
for (size_t i = 0; i < rows; ++i)
    for (size_t j = 0; j < cols; ++j)
        matrix[i * cols + j] *= 2;  // 完全连续访问 → 预取友好

// 分块优化：子块可精确放入 L1 cache
constexpr size_t BLOCK = 64;
for (size_t i0 = 0; i0 < rows; i0 += BLOCK)
    for (size_t j0 = 0; j0 < cols; j0 += BLOCK)
        for (size_t i = i0; i < std::min(i0 + BLOCK, rows); ++i)
            for (size_t j = j0; j < std::min(j0 + BLOCK, cols); ++j)
                C[i * cols + j] += A[i * cols + k] * B[k * cols + j];
```

| 方面 | Java | C++ |
|------|------|-----|
| 矩阵表示 | `double[][]` — 行独立对象 | `vector<double>` — 平坦化连续 |
| 缓存利用 | 行内连续，行间随机 | 完全连续 → 预取器友好 |
| 分块优化 | 困难（行间不连续，无法精确定位） | 直接（block size 精确匹配 cache 大小） |
| 零拷贝视图 | 困难（需 ByteBuffer 等） | 指针/span/view 直接零开销 |
| 跨步访问 | 通过索引计算，但解引用开销大 | `std::mdspan` (C++23) 或手动 stride |

## 三、编译期优化 vs 运行时 JIT

```java
// Java — JIT 运行时优化
// - 热点循环才会被编译（需要预热）
// - C2 编译器进行循环展开、向量化
// - 逃逸分析消除堆分配
// - 但：优化发生在运行时，有预热开销
// - 且 JIT 有时间预算，不会无限优化

public void elementWiseAdd(float[] a, float[] b, float[] c) {
    for (int i = 0; i < a.length; i++) {
        c[i] = a[i] + b[i];         // 前几千次：解释执行
    }                                // 热点后：JIT 编译 → 可能向量化
}
```

```cpp
// C++ — 编译期全部优化完成
// - -O3 自动向量化、循环展开、内联
// - -march=native 使用当前 CPU 全部指令集
// - 零预热：第一次运行就是最优代码
// - constexpr 在编译期求值，完全消除运行时开销

template<typename T>
void elementWiseAdd(const T* a, const T* b, T* c, size_t n) {
    // 编译后直接是 SIMD 循环（如果编译器判断可行）
    for (size_t i = 0; i < n; ++i)
        c[i] = a[i] + b[i];
}

// 编译期维度已知 → 完全展开甚至编译期求值
template<size_t N>
constexpr auto constAdd(const std::array<float, N>& a,
                        const std::array<float, N>& b) {
    std::array<float, N> c{};
    for (size_t i = 0; i < N; ++i) c[i] = a[i] + b[i];
    return c;  // N 小时，编译器可能完全展开或编译期求值
}
```

## 四、数值稳定性控制

```java
// Java — 浮点语义由 JLS 严格定义
// - strictfp 关键字：保证跨平台完全一致的浮点结果
// - 默认（非 strictfp）：允许使用扩展精度（x87 80-bit）
// - 无法轻易禁用非规格化数支持
// - 无法控制 FTZ/DAZ 标志

strictfp double compute(double x) {
    return Math.exp(x);  // 保证在所有平台结果完全一致
}
// 代价：可能无法使用某些硬件加速
```

```cpp
// C++ — 完全控制浮点行为
// - -ffast-math: 允许不精确但更快的数学
//     - 假设 NaN/Inf 不出现
//     - 允许结合律重排 (a+b)+c → a+(b+c)
//     - 刷新非规格化数为零 (FTZ/DAZ)
// - 精确控制：-fno-fast-math 使用 IEEE 754 严格合规
// - #pragma STDC FENV_ACCESS: 控制浮点异常和舍入

// 数值稳定 softmax（手动控制）
template<typename T>
void softmaxStable(const T* x, T* y, size_t n) {
    T maxVal = *std::max_element(x, x + n);
    T sum = 0;
    for (size_t i = 0; i < n; ++i) {
        y[i] = std::exp(x[i] - maxVal);
        sum += y[i];
    }
    for (size_t i = 0; i < n; ++i) y[i] /= sum;
}
```

| 特性 | Java | C++ |
|------|------|-----|
| 浮点语义 | JLS 严格定义 / strictfp | 编译器标志控制（-ffast-math 等） |
| 非规格化数 | 自动处理（慢） | 可禁用（FTZ/DAZ，-ffast-math） |
| 跨平台一致性 | strictfp 保证 | `-fno-fast-math` 保证 |
| FMA 指令 | `Math.fma()` (JDK 9+) | `std::fma()` (C++17) |
| 舍入模式 | 无法运行时切换 | `fesetround()` |
| Softmax 稳定性 | 手动实现（与 C++ 相同逻辑） | 手动实现 + 可选 SIMD intrinsics |

## 五、快速参考卡

| 场景 | Java 做法 | C++ 做法 |
|------|----------|---------|
| 逐元素操作 | 循环 + JIT 自动向量化 | 模板 + `-O3` 自动向量化或 intrinsics |
| 矩阵乘法 | `double[][]` + 循环（不佳） | 平坦化 + 分块 + ikj 循环顺序 |
| 归约 | `Arrays.stream().sum()` / 手动循环 | 模板 reduce 函数 + SIMD 归约 |
| 广播 | 手动复写或数组复制 | 虚广播（索引映射，零拷贝） |
| Softmax 稳定 | `exp(x - max)` 手动 | `exp(x - max)` + `-ffast-math` |
| 缓存控制 | 无直接控制 | `alignas` + 平坦化 + 分块大小选择 |
| 性能测量 | `System.nanoTime()` / JMH | `<chrono>` + `benchmark` + warmup |
| 编译标志 | 无（JIT 标志通过 -XX:） | `-O3 -march=native -ffast-math` |
