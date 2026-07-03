// Example 03: Chrono Benchmark — 基于 chrono 的基准测试框架
// 目的：展示如何编写可靠的微基准测试（替代 Google Benchmark）

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <string>
#include <functional>
#include <algorithm>
#include <cmath>
#include <random>
#include <numeric>

// ── 基准测试框架 ──────────────────────────────────────────

template<typename T>
void doNotOptimize(T const& value) {
    asm volatile("" : : "r,m"(value) : "memory");
}

void clobberMemory() {
    asm volatile("" : : : "memory");
}

struct BenchmarkResult {
    std::string name;
    double meanNs;          // 平均耗时 (纳秒)
    double medianNs;        // 中位数
    double stddevNs;        // 标准差
    double minNs;           // 最小值
    int iterations;

    void print() const {
        std::cout << std::setw(35) << std::left << name
                  << std::right
                  << std::setw(12) << std::fixed << std::setprecision(2) << meanNs
                  << std::setw(12) << medianNs
                  << std::setw(12) << minNs
                  << "  (" << iterations << " iters)\n";
    }
};

template<typename F>
BenchmarkResult benchmark(const std::string& name, F func,
                           int warmupRounds = 3,
                           int measureRounds = 10) {
    // 预热
    for (int i = 0; i < warmupRounds; ++i) {
        func();
    }

    // 测量
    std::vector<double> times;
    times.reserve(measureRounds);

    for (int i = 0; i < measureRounds; ++i) {
        auto start = std::chrono::steady_clock::now();
        func();
        auto end = std::chrono::steady_clock::now();
        auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        times.push_back(static_cast<double>(ns.count()));
    }

    // 统计
    std::sort(times.begin(), times.end());
    double sum = std::accumulate(times.begin(), times.end(), 0.0);
    double mean = sum / times.size();
    double median = times[times.size() / 2];
    double min = times.front();

    double variance = 0.0;
    for (double t : times) {
        variance += (t - mean) * (t - mean);
    }
    double stddev = std::sqrt(variance / times.size());

    return {name, mean, median, stddev, min, measureRounds};
}

// ── 被测代码 ──────────────────────────────────────────────

// 场景1: string 拼接方式对比
constexpr int STR_ITERS = 10000;

std::string concatPlus() {
    std::string result;
    for (int i = 0; i < STR_ITERS; ++i) {
        result += "a";                        // operator+= 可能复用内存
    }
    return result;
}

std::string concatPlusEq() {
    std::string result;
    result.reserve(STR_ITERS);
    for (int i = 0; i < STR_ITERS; ++i) {
        result += 'a';                        // char 版本更快
    }
    return result;
}

// 场景2: 分支预测
constexpr size_t SORT_SIZE = 32768;

int countIfWithSort(const std::vector<int>& arr, int threshold) {
    int count = 0;
    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr[i] < threshold)               // 排序后分支可预测
            ++count;
    }
    return count;
}

// 场景3: pow(x,2) vs x*x
constexpr int MATH_ITERS = 1000000;

double computePow2() {
    double sum = 0.0;
    for (int i = 0; i < MATH_ITERS; ++i) {
        sum += std::pow(static_cast<double>(i) / MATH_ITERS, 2.0);
    }
    return sum;
}

double computeX2() {
    double sum = 0.0;
    for (int i = 0; i < MATH_ITERS; ++i) {
        double x = static_cast<double>(i) / MATH_ITERS;
        sum += x * x;
    }
    return sum;
}

// ── main ──────────────────────────────────────────────────

int main() {
    std::cout << "========== Chrono 基准测试框架演示 ==========\n";
    std::cout << "编译优化: "
#ifdef __OPTIMIZE__
        << "-O" << static_cast<int>(__OPTIMIZE__)
#else
        << "-O0"
#endif
        << "\n\n";

    // ── 测试1: String 拼接 ──────────────────────────────
    std::cout << "── 测试1: String 拼接 (×" << STR_ITERS << ") ──\n";
    std::cout << std::setw(35) << std::left << "Name"
              << std::setw(12) << "Mean(ns)"
              << std::setw(12) << "Median(ns)"
              << std::setw(12) << "Min(ns)" << "  Info\n";
    std::cout << std::string(80, '-') << '\n';

    auto r1 = benchmark("operator+= string",
        [] { doNotOptimize(concatPlus()); },
        3, 10);
    r1.print();

    auto r2 = benchmark("operator+= char + reserve",
        [] { doNotOptimize(concatPlusEq()); },
        3, 10);
    r2.print();

    std::cout << "  → reserve + char 更快：避免多次分配和 SSO 策略差异\n\n";

    // ── 测试2: 分支预测 ─────────────────────────────────
    std::cout << "── 测试2: 分支预测 (size=" << SORT_SIZE << ") ──\n";
    std::cout << std::setw(35) << std::left << "Name"
              << std::setw(12) << "Mean(ns)"
              << std::setw(12) << "Median(ns)"
              << std::setw(12) << "Min(ns)" << "  Info\n";
    std::cout << std::string(80, '-') << '\n';

    std::vector<int> arr(SORT_SIZE);
    std::mt19937 rng(42);
    for (size_t i = 0; i < SORT_SIZE; ++i)
        arr[i] = rng() % 100000;

    // 未排序版本
    auto r3 = benchmark("unsorted array",
        [&] { doNotOptimize(countIfWithSort(arr, 50000)); },
        5, 10);
    r3.print();

    // 排序后版本
    std::sort(arr.begin(), arr.end());
    auto r4 = benchmark("sorted array",
        [&] { doNotOptimize(countIfWithSort(arr, 50000)); },
        5, 10);
    r4.print();

    std::cout << "  → 排序后分支可预测 → 快 3-6×（CPU 分支预测器生效）\n\n";

    // ── 测试3: pow vs x*x ────────────────────────────────
    std::cout << "── 测试3: pow(x,2) vs x*x (×" << MATH_ITERS << ") ──\n";
    std::cout << std::setw(35) << std::left << "Name"
              << std::setw(12) << "Mean(ns)"
              << std::setw(12) << "Median(ns)"
              << std::setw(12) << "Min(ns)" << "  Info\n";
    std::cout << std::string(80, '-') << '\n';

    auto r5 = benchmark("std::pow(x, 2.0)",
        [] { doNotOptimize(computePow2()); },
        3, 10);
    r5.print();

    auto r6 = benchmark("x * x",
        [] { doNotOptimize(computeX2()); },
        3, 10);
    r6.print();

    std::cout << "  → x*x 远快于 pow( ,2)：pow 是通用幂运算，开销大\n";

    return 0;
}
