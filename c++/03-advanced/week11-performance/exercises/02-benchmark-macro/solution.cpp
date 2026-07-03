// Exercise 02: Benchmark Macro — 完整解法

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cmath>
#include <numeric>

template<typename T>
void doNotOptimize(T const& value) {
    asm volatile("" : : "r,m"(value) : "memory");
}

// ── BENCHMARK 宏 ────────────────────────────────────────

#define BENCHMARK(block, iterations, description)                        \
    do {                                                                 \
        /* 预热 */                                                       \
        { block; }                                                       \
        auto __start = std::chrono::steady_clock::now();                 \
        for (int __i = 0; __i < (iterations); ++__i) {                   \
            { block; }                                                   \
        }                                                                \
        auto __end = std::chrono::steady_clock::now();                   \
        auto __total = std::chrono::duration_cast<                      \
            std::chrono::nanoseconds>(__end - __start);                  \
        double __avg = static_cast<double>(__total.count()) / (iterations); \
        std::cout << "[" << (description) << "]  "                       \
                  << "平均: " << std::fixed << std::setprecision(2)      \
                  << __avg << " ns  (" << (iterations) << " iters)\n";   \
    } while(0)

// ── BENCHMARK_MULTI 宏 ───────────────────────────────────

#define BENCHMARK_MULTI(block, warmup, measure, description)             \
    do {                                                                 \
        /* 预热 */                                                       \
        for (int __w = 0; __w < (warmup); ++__w) { { block; } }         \
        std::vector<double> __times;                                     \
        __times.reserve(measure);                                        \
        for (int __m = 0; __m < (measure); ++__m) {                      \
            auto __start = std::chrono::steady_clock::now();             \
            { block; }                                                   \
            auto __end = std::chrono::steady_clock::now();               \
            __times.push_back(static_cast<double>(                       \
                std::chrono::duration_cast<std::chrono::nanoseconds>(   \
                    __end - __start).count()));                          \
        }                                                                \
        std::sort(__times.begin(), __times.end());                       \
        double __sum = std::accumulate(__times.begin(), __times.end(), 0.0); \
        double __mean = __sum / __times.size();                          \
        double __median = __times[__times.size() / 2];                   \
        double __min = __times.front();                                  \
        std::cout << "[" << (description) << "]\n";                       \
        std::cout << "  平均: " << std::fixed << std::setprecision(2)    \
                  << __mean << " ns\n";                                  \
        std::cout << "  中位数: " << __median << " ns\n";                \
        std::cout << "  最小值: " << __min << " ns";                     \
        std::cout << "  (" << (measure) << " iters)\n";                  \
    } while(0)

// ── 被测函数 ──────────────────────────────────────────────

constexpr int WORK_SIZE = 10000;

void vectorNoReserve() {
    std::vector<int> v;
    for (int i = 0; i < WORK_SIZE; ++i) {
        v.push_back(i);
    }
    doNotOptimize(v.data());
}

void vectorWithReserve() {
    std::vector<int> v;
    v.reserve(WORK_SIZE);
    for (int i = 0; i < WORK_SIZE; ++i) {
        v.push_back(i);
    }
    doNotOptimize(v.data());
}

constexpr int MAT_SIZE = 2048;
static int matrix[MAT_SIZE][MAT_SIZE];

void initMatrix() {
    for (int i = 0; i < MAT_SIZE; ++i)
        for (int j = 0; j < MAT_SIZE; ++j)
            matrix[i][j] = i + j;
}

int rowMajorSum() {
    int sum = 0;
    for (int i = 0; i < MAT_SIZE; ++i)
        for (int j = 0; j < MAT_SIZE; ++j)
            sum += matrix[i][j];
    return sum;
}

int columnMajorSum() {
    int sum = 0;
    for (int j = 0; j < MAT_SIZE; ++j)
        for (int i = 0; i < MAT_SIZE; ++i)
            sum += matrix[i][j];
    return sum;
}

int main() {
    std::cout << "========== BENCHMARK 宏练习 ==========\n\n";

    initMatrix();

    std::cout << "── 测试1: vector::reserve 效果 ──\n";
    BENCHMARK({ vectorNoReserve(); }, 100, "push_back 无 reserve");
    BENCHMARK({ vectorWithReserve(); }, 100, "push_back 有 reserve");

    std::cout << "\n── 测试2: Row-major vs Column-major ──\n";
    BENCHMARK({ doNotOptimize(rowMajorSum()); }, 10, "row-major 求和");
    BENCHMARK({ doNotOptimize(columnMajorSum()); }, 10, "column-major 求和");

    std::cout << "\n── 测试3 (进阶): BENCHMARK_MULTI 带统计 ──\n";
    BENCHMARK_MULTI({ doNotOptimize(rowMajorSum()); }, 2, 5, "row-major (带统计)");

    return 0;
}
