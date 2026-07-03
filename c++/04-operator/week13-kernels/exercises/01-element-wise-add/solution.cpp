// Exercise 01: 模板化逐元素加法 — 完整解法

#include <iostream>
#include <vector>
#include <functional>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <algorithm>

// ── 函数模板1：逐元素加法 ───────────────────────────────
template<typename T>
void elementWiseAdd(const T* a, const T* b, T* c, size_t n) {
    for (size_t i = 0; i < n; ++i)
        c[i] = a[i] + b[i];
}

// ── 函数模板2：标量广播乘法 ─────────────────────────────
template<typename T>
void scalarMultiply(const T* in, T scalar, T* out, size_t n) {
    for (size_t i = 0; i < n; ++i)
        out[i] = in[i] * scalar;
}

// ── 函数模板3：原地 ReLU ────────────────────────────────
template<typename T>
void inPlaceRelu(T* data, size_t n) {
    for (size_t i = 0; i < n; ++i)
        if (data[i] < 0) data[i] = 0;
}

// ── 辅助函数 ────────────────────────────────────────────

template<typename T>
void printVec(const char* label, const T* data, size_t n) {
    std::cout << "  " << label << ": [";
    for (size_t i = 0; i < n && i < 10; ++i)
        std::cout << data[i] << (i < n - 1 && i < 9 ? ", " : "");
    if (n > 10) std::cout << ", ...";
    std::cout << "]\n";
}

template<typename T>
bool compare(const T* a, const T* b, size_t n) {
    for (size_t i = 0; i < n; ++i)
        if (std::abs(a[i] - b[i]) > 1e-9f) return false;
    return true;
}

template<typename T>
void testCase(const char* name, const T* result, const T* expected, size_t n) {
    bool pass = compare(result, expected, n);
    std::cout << "  " << name << ": " << (pass ? "✓ 通过" : "✗ 失败") << "\n";
    if (!pass) {
        std::cout << "    期望: "; for (size_t i = 0; i < n; ++i) std::cout << expected[i] << " "; std::cout << "\n";
        std::cout << "    实际: "; for (size_t i = 0; i < n; ++i) std::cout << result[i] << " "; std::cout << "\n";
    }
}

int main() {
    std::cout << "========== 习题01: 模板化逐元素加法 ==========\n\n";

    // ── 测试1: float 加法 ───────────────────────────────
    {
        std::cout << "── 测试1: float 加法 ──\n";
        float a[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
        float b[] = {10.0f, 20.0f, 30.0f, 40.0f, 50.0f};
        float expected[] = {11.0f, 22.0f, 33.0f, 44.0f, 55.0f};
        constexpr size_t n = sizeof(a) / sizeof(a[0]);
        float c[n];

        elementWiseAdd(a, b, c, n);
        printVec("输入a", a, n);
        printVec("输入b", b, n);
        printVec("结果c", c, n);
        testCase("float加法", c, expected, n);
    }

    // ── 测试2: int 加法 ─────────────────────────────────
    {
        std::cout << "\n── 测试2: int 加法 ──\n";
        int a[] = {1, 2, 3, 4, 5};
        int b[] = {10, 20, 30, 40, 50};
        int expected[] = {11, 22, 33, 44, 55};
        constexpr size_t n = sizeof(a) / sizeof(a[0]);
        int c[n];

        elementWiseAdd(a, b, c, n);
        printVec("输入a", a, n);
        printVec("输入b", b, n);
        printVec("结果c", c, n);
        testCase("int加法", c, expected, n);
    }

    // ── 测试3: 标量广播乘法 ─────────────────────────────
    {
        std::cout << "\n── 测试3: 标量广播乘法 ──\n";
        double in[] = {1.0, 2.0, 3.0, 4.0, 5.0};
        double scalar = 2.5;
        double expected[] = {2.5, 5.0, 7.5, 10.0, 12.5};
        constexpr size_t n = sizeof(in) / sizeof(in[0]);
        double out[n];

        scalarMultiply(in, scalar, out, n);
        printVec("输入", in, n);
        std::cout << "  标量: " << scalar << "\n";
        printVec("结果", out, n);
        testCase("标量乘法", out, expected, n);
    }

    // ── 测试4: 原地 ReLU ────────────────────────────────
    {
        std::cout << "\n── 测试4: 原地 ReLU ──\n";
        float data[] = {-3.0f, -1.0f, 0.0f, 2.0f, 5.0f, -0.5f};
        float expected[] = {0.0f, 0.0f, 0.0f, 2.0f, 5.0f, 0.0f};
        constexpr size_t n = sizeof(data) / sizeof(data[0]);

        printVec("输入", data, n);
        inPlaceRelu(data, n);
        printVec("结果", data, n);
        testCase("原地ReLU", data, expected, n);
    }

    // ── 测试5: 大型数组压力测试 ─────────────────────────
    {
        std::cout << "\n── 测试5: 大型数组 ──\n";
        constexpr size_t BIG = 1'000'000;
        std::vector<float> a(BIG, 1.0f), b(BIG, 2.0f), c(BIG);
        elementWiseAdd(a.data(), b.data(), c.data(), BIG);
        bool pass = true;
        for (size_t i = 0; i < BIG; ++i)
            if (c[i] != 3.0f) { pass = false; break; }
        std::cout << "  " << BIG << " 元素加法: " << (pass ? "✓ 通过" : "✗ 失败") << "\n";
    }

    std::cout << "\n── 全部测试完成 ──\n";
    return 0;
}
