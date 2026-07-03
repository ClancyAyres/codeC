// Example 03: 数值稳定 Softmax — 朴素 vs 稳定实现（减 max 防溢出）
// 演示 exp 溢出问题及减最大值解法的数学正确性

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <chrono>

template<typename F>
double benchmark(F&& func, int warmup, int iterations) {
    for (int i = 0; i < warmup; ++i) func();
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

// ── 朴素 Softmax（不稳定） ───────────────────────────────
template<typename T>
void softmaxNaive(const T* x, T* y, size_t n) {
    T sum = 0;
    for (size_t i = 0; i < n; ++i) {
        y[i] = std::exp(x[i]);       // 危险：exp(大数) → +inf
        sum += y[i];
    }
    for (size_t i = 0; i < n; ++i)
        y[i] /= sum;
}

// ── 稳定 Softmax（减最大值） ─────────────────────────────
template<typename T>
void softmaxStable(const T* x, T* y, size_t n) {
    T maxVal = x[0];
    for (size_t i = 1; i < n; ++i)
        if (x[i] > maxVal) maxVal = x[i];

    T sum = 0;
    for (size_t i = 0; i < n; ++i) {
        y[i] = std::exp(x[i] - maxVal);
        sum += y[i];
    }
    for (size_t i = 0; i < n; ++i)
        y[i] /= sum;
}

// ── Log-Softmax（进一步稳定） ────────────────────────────
template<typename T>
void logSoftmax(const T* x, T* y, size_t n) {
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

// ── 矩阵 Softmax（沿 axis=1 即每行独立） ─────────────────
template<typename T>
void softmax2D(const T* x, T* y, size_t rows, size_t cols) {
    for (size_t r = 0; r < rows; ++r)
        softmaxStable(x + r * cols, y + r * cols, cols);
}

// ── 辅助打印 ────────────────────────────────────────────
template<typename T>
void printVec(const char* label, const T* data, size_t n) {
    std::cout << "  " << label << ": [";
    for (size_t i = 0; i < n; ++i)
        std::cout << std::fixed << std::setprecision(4) << data[i]
                  << (i < n - 1 ? ", " : "");
    std::cout << "]\n";
}

// ── 验证 softmax 输出和为 1 ─────────────────────────────
template<typename T>
bool verifySumToOne(const T* y, size_t n, T eps = 1e-6) {
    T sum = 0;
    for (size_t i = 0; i < n; ++i) sum += y[i];
    return std::abs(sum - T(1)) < eps;
}

int main() {
    std::cout << "========== 数值稳定 Softmax ==========\n\n";

    // ── 1. 正常输入 ─────────────────────────────────────
    std::cout << "── 1. 正常输入（无溢出风险）──\n";
    float x1[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    constexpr size_t n1 = sizeof(x1) / sizeof(x1[0]);
    float y1[n1], y2[n1];

    printVec("输入   ", x1, n1);
    softmaxNaive(x1, y1, n1);
    softmaxStable(x1, y2, n1);
    printVec("朴素   ", y1, n1);
    printVec("稳定   ", y2, n1);
    std::cout << "  朴素 sum = 1? " << (verifySumToOne(y1, n1) ? "✓" : "✗") << "\n";
    std::cout << "  稳定 sum = 1? " << (verifySumToOne(y2, n1) ? "✓" : "✗") << "\n\n";

    // ── 2. 中等输入（朴素开始不准） ─────────────────────
    std::cout << "── 2. 较大输入（朴素可能不准确）──\n";
    float x2[] = {50.0f, 51.0f, 52.0f, 53.0f, 54.0f};
    constexpr size_t n2 = sizeof(x2) / sizeof(x2[0]);
    float y3[n2], y4[n2];

    printVec("输入   ", x2, n2);
    softmaxNaive(x2, y3, n2);
    softmaxStable(x2, y4, n2);
    printVec("朴素   ", y3, n2);
    printVec("稳定   ", y4, n2);
    std::cout << "  朴素 sum = 1? " << (verifySumToOne(y3, n2) ? "✓" : "✗") << "\n";
    std::cout << "  稳定 sum = 1? " << (verifySumToOne(y4, n2) ? "✓" : "✗") << "\n\n";

    // ── 3. 极端输入（朴素必然溢出） ──────────────────────
    std::cout << "── 3. 极端输入（朴素必然溢出）──\n";
    float x3[] = {88.0f, 89.0f, 90.0f, 91.0f, 92.0f};
    constexpr size_t n3 = sizeof(x3) / sizeof(x3[0]);
    float y5[n3], y6[n3];

    printVec("输入   ", x3, n3);
    softmaxStable(x3, y5, n3);
    printVec("稳定   ", y5, n3);
    std::cout << "  稳定 sum = 1? " << (verifySumToOne(y5, n3) ? "✓" : "✗") << "\n";

    std::cout << "  朴素 (预测): exp(92) = " << std::exp(92.0f) << " → +inf → NaN\n";
    std::cout << "  分析: exp(92-92=0)=1, exp(91-92=-1)=1/e≈0.368, ...\n";
    std::cout << "        所有 exp(x_i - max) ≤ 1 → 永不溢出\n\n";

    // ── 4. Log-Softmax 演示 ─────────────────────────────
    std::cout << "── 4. Log-Softmax ──\n";
    float logY[n1];
    logSoftmax(x1, logY, n1);
    printVec("输入      ", x1, n1);
    printVec("LogSoftmax", logY, n1);
    std::cout << "  验证: exp(LogSoftmax[i]) = Softmax[i]\n";
    for (size_t i = 0; i < n1; ++i)
        std::cout << "    " << std::exp(logY[i]) << " vs " << y2[i] << "\n";

    // ── 5. 2D Softmax ──────────────────────────────────
    std::cout << "\n── 5. 2D Softmax (沿 axis=1，每行独立) ──\n";
    float x4[] = {
        1.0f, 2.0f, 3.0f,   // 第0行
        4.0f, 5.0f, 6.0f,   // 第1行
        7.0f, 8.0f, 9.0f    // 第2行
    };
    constexpr size_t rows = 3, cols = 3;
    float y7[rows * cols];
    softmax2D(x4, y7, rows, cols);

    for (size_t r = 0; r < rows; ++r) {
        std::cout << "  行" << r << ": [";
        for (size_t c = 0; c < cols; ++c)
            std::cout << std::setprecision(4) << y7[r * cols + c] << (c < cols-1 ? ", " : "");
        std::cout << "]";
        float rowSum = 0;
        for (size_t c = 0; c < cols; ++c) rowSum += y7[r * cols + c];
        std::cout << "  sum=" << rowSum << "\n";
    }

    // ── 6. 性能对比 ─────────────────────────────────────
    std::cout << "\n── 6. 性能对比 (N = 1,000,000) ──\n";
    constexpr size_t BIG = 1'000'000;
    std::vector<float> bigX(BIG), bigY(BIG);
    for (size_t i = 0; i < BIG; ++i) bigX[i] = static_cast<float>(i % 100);

    auto benchNaive = [&] {
        softmaxNaive(bigX.data(), bigY.data(), BIG);
        doNotOptimize(bigY[0]);
    };
    auto benchStable = [&] {
        softmaxStable(bigX.data(), bigY.data(), BIG);
        doNotOptimize(bigY[0]);
    };

    double tNaive = benchmark(benchNaive, 5, 10);
    double tStable = benchmark(benchStable, 5, 10);
    std::cout << "  朴素:  " << std::setprecision(2) << tNaive << " us/iter\n";
    std::cout << "  稳定:  " << std::setprecision(2) << tStable << " us/iter\n";
    std::cout << "  额外开销: 仅多一次 max 遍历 ≈ " << std::setprecision(1)
              << ((tStable / tNaive - 1.0) * 100) << "%（换取完全稳定性）\n";

    std::cout << "\n── 结论 ──\n";
    std::cout << "减 max 技巧适用于: softmax, log-softmax, cross-entropy loss\n";
    std::cout << "数学正确性: softmax(x)_i = exp(x_i - max) / Σ exp(x_j - max)\n";
    std::cout << "性能代价: 微乎其微（一次额外扫描），必须永远使用稳定版本\n";

    return 0;
}
