// Example 01: 逐元素操作模板化 — 模板化 element-wise 操作
// 演示一元、二元、组合操作的模板设计

#include <iostream>
#include <vector>
#include <functional>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <numeric>

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
void doNotOptimize(T const& val) {
    asm volatile("" : : "r,m"(val) : "memory");
}

// ── 一元逐元素操作模板 ──────────────────────────────────
template<typename T, typename UnaryOp>
void elementWiseUnary(const T* input, T* output, size_t n, UnaryOp op) {
    for (size_t i = 0; i < n; ++i)
        output[i] = op(input[i]);
}

// ── 二元逐元素操作模板 ──────────────────────────────────
template<typename T, typename BinaryOp>
void elementWiseBinary(const T* a, const T* b, T* out, size_t n, BinaryOp op) {
    for (size_t i = 0; i < n; ++i)
        out[i] = op(a[i], b[i]);
}

// ── 原位置一元操作 ──────────────────────────────────────
template<typename T, typename UnaryOp>
void elementWiseInPlace(T* data, size_t n, UnaryOp op) {
    for (size_t i = 0; i < n; ++i)
        data[i] = op(data[i]);
}

// ── 组合操作：二元 → 一元 链式 ─────────────────────────
template<typename T>
void addThenRelu(const T* a, const T* b, T* out, size_t n) {
    auto add = [](T x, T y) { return x + y; };
    auto relu = [](T x) { return x > 0 ? x : 0; };

    std::vector<T> tmp(n);
    elementWiseBinary(a, b, tmp.data(), n, add);
    elementWiseUnary(tmp.data(), out, n, relu);
}

// ── 标量广播 ────────────────────────────────────────────
template<typename T, typename BinaryOp>
void elementWiseScalar(const T* a, T scalar, T* out, size_t n, BinaryOp op) {
    for (size_t i = 0; i < n; ++i)
        out[i] = op(a[i], scalar);
}

// ── 辅助打印 ────────────────────────────────────────────
template<typename T>
void printVec(const char* label, const T* data, size_t n) {
    std::cout << "  " << label << ": [";
    for (size_t i = 0; i < n && i < 10; ++i)
        std::cout << std::fixed << std::setprecision(2) << data[i] << (i < n-1 && i < 9 ? ", " : "");
    if (n > 10) std::cout << ", ...";
    std::cout << "]\n";
}

int main() {
    std::cout << "========== 逐元素操作模板化 ==========\n\n";

    constexpr size_t N = 8;
    float a[N] = {1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f};
    float b[N] = {0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f};
    float c[N];

    std::cout << "── 一元操作 ──\n";
    printVec("输入 a", a, N);

    elementWiseUnary(a, c, N, [](float x) { return -x; });
    printVec("取反   ", c, N);

    elementWiseUnary(a, c, N, [](float x) { return std::abs(x); });
    printVec("绝对值 ", c, N);

    elementWiseUnary(a, c, N, [](float x) { return x > 0 ? x : 0.0f; });
    printVec("ReLU   ", c, N);

    elementWiseUnary(a, c, N, [](float x) { return 1.0f / (1.0f + std::exp(-x)); });
    printVec("Sigmoid", c, N);

    std::cout << "\n── 二元操作 ──\n";
    printVec("a", a, N);
    printVec("b", b, N);

    elementWiseBinary(a, b, c, N, std::plus<float>{});
    printVec("a + b  ", c, N);
    elementWiseBinary(a, b, c, N, std::multiplies<float>{});
    printVec("a * b  ", c, N);
    elementWiseBinary(a, b, c, N, [](float x, float y) { return std::max(x, y); });
    printVec("max(a,b)", c, N);

    std::cout << "\n── 标量广播 ──\n";
    elementWiseScalar(a, 2.0f, c, N, std::multiplies<float>{});
    printVec("a * 2.0 (广播)", c, N);

    std::cout << "\n── 组合操作：addThenRelu ──\n";
    addThenRelu(a, b, c, N);
    printVec("ReLU(a + b)", c, N);

    // ── 性能对比：模板 vs 手动循环 ──────────────────────
    std::cout << "\n── 性能对比 (N = 10,000,000) ──\n";
    constexpr size_t BIG = 10'000'000;
    std::vector<float> bigA(BIG), bigB(BIG), bigC(BIG);
    std::iota(bigA.begin(), bigA.end(), 0.0f);
    std::iota(bigB.begin(), bigB.end(), 1.0f);

    auto manualAdd = [&] {
        for (size_t i = 0; i < BIG; ++i) bigC[i] = bigA[i] + bigB[i];
        doNotOptimize(bigC[0]);
    };

    auto templateAdd = [&] {
        elementWiseBinary(bigA.data(), bigB.data(), bigC.data(),
                          BIG, std::plus<float>{});
        doNotOptimize(bigC[0]);
    };

    auto lambdaAdd = [&] {
        elementWiseBinary(bigA.data(), bigB.data(), bigC.data(),
                          BIG, [](float x, float y) { return x + y; });
        doNotOptimize(bigC[0]);
    };

    double t1 = benchmark(manualAdd, 20);
    double t2 = benchmark(templateAdd, 20);
    double t3 = benchmark(lambdaAdd, 20);

    std::cout << "  手动循环:     " << std::setprecision(2) << t1 << " us/iter\n";
    std::cout << "  模板+std::plus: " << std::setprecision(2) << t2 << " us/iter\n";
    std::cout << "  模板+lambda:    " << std::setprecision(2) << t3 << " us/iter\n";
    std::cout << "  结论: 模板不引入运行时开销（与手动循环相同）\n";
    std::cout << "        std::plus 可能略快于 lambda（编译器更易内联）\n";

    return 0;
}
