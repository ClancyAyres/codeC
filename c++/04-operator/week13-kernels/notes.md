# Week 13 知识点笔记：高性能算子核函数

## 一、逐元素操作模板化

### 1.1 概念

逐元素（element-wise）操作对张量的每个元素独立施加同一个函数，是最基本的算子模式。

```cpp
// 非模板：每个操作都要写一个新函数
void add(const float* a, const float* b, float* c, size_t n) {
    for (size_t i = 0; i < n; ++i) c[i] = a[i] + b[i];
}
void mul(const float* a, const float* b, float* c, size_t n) {
    for (size_t i = 0; i < n; ++i) c[i] = a[i] * b[i];
}
```

### 1.2 模板化方案

```cpp
// 二元操作模板
template<typename T, typename BinaryOp>
void elementWiseBinary(const T* a, const T* b, T* c, size_t n, BinaryOp op) {
    for (size_t i = 0; i < n; ++i)
        c[i] = op(a[i], b[i]);
}

// 一元操作模板
template<typename T, typename UnaryOp>
void elementWiseUnary(const T* a, T* b, size_t n, UnaryOp op) {
    for (size_t i = 0; i < n; ++i)
        b[i] = op(a[i]);
}

// 使用
auto c = elementWiseBinary(a, b, n, std::plus<float>{});
auto d = elementWiseBinary(a, b, n, [](float x, float y) { return x * y + 1; });
```

### 1.3 编译期优化

```cpp
// 使用 std::enable_if 或 constexpr if 在编译期选择分支
template<typename T, typename BinaryOp>
void elementWiseBinary(T* a, const T* b, const T* c, size_t n, BinaryOp op) {
    if constexpr (std::is_same_v<BinaryOp, std::plus<T>>) {
        // 加法特化：编译器更容易向量化
        #pragma GCC ivdep
        for (size_t i = 0; i < n; ++i) a[i] = b[i] + c[i];
    } else {
        for (size_t i = 0; i < n; ++i) a[i] = op(b[i], c[i]);
    }
}
```

### 1.4 SIMD 向量化要点

| 优化点 | 说明 |
|--------|------|
| 内存对齐 | `alignas(32)` 确保数据对齐到 AVX 边界 |
| `__restrict` | 告知编译器指针不重叠，允许向量化 |
| `#pragma GCC ivdep` | 忽略假定的向量依赖 |
| 编译标志 | `-O3 -march=native -ffast-math` |

## 二、归约模式

### 2.1 概念

归约（reduce）将张量沿某个维度折叠，产生更小的张量。典型操作：sum、max、min、product。

```cpp
// 简单一维归约
template<typename T>
T reduceSum(const T* data, size_t n) {
    T result = 0;
    for (size_t i = 0; i < n; ++i)
        result += data[i];
    return result;
}
```

### 2.2 多维归约

```cpp
// shape = {rows, cols}，沿 axis=1 归约：每行求和 → shape = {rows}
template<typename T>
void reduceSumAxis1(const T* input, T* output, size_t rows, size_t cols) {
    for (size_t r = 0; r < rows; ++r) {
        T sum = 0;
        for (size_t c = 0; c < cols; ++c) {
            sum += input[r * cols + c];
        }
        output[r] = sum;
    }
}

// 沿 axis=0 归约：每列求和 → shape = {cols}
template<typename T>
void reduceSumAxis0(const T* input, T* output, size_t rows, size_t cols) {
    for (size_t c = 0; c < cols; ++c) output[c] = 0;
    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            output[c] += input[r * cols + c];
        }
    }
}
```

### 2.3 常见归约操作

| 操作 | 初始值 | 结合操作 |
|------|--------|----------|
| sum | 0 | `+` |
| product | 1 | `*` |
| max | `-inf` | `std::max` |
| min | `+inf` | `std::min` |
| logical_and | `true` | `&&` |
| logical_or | `false` | `\|\|` |

### 2.4 数值稳定性

浮点累加可能丢失精度。使用成对求和（pairwise summation）减轻误差：

```cpp
template<typename T>
T reduceSumPairwise(const T* data, size_t n) {
    if (n == 0) return T(0);
    if (n == 1) return data[0];
    size_t mid = n / 2;
    return reduceSumPairwise(data, mid) + reduceSumPairwise(data + mid, n - mid);
}
```

## 三、矩阵乘法优化阶梯

### 3.1 朴素实现（ijk）

```cpp
// 最直观的三重循环，但缓存极不友好
// A: M×K, B: K×N, C: M×N
for (size_t i = 0; i < M; ++i)
    for (size_t j = 0; j < N; ++j)
        for (size_t k = 0; k < K; ++k)
            C[i * N + j] += A[i * K + k] * B[k * N + j];
// 问题：内层访问 B[k*N + j] — k 变化时跨 N 个元素 → 大量 cache miss
```

### 3.2 循环交换优化（ikj）

```cpp
// 交换 j 和 k 循环顺序，让内层连续访问
for (size_t i = 0; i < M; ++i)
    for (size_t k = 0; k < K; ++k)
        for (size_t j = 0; j < N; ++j)
            C[i * N + j] += A[i * K + k] * B[k * N + j];
// A[i*K + k]: k递增 → 连续访问 ✓
// B[k*N + j]: j递增 → 连续访问 ✓
// C[i*N + j]: j递增 → 连续访问 ✓
// 所有三个矩阵内层连续访问，缓存友好
```

### 3.3 分块优化（blocking / tiling）

```cpp
constexpr size_t BLOCK = 64;  // 典型值，使子块能装入 L1 cache

for (size_t i0 = 0; i0 < M; i0 += BLOCK)
    for (size_t k0 = 0; k0 < K; k0 += BLOCK)
        for (size_t j0 = 0; j0 < N; j0 += BLOCK)
            // 内层在 BLOCK×BLOCK 子块上运行 ikj 顺序
            for (size_t i = i0; i < std::min(i0 + BLOCK, M); ++i)
                for (size_t k = k0; k < std::min(k0 + BLOCK, K); ++k)
                    for (size_t j = j0; j < std::min(j0 + BLOCK, N); ++j)
                        C[i * N + j] += A[i * K + k] * B[k * N + j];
```

### 3.4 优化阶梯总结

| 级别 | 方法 | 相对加速 | 关键原理 |
|------|------|---------|----------|
| 0 | 朴素 ijk | 1× | 基准线，大量 cache miss |
| 1 | 循环交换 ikj | 3-5× | 内层全部连续访问 |
| 2 | 分块 blocking | 10-30× | 子块常驻 L1/L2 cache |
| 3 | 循环展开 + SIMD | 50-100× | 利用向量寄存器 |
| 4 | 多线程 + 多级分块 | 200-1000× | 多核并行 + 各级缓存分块 |

## 四、广播语义

### 4.1 概念

广播（broadcasting）允许形状不同的张量进行逐元素操作：较小的张量沿缺失维度"扩展"以匹配较大张量的形状。

```cpp
// NumPy 示例：shape (3,1) + (4,) → (3,4)
// 规则：从右对齐维度，维度为1或缺失时可广播

// C++ 实现：标量广播到向量
template<typename T>
void broadcastScalar(const T* vec, size_t n, T scalar, T* out, BinaryOp op) {
    for (size_t i = 0; i < n; ++i)
        out[i] = op(vec[i], scalar);    // scalar 广播到每个元素
}

// 向量广播到矩阵（行广播）
template<typename T>
void broadcastRow(const T* mat, size_t rows, size_t cols,
                  const T* vec, T* out) {
    for (size_t r = 0; r < rows; ++r)
        for (size_t c = 0; c < cols; ++c)
            out[r * cols + c] = mat[r * cols + c] + vec[c];
            // vec[c] 对每一行重复使用
}
```

### 4.2 广播规则

```
规则1：如果两个数组维度不同，较小数组的形状左侧补1
规则2：如果两个维度相等，或其中一个为1，则兼容
规则3：结果形状 = max(shape_a, shape_b) 逐维度

示例：
  (3, 4) + (4,)     → 右边对齐 → (3,4) + (1,4) → (3,4)  ✓
  (3, 4) + (3, 1)   → (3,4) + (3,1) → (3,4)  ✓
  (3, 4) + (2, 4)   → 不兼容 ✗（3 ≠ 2 且都不是1）
```

### 4.3 内存视角

广播不实际复制数据 — 它改变索引计算方式：

```cpp
// 广播索引映射：逻辑索引 (r, c) → 物理索引的计算
// 输入 shape (3, 4)，广播 shape (1, 4) — 行维度广播
size_t src_c = c;        // 列索引直接使用
size_t src_r = 0;        // 行索引始终为0（广播）
T value = vec[src_r * 4 + src_c];  // 虚复制：不分配额外内存
```

## 五、Softmax 数值稳定性

### 5.1 朴素实现的问题

```cpp
// 朴素 softmax: softmax(x_i) = exp(x_i) / sum(exp(x_j))
// 问题：exp 增长极快 — exp(89) ≈ 4.9×10^38 已溢出 float 范围
//        exp(90) = +inf → 整个计算崩坏

template<typename T>
void softmaxNaive(const T* x, T* y, size_t n) {
    T sum = 0;
    for (size_t i = 0; i < n; ++i) {
        y[i] = std::exp(x[i]);       // 可能溢出！
        sum += y[i];
    }
    for (size_t i = 0; i < n; ++i)
        y[i] /= sum;
}

// 对于输入 [1000, 1000, 1000]:
//   exp(1000) = +inf → softmax = [NaN, NaN, NaN]
```

### 5.2 稳定实现：减最大值

```cpp
// 数学恒等式：softmax(x)_i = exp(x_i - max(x)) / sum(exp(x_j - max(x)))
// 减去 max 后，最大指数为 exp(0) = 1，所有值在 (0, 1] 区间

template<typename T>
void softmaxStable(const T* x, T* y, size_t n) {
    // Step 1: 找最大值
    T maxVal = x[0];
    for (size_t i = 1; i < n; ++i)
        if (x[i] > maxVal) maxVal = x[i];

    // Step 2: exp(x_i - max) + 求和
    T sum = 0;
    for (size_t i = 0; i < n; ++i) {
        y[i] = std::exp(x[i] - maxVal);  // 现在安全了
        sum += y[i];
    }

    // Step 3: 归一化
    for (size_t i = 0; i < n; ++i)
        y[i] /= sum;
}
```

### 5.3 为什么减去 max 就安全了

```
原始输入:         [89, 90, 91]
朴素 exp:          [exp(89), exp(90)=+inf, exp(91)=+inf] → 溢出

减去 max(91):     [89-91=-2, 90-91=-1, 91-91=0]
稳定 exp:          [exp(-2)≈0.135, exp(-1)≈0.368, exp(0)=1.0]
求和:              0.135 + 0.368 + 1.0 = 1.503
归一化:            [0.090, 0.245, 0.665] ← 完全相同的结果

关键：x_i - max ≤ 0，所以 exp(x_i - max) ≤ 1 → 永不溢出
```

### 5.4 对数空间技巧

```cpp
// log-softmax：直接输出对数概率，避免指数的精度损失
// log_softmax(x)_i = x_i - log(sum(exp(x_j)))
//                  = (x_i - max) - log(sum(exp(x_j - max)))
// 常用于 NLL Loss 计算

template<typename T>
void logSoftmaxStable(const T* x, T* y, size_t n) {
    T maxVal = x[0];
    for (size_t i = 1; i < n; ++i)
        if (x[i] > maxVal) maxVal = x[i];

    T sumExp = 0;
    for (size_t i = 0; i < n; ++i)
        sumExp += std::exp(x[i] - maxVal);

    T logSum = std::log(sumExp) + maxVal;
    for (size_t i = 0; i < n; ++i)
        y[i] = x[i] - logSum;
}
```

## 六、性能分析与验证

### 6.1 GFLOPS 计算

```cpp
// 矩阵乘法 C = A(M×K) * B(K×N) → 计算量 = 2*M*N*K FLOPs
// GFLOPS = (2.0 * M * N * K) / (elapsed_seconds * 1e9)

double computeGFLOPS(size_t M, size_t N, size_t K, double elapsedSeconds) {
    double flops = 2.0 * M * N * K;   // 乘法 + 加法各算一次
    return flops / (elapsedSeconds * 1e9);
}
```

### 6.2 验证正确性

```cpp
// 对比朴素实现和优化实现是否产生相同结果
template<typename T>
bool verify(const T* a, const T* b, size_t n, T epsilon = 1e-4) {
    for (size_t i = 0; i < n; ++i) {
        if (std::abs(a[i] - b[i]) > epsilon) return false;
    }
    return true;
}
```

### 6.3 常见陷阱

| 陷阱 | 说明 | 解决方案 |
|------|------|----------|
| 浮点溢出 | exp 快速溢出 float/double | 减 max、log-space |
| 数值下溢 | 极小概率变为 0 | double 精度或 log-space |
| 非规格化数 | 极小的 subnormal 数极大拖慢速度 | `-ffast-math` 或 `FTZ/DAZ` 标志 |
| 假共享 | 多线程写入同一 cache line | `alignas(64)` 填充 |
| TLB miss | 大矩阵跨页访问 | 分块 + 大页支持 |
