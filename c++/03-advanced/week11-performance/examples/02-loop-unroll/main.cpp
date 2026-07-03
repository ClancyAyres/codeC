// Example 02: Loop Unroll — 循环展开对比
// 目的：展示手动循环展开 vs 编译器自动展开的效果

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <random>
#include <numeric>

constexpr size_t N = 50'000'000;
constexpr int ITERATIONS = 10;

template<typename F>
double benchmark(F&& func, int iterations) {
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        func();
    }
    auto end = std::chrono::steady_clock::now();
    auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return static_cast<double>(total.count()) / iterations;
}

// ── 版本1: 朴素循环 ─────────────────────────────────────────
long long sumSimple(const std::vector<int>& arr) {
    long long sum = 0;
    for (size_t i = 0; i < arr.size(); ++i) {
        sum += arr[i];
    }
    return sum;
}

// ── 版本2: 手动展开 ×2 ─────────────────────────────────────
long long sumUnroll2(const std::vector<int>& arr) {
    long long sum = 0;
    size_t i = 0;
    for (; i + 1 < arr.size(); i += 2) {
        sum += arr[i];
        sum += arr[i + 1];
    }
    for (; i < arr.size(); ++i) {
        sum += arr[i];
    }
    return sum;
}

// ── 版本3: 手动展开 ×4 ─────────────────────────────────────
long long sumUnroll4(const std::vector<int>& arr) {
    long long sum = 0;
    size_t i = 0;
    for (; i + 3 < arr.size(); i += 4) {
        sum += arr[i];
        sum += arr[i + 1];
        sum += arr[i + 2];
        sum += arr[i + 3];
    }
    for (; i < arr.size(); ++i) {
        sum += arr[i];
    }
    return sum;
}

// ── 版本4: 手动用多累加器展开 ×4（打破依赖链） ───────────────
long long sumUnroll4Acc(const std::vector<int>& arr) {
    long long sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;
    size_t i = 0;
    for (; i + 3 < arr.size(); i += 4) {
        sum0 += arr[i];
        sum1 += arr[i + 1];
        sum2 += arr[i + 2];
        sum3 += arr[i + 3];
    }
    for (; i < arr.size(); ++i) {
        sum0 += arr[i];
    }
    return sum0 + sum1 + sum2 + sum3;
}

// ── 版本5: 手动展开 ×8 ─────────────────────────────────────
long long sumUnroll8(const std::vector<int>& arr) {
    long long sum = 0;
    size_t i = 0;
    for (; i + 7 < arr.size(); i += 8) {
        sum += arr[i]   + arr[i + 1]
             + arr[i + 2] + arr[i + 3]
             + arr[i + 4] + arr[i + 5]
             + arr[i + 6] + arr[i + 7];
    }
    for (; i < arr.size(); ++i) {
        sum += arr[i];
    }
    return sum;
}

// ── 版本6: C++ accumulate ───────────────────────────────────
long long sumAccumulate(const std::vector<int>& arr) {
    return std::accumulate(arr.begin(), arr.end(), 0LL);
}

// ── 打印结果 ────────────────────────────────────────────────
void printResult(const char* name, double timeUs) {
    std::cout << "  " << std::left << std::setw(35) << name
              << std::right << std::setw(10) << std::fixed
              << std::setprecision(2) << timeUs << " us/iter\n";
}

int main() {
    std::cout << "========== 循环展开对比 (N = " << N << ") ==========\n";
    std::cout << "编译选项: " <<
#ifdef __OPTIMIZE__
        "-O" <<
#if __OPTIMIZE__ == 1
        "1"
#elif __OPTIMIZE__ == 2
        "2"
#elif __OPTIMIZE__ == 3
        "3"
#else
        "?"
#endif
#else
        "-O0（无优化）"
#endif
        << " | 迭代 " << ITERATIONS << " 次取平均\n\n";

    // 初始化
    std::vector<int> arr(N);
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(1, 100);
    for (size_t i = 0; i < N; ++i)
        arr[i] = dist(rng);

    long long ref = sumSimple(arr);

    // 基准测试
    printResult("01. 朴素循环",
        benchmark([&] { volatile auto r = sumSimple(arr); }, ITERATIONS));

    printResult("02. 展开 ×2",
        benchmark([&] { volatile auto r = sumUnroll2(arr); }, ITERATIONS));

    printResult("03. 展开 ×4",
        benchmark([&] { volatile auto r = sumUnroll4(arr); }, ITERATIONS));

    printResult("04. 展开 ×4 + 多累加器",
        benchmark([&] { volatile auto r = sumUnroll4Acc(arr); }, ITERATIONS));

    printResult("05. 展开 ×8",
        benchmark([&] { volatile auto r = sumUnroll8(arr); }, ITERATIONS));

    printResult("06. std::accumulate",
        benchmark([&] { volatile auto r = sumAccumulate(arr); }, ITERATIONS));

    std::cout << "\n========== 分析 ==========\n";
    std::cout << "- -O2/-O3 下编译器会自动循环展开，手动展开收益有限\n";
    std::cout << "- 多累加器打破数据依赖链（关键！），允许 CPU 级并行\n";
    std::cout << "- -O0 下手动展开收益巨大，但生产环境不应使用 -O0\n";
    std::cout << "- 过度手动展开可能增加指令缓存压力，适得其反\n";
    std::cout << "- 建议：信任编译器，用 -O3 -march=native\n";

    return 0;
}
