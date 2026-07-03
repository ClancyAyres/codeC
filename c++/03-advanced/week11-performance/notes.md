# Week 11 知识点笔记：性能优化基础

## 一、缓存层次与 Cache Line

### 1.1 为什么缓存重要

CPU 速度远快于内存。访问延迟大致为：

| 层级 | 大小 | 延迟 | 带宽 |
|------|------|------|------|
| L1 Cache | 32KB | ~1ns (4 cycles) | ~1TB/s |
| L2 Cache | 256KB | ~4ns (12 cycles) | ~500GB/s |
| L3 Cache | 8-32MB | ~12ns (40 cycles) | ~200GB/s |
| RAM | 16GB+ | ~100ns (300 cycles) | ~50GB/s |

**一条 cache line 是 64 字节（典型 x86-64）。每次从内存读取至少一整条 cache line。**

### 1.2 时间局部性与空间局部性

```cpp
// 时间局部性：刚访问过的数据很可能会再次访问
int hotVar = 0;           // 放在寄存器或 L1 — 再访问很快
for (int i = 0; i < 100000; ++i) {
    hotVar++;             // 每次都命中 L1
}

// 空间局部性：连续访问相邻内存地址
int arr[1000000];
for (int i = 0; i < 1000000; ++i) {
    arr[i] *= 2;          // 连续访问 → 预取器自动预取 → 高缓存命中率
}

// 破坏空间局部性的例子：
for (int i = 0; i < 1000000; i += 137) {  // 大跨步访问
    arr[i] *= 2;          // 每次可能 miss → 慢！
}
```

### 1.3 缓存行的工作原理

```
┌─────────────────────────────────────────────────────────┐
│  缓存行（64 字节）                                        │
│ ┌─────────────────────────────────────────────────────┐ │
│ │ 字节 0-63: 连续数据（或一个 struct 的多个字段）        │ │
│ └─────────────────────────────────────────────────────┘ │
│                                                         │
│ 地址映射：物理地址的低6位是行内偏移（2^6 = 64）          │
└─────────────────────────────────────────────────────────┘
```

## 二、AoS vs SoA

### 2.1 概念

```cpp
// AoS: Array of Structures — 结构体数组
struct Particle {
    float x, y, z;    // 位置
    float vx, vy, vz; // 速度
    float mass;
    int id;
};
std::vector<Particle> particles(N);  // 一个对象 → 一块连续内存

// 内存布局: [x0,y0,z0,vx0,vy0,vz0,m0,id0] [x1,y1,z1,...]
// 遍历 x 坐标: 每读取 64 字节，只用到 12 字节的 x,y,z（浪费 52 字节！）

// SoA: Structure of Arrays — 数组结构体
struct Particles {
    std::vector<float> x, y, z;
    std::vector<float> vx, vy, vz;
    std::vector<float> mass;
};
// 内存布局: [x0,x1,x2,...,xN-1] [y0,y1,...] [z0,...]
// 遍历 x 坐标: 连续读取 float → 64 字节装满 16 个 x → 缓存利用率 100%
```

### 2.2 性能对比

```cpp
// 场景：只更新所有粒子的位置（仅用到 x, y, z, vx, vy, vz）
void updatePositionAoS(Particle* p, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        p[i].x += p[i].vx;       // 读 x,vx → 带来整个 Particle
        p[i].y += p[i].vy;       // 这 64 字节的其余字段浪费了
        p[i].z += p[i].vz;
    }
}
// 缓存效率：每次 miss 加载 64 字节，用到 24 字节 = 37.5%

void updatePositionSoA(float* x, float* vx, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        x[i] += vx[i];           // 每次 miss 加载 64 字节 = 16 个 float
    }
}
// 缓存效率：每次 miss 加载 64 字节，全部用到 = 100%
// SoA 通常快 2-5 倍（取决于访问模式）
```

### 2.3 何时用 AoS，何时用 SoA

| 场景 | 选择 |
|------|------|
| 总是同时访问一个对象的多个字段 | AoS（如序列化/反序列化） |
| 遍历时只访问少数几个字段 | SoA |
| 需要随机访问单个对象 | AoS |
| SIMD 向量化操作 | SoA（连续数据可直接加载到向量寄存器） |
| 面向对象设计 | 可混合：Hot/Cold 分离 + 批量操作用 SoA |

## 三、`alignas` / `alignof`

### 3.1 基本用法

```cpp
#include <iostream>

// alignof: 查询类型的对齐要求（编译期常量）
std::cout << "int:      " << alignof(int) << '\n';       // 4
std::cout << "double:   " << alignof(double) << '\n';    // 8
std::cout << "int64_t:  " << alignof(int64_t) << '\n';   // 8

// alignas: 指定变量或类型的对齐方式
alignas(16) int x;                // x 对齐到 16 字节（通常 4 就够了）

struct alignas(64) CacheLine {    // 整个类型对齐到 64 字节
    int data;
};
// sizeof(CacheLine) >= 64（编译器填充到 64 的倍数）
```

### 3.2 为什么需要对齐

```cpp
// 1. CPU 要求：某些指令要求数据对齐
//    - x86: 非对齐访问可以，但慢
//    - ARM: 某些指令要求自然对齐，否则 fault

// 2. 避免 false sharing（伪共享）：
struct alignas(64) PaddedCounter {
    std::atomic<int> value;
    // 编译器自动填充剩余 60 字节到 64 字节边界
};
// 两个线程各操作一个 PaddedCounter → 在不同 cache line → 不互相 invalidate

// 3. SIMD 要求：SSE（16 字节）/ AVX（32 字节）/ AVX-512（64 字节）
alignas(32) float data[8];  // AVX: __m256 要求 32 字节对齐
```

### 3.3 false sharing 详解

```cpp
// 问题：两个共享变量在同一 cache line（64 字节）
struct Counters {
    int a;  // 4 字节
    int b;  // 4 字节（距 a 仅 4 字节，在同一条 cache line 里）
};
Counters c;
// 线程1 写 c.a → 使整条 cache line 在核心1 上置 dirty
// 线程2 写 c.b → 使整条 cache line 在核心1 上 invalidate
// 结果：两个线程互相 invalidate → 每次都 cache miss → 慢 100 倍！

// 解决方案：填充到不同 cache line
struct alignas(64) CounterA { int a; };
struct alignas(64) CounterB { int b; };
```

## 四、`inline` / `-O2` / `-O3`

### 4.1 `inline` 关键字

```cpp
// C++ inline: 对编译器的建议（不是命令），真正含义是"允许在多个编译单元定义"
// 与性能优化的关系：建议编译器内联展开函数体

// 头文件中定义的非模板免费函数必须 inline（否则 multiple definition）
inline int add(int a, int b) { return a + b; }

// 编译器自己会决定是否内联：
// - -O0: 几乎不内联（inline 关键字无效）
// - -O1: 内联标记为 inline 的函数 + 简单候选函数
// - -O2/O3: 启发式内联（代码量、调用频率、函数大小等）
// 强制内联：
[[gnu::always_inline]] inline int forceInline(int x) { return x * 2; }
// 禁止内联（用于调试、减少 icache 压力）：
[[gnu::noinline]] void noInline() { /* large function */ }
```

### 4.2 优化等级

| 等级 | 开启的优化 |
|------|-----------|
| `-O0` | 无优化，调试友好 |
| `-O1` | 基本优化：死代码删除、常量折叠、简单内联 |
| `-O2` | 几乎所有不涉及 space-speed tradeoff 的优化 |
| `-O3` | `-O2` + 更激进内联、循环展开、自动向量化、`-ftree-vectorize` |
| `-Os` | `-O2` + 减少代码体积（适合嵌入式/缓存敏感的代码） |
| `-Ofast` | `-O3` + 忽略标准合规、允许不精确浮点数学 |

### 4.3 `-O2` vs `-O3` 示例

```cpp
// 原始代码：
int sum(const int* arr, int n) {
    int s = 0;
    for (int i = 0; i < n; ++i)
        s += arr[i];
    return s;
}

// -O2 可能生成：标量循环（scalar loop）
// -O3 可能生成：自动向量化 → 使用 SSE/AVX 一次处理 4/8 个元素
// -O3 -march=native：使用当前 CPU 支持的所有指令集

// 经验法则：
// - 开发阶段: -O0 -g
// - 发布版本: -O2（稳定）+ -DNDEBUG
// - 数值密集型: -O3 -march=native -ffast-math（注意浮点精度）
```

## 五、`<chrono>` 基准测试

### 5.1 高精度计时

```cpp
#include <chrono>
#include <iostream>

// 推荐：steady_clock — 单调递增，不受系统时间调整影响
auto start = std::chrono::steady_clock::now();
// ... 待测试代码 ...
auto end = std::chrono::steady_clock::now();

auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
std::cout << "Elapsed: " << elapsed.count() << " us\n";

// 纳秒精度：
auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
std::cout << "Elapsed: " << ns.count() << " ns\n";
```

### 5.2 正确基准测试模式

```cpp
// 错误：单次运行（噪音太多）
auto t1 = std::chrono::steady_clock::now();
func();
auto t2 = std::chrono::steady_clock::now();
// 结果受 CPU 频率缩放、其他进程、缓存状态影响

// 正确：多次重复取最小值（或中位数）
template<typename F>
double benchmark(F&& f, int iterations = 100) {
    // 预热：让数据和指令进入缓存，CPU 达到最高频率
    f();

    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        f();
    }
    auto end = std::chrono::steady_clock::now();

    auto total = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    return static_cast<double>(total.count()) / iterations;
}

// 更严格：doNotOptimize 防止编译器消除"无意义"的代码
template<typename T>
void doNotOptimize(T const& value) {
    asm volatile("" : : "r,m"(value) : "memory");
}
// 或 C++23: std::do_not_optimize()
```

### 5.3 Google Benchmark（如果可用）

```cpp
// #include <benchmark/benchmark.h>
//
// static void BM_StringCreation(benchmark::State& state) {
//     for (auto _ : state) {
//         std::string empty_string;
//         benchmark::DoNotOptimize(empty_string);
//     }
// }
// BENCHMARK(BM_StringCreation);
//
// BENCHMARK_MAIN();
//
// 优点：自动迭代计数、统计（min/mean/median/stddev）、多线程支持
```

## 六、`perf` / `valgrind` 简介

### 6.1 `perf` — Linux 性能分析

```bash
# 统计性能计数器（cycles, instructions, cache-misses, branch-misses）
perf stat ./my_program

# 采样热点（哪些函数花最多时间）
perf record ./my_program
perf report

# 实时 top-down 分析
perf top

# 查看 cache miss 率
perf stat -e cache-references,cache-misses ./my_program

# 查看分支预测错误
perf stat -e branch-instructions,branch-misses ./my_program
```

### 6.2 `valgrind` — 内存检查 + Cache 分析

```bash
# Cachegrind: 模拟 L1/L2 缓存行为
valgrind --tool=cachegrind ./my_program
cg_annotate cachegrind.out.<pid>

# Callgrind: 调用图 + 指令计数
valgrind --tool=callgrind ./my_program
callgrind_annotate callgrind.out.<pid>

# 报告内容：
# - Ir: 指令读取次数
# - I1mr/I2mr: L1/L2 指令缓存未命中
# - D1mr/D2mr: L1/L2 数据缓存未命中
# - LLmr: 最后一级缓存（L3）未命中

# macOS 替代：
# - Instruments.app（Time Profiler, System Trace）
# - sample <pid>（命令行采样）
```

### 6.3 macOS 性能工具

```bash
# macOS 没有原生的 perf，但有：
# Instruments.app — GUI 性能分析器
#   Time Profiler       → 类似 perf record + report
#   System Trace        → 系统级跟踪
#   Counters            → PMC 性能计数器

# 命令行替代：
sample <pid> 10          # 采样指定进程 10 秒
sudo dtruss -p <pid>     # 系统调用跟踪（需要 SIP 关闭）
```

## 七、编译器优化技巧总结

### 7.1 帮助编译器优化的写法

```cpp
// 1. 用 const/constexpr 让编译器知道值是编译期常量
constexpr int N = 1024;
int arr[N];               // 编译器可以完全展开或向量化

// 2. 用 restrict（C）/ __restrict（C++）告知指针不重叠
void add(float* __restrict a, const float* __restrict b, const float* __restrict c, int n) {
    for (int i = 0; i < n; ++i)
        a[i] = b[i] + c[i];  // 编译器知道 a, b, c 无重叠 → 向量化
}

// 3. 遍历顺序：i 行 → j 列（连续访问），不要 j → i（跨行访问）
for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
        matrix[i][j] *= 2;           // 好：i 内层连续

// 4. 局部变量 hint：
if (__builtin_expect(x > 0, 1)) {   // likely
    // 快速路径
}

// 5. 预取（prefetch）—— 仅在确定性顺序访问时有用
__builtin_prefetch(&arr[i + 16]);   // 提前 16 个元素加载到缓存
```

### 7.2 常见误区

| 误区 | 真相 |
|------|------|
| `inline` 关键字保证内联 | 只是建议；`-O2` 以上编译器自主决定 |
| `register` 关键字有用 | C++17 已移除，编译器忽略 |
| 手写循环展开一定更快 | `-O3` 自动展开通常更好，手写反而阻碍其他优化 |
| `volatile` 阻止优化 | 阻止所有优化，包括合法优化。用于硬件寄存器，非性能 |
| 越多 `#pragma` 越好 | 过多 pragma 可能冲突，编译器自动选择通常合理 |
