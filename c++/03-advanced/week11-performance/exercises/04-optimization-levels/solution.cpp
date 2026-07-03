// Exercise 04: Optimization Levels — 完整解法

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <random>
#include <cmath>
#include <numeric>
#include <algorithm>

template<typename F>
double benchmark(F&& func, int iterations) {
    func();
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

// ── 1. 多项式求值 ──────────────────────────────────────

double hornerPoly(const std::vector<double>& coeffs, double x) {
    double result = 0.0;
    for (int i = static_cast<int>(coeffs.size()) - 1; i >= 0; --i) {
        result = result * x + coeffs[i];
    }
    return result;
}

double directPoly(const std::vector<double>& coeffs, double x) {
    double result = 0.0;
    double xPower = 1.0;
    for (size_t i = 0; i < coeffs.size(); ++i) {
        result += coeffs[i] * xPower;
        xPower *= x;
    }
    return result;
}

// ── 2. 死代码消除测试 ──────────────────────────────────

int deadCodeTest(int n) {
    int result = 0;
    for (int i = 0; i < n; ++i) {
        result += i;
    }
    return result;
}

// ── 3. 常量折叠测试 ────────────────────────────────────

int compileTimeFold() {
    // 在 -O1+ 下被折叠为 return 3025;
    int a = 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10;
    return a * a;
}

// ── 4. 循环不变量外提测试 (LICM) ───────────────────────

double loopInvariantTest(const std::vector<double>& arr) {
    double sum = 0.0;
    // 故意把不变量放在循环内——观察编译器是否自动外提
    for (size_t i = 0; i < arr.size(); ++i) {
        double invariant = std::sqrt(2.0) * std::sin(3.14);
        sum += arr[i] * invariant;
    }
    return sum;
}

// 对照组：手动外提
double loopInvariantManual(const std::vector<double>& arr) {
    double sum = 0.0;
    double invariant = std::sqrt(2.0) * std::sin(3.14);
    for (size_t i = 0; i < arr.size(); ++i) {
        sum += arr[i] * invariant;
    }
    return sum;
}

int main() {
    std::cout << "========== 编译优化级别对比 ==========\n";
    std::cout << "当前优化级别: "
#ifdef __OPTIMIZE__
        << "-O" << static_cast<int>(__OPTIMIZE__)
#else
        << "-O0（无优化）"
#endif
        << "\n\n";

    constexpr int N = 1'000'000;
    constexpr int ITER = 20;

    std::vector<double> coeffs = {1.0, 0.5, 0.25, 0.125, 0.0625};

    std::vector<double> arr(N);
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(0.0, 100.0);
    for (int i = 0; i < N; ++i) arr[i] = dist(rng);

    // ── 测试1 ──────────────────────────────────────────
    std::cout << "── 测试1: 多项式求值 (N=" << N << ") ──\n";

    double tHorner = benchmark([&] {
        double s = 0;
        for (int i = 0; i < N; ++i) s += hornerPoly(coeffs, arr[i]);
        doNotOptimize(s);
    }, ITER);
    std::cout << "  Horner 方法: " << std::fixed << std::setprecision(2)
              << tHorner << " us/iter\n";

    double tDirect = benchmark([&] {
        double s = 0;
        for (int i = 0; i < N; ++i) s += directPoly(coeffs, arr[i]);
        doNotOptimize(s);
    }, ITER);
    std::cout << "  直接方法: " << tDirect << " us/iter\n";
    std::cout << "  Horner/Direct = " << std::setprecision(2) << tHorner / tDirect << "\n\n";

    // ── 测试2 ──────────────────────────────────────────
    std::cout << "── 测试2: 死代码消除 ──\n";
    double tDCE = benchmark([&] {
        deadCodeTest(1000000);  // 返回值未使用!
    }, ITER);
    std::cout << "  返回值未使用: " << tDCE << " us/iter\n";
    std::cout << "  观察: -O0 完整执行, -O2+ 可能 ~0 us（整个调用被消除）\n\n";

    // ── 测试3 ──────────────────────────────────────────
    std::cout << "── 测试3: 常量折叠 ──\n";
    double tConstFold = benchmark([&] {
        int r = compileTimeFold();
        doNotOptimize(r);
    }, ITER);
    std::cout << "  compileTimeFold(): " << tConstFold << " us/iter\n";
    std::cout << "  观察: -O0 运行时算, -O1+ 编译为 return 3025\n\n";

    // ── 测试4 ──────────────────────────────────────────
    std::cout << "── 测试4: 循环不变量外提 (LICM) ──\n";

    double tLICM = benchmark([&] {
        double r = loopInvariantTest(arr);
        doNotOptimize(r);
    }, ITER);
    std::cout << "  不变量在循环内: " << tLICM << " us/iter\n";

    double tLICMManual = benchmark([&] {
        double r = loopInvariantManual(arr);
        doNotOptimize(r);
    }, ITER);
    std::cout << "  手动外提不变量: " << tLICMManual << " us/iter\n";

    if (tLICMManual > 0) {
        std::cout << "  内/外比值 = " << std::setprecision(2)
                  << tLICM / tLICMManual << "x\n";
        std::cout << "  (~1x 说明编译器已自动外提, >1x 说明未优化)\n";
    }
    std::cout << '\n';

    // ── 总结 ───────────────────────────────────────────
    std::cout << "========== 操作指南 ==========\n";
    std::cout << "用不同优化级别编译并对比时间：\n";
    std::cout << "  g++ -std=c++17 -O0 solution.cpp && ./a.out\n";
    std::cout << "  g++ -std=c++17 -O1 solution.cpp && ./a.out\n";
    std::cout << "  g++ -std=c++17 -O2 solution.cpp && ./a.out\n";
    std::cout << "  g++ -std=c++17 -O3 solution.cpp && ./a.out\n";
    std::cout << "  g++ -std=c++17 -O3 -march=native -ffast-math solution.cpp && ./a.out\n";

    return 0;
}
