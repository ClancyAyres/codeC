// Exercise 04: Optimization Levels — 测试不同编译优化级别
// TODO: 实现函数并观察 -O0/-O1/-O2/-O3 下的性能和行为差异

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <random>
#include <cmath>
#include <numeric>
#include <algorithm>

// ── 基准测试工具（已提供） ────────────────────────────────

template<typename F>
double benchmark(F&& func, int iterations) {
    func();  // 预热
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) func();
    auto end = std::chrono::steady_clock::now();
    auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return static_cast<double>(total.count()) / iterations;
}

template<typename T>
void doNotOptimize(T const& value) {
    asm volatile("" : : "r,m"(value) : "memory");
}

// ── 测试函数实现 ──────────────────────────────────────────

// TODO 1: 实现多项式求值
// Horner 方法：a0 + x*(a1 + x*(a2 + x*a3))
// vs
// 直接计算：a0 + a1*x + a2*x*x + a3*x*x*x
//
// 测试循环中对同一个 x 重复求值。
// - 如果编译器和 x 都可以看到是常量 → 可能完全优化掉
// - Horner 法减少乘法次数 → 理论上更快
double hornerPoly(const std::vector<double>& coeffs, double x) {
    // TODO: 用 Horner 方法计算多项式值
    // coeffs[0] + x*(coeffs[1] + x*(coeffs[2] + ...))
    return 0.0;
}

double directPoly(const std::vector<double>& coeffs, double x) {
    // TODO: 直接逐项计算
    // coeffs[0] + coeffs[1]*x + coeffs[2]*x^2 + ...
    return 0.0;
}

// TODO 2: 实现死代码消除测试
// 写一个函数，内部有计算但返回值不参与 main 输出。
// - -O0: 所有计算都执行
// - -O2/-O3: 未使用的计算结果可能被完全消除（函数变空）
int deadCodeTest(int n) {
    // TODO:
    // int result = 0;
    // for (int i = 0; i < n; ++i) result += i;
    // return result; // 如果调用者不用返回值 → 整个循环可能被优化掉
    return 0;
}

// TODO 3: 实现常量折叠测试
// 写一个看似复杂但编译期可完全计算的函数。
// - -O0: 运行时计算
// - -O1+: 编译期折叠为常量
int compileTimeFold() {
    // TODO: return (1+2+3+4+5+6+7+8+9+10) * (1+2+3+4+5+6+7+8+9+10);
    // 在 -O1+ 下编译为 return 3025;
    return 0;
}

// TODO 4: 实现循环不变量外提测试
// 写一个循环，内部有不随循环变化的计算。
// - -O0: 每次迭代都计算
// - -O1+: 提到循环外（loop-invariant code motion, LICM）
double loopInvariantTest(const std::vector<double>& arr) {
    // TODO:
    // double sum = 0.0;
    // for (size_t i = 0; i < arr.size(); ++i) {
    //     sum += arr[i] * std::sqrt(2.0) * std::sin(3.14); // std::sqrt 和 std::sin 结果不变
    // }
    // -O1+: 编译器把 sqrt(2.0)*sin(3.14) 提到循环外 → 只算一次
    return 0.0;
}

// ── main ──────────────────────────────────────────────────

int main() {
    std::cout << "========== 编译优化级别对比 ==========\n";
    std::cout << "当前优化: "
#ifdef __OPTIMIZE__
        << "-O" << static_cast<int>(__OPTIMIZE__)
#else
        << "-O0"
#endif
        << "\n\n";

    // 准备数据
    constexpr int N = 1'000'000;
    constexpr int ITER = 20;
    std::vector<double> coeffs = {1.0, 0.5, 0.25, 0.125, 0.0625};
    std::vector<double> arr(N);
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(0.0, 100.0);
    for (int i = 0; i < N; ++i) arr[i] = dist(rng);

    // ── 测试1: 多项式求值 ──────────────────────────────
    std::cout << "── 测试1: 多项式求值（Horner vs 直接） ──\n";

    // TODO: 调用你的实现
    // double polyH = benchmark([&] {
    //     double s = 0;
    //     for (int i = 0; i < N; ++i) s += hornerPoly(coeffs, arr[i]);
    //     doNotOptimize(s);
    // }, ITER);
    // std::cout << "  Horner: " << polyH << " us\n";

    // double polyD = benchmark([&] {
    //     double s = 0;
    //     for (int i = 0; i < N; ++i) s += directPoly(coeffs, arr[i]);
    //     doNotOptimize(s);
    // }, ITER);
    // std::cout << "  直接: " << polyD << " us\n";

    std::cout << "  TODO: 实现多项式求值函数\n\n";

    // ── 测试2: 死代码消除 ──────────────────────────────
    std::cout << "── 测试2: 死代码消除 ──\n";
    double tDCE = benchmark([&] {
        deadCodeTest(1000000);  // 返回值未使用 → 可能被消除
    }, ITER);
    std::cout << "  deadCodeTest(1e6), 返回值未使用: "
              << std::fixed << std::setprecision(2) << tDCE << " us\n";
    std::cout << "  观察: -O0 执行完整计算，-O2 可能 0 us（被消除）\n\n";

    // ── 测试3: 常量折叠 ──────────────────────────────────
    std::cout << "── 测试3: 常量折叠 ──\n";
    double tConstFold = benchmark([&] {
        int r = compileTimeFold();
        doNotOptimize(r);
    }, ITER);
    std::cout << "  compileTimeFold(): " << tConstFold << " us\n";
    std::cout << "  观察: -O1+ 编译为 return 3025 → 极快\n\n";

    // ── 测试4: 循环不变量外提 ────────────────────────────
    std::cout << "── 测试4: 循环不变量外提 (LICM) ──\n";
    double tLICM = benchmark([&] {
        double r = loopInvariantTest(arr);
        doNotOptimize(r);
    }, ITER);
    std::cout << "  loopInvariantTest(N=" << N << "): " << tLICM << " us\n";
    std::cout << "  观察: -O0 每次迭代算 sqrt/sin，-O1+ 只算一次\n\n";

    std::cout << "========== 操作指南 ==========\n";
    std::cout << "用不同优化级别编译并对比时间：\n";
    std::cout << "  g++ -O0 main.cpp -o bench_O0 && ./bench_O0\n";
    std::cout << "  g++ -O1 main.cpp -o bench_O1 && ./bench_O1\n";
    std::cout << "  g++ -O2 main.cpp -o bench_O2 && ./bench_O2\n";
    std::cout << "  g++ -O3 main.cpp -o bench_O3 && ./bench_O3\n";
    std::cout << "  g++ -O3 -march=native -ffast-math main.cpp -o bench_O3native && ./bench_O3native\n";

    return 0;
}
