// Exercise 02: 多维归约 — 完整解法

#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <limits>

// ── 1. 一维归约 ────────────────────────────────────────
template<typename T>
T reduceSum1D(const T* data, size_t n) {
    T sum = T(0);
    for (size_t i = 0; i < n; ++i)
        sum += data[i];
    return sum;
}

// ── 2. 二维归约沿 axis=0 (列归约) ──────────────────────
template<typename T>
void reduceSumAxis0(const T* input, T* output,
                    size_t rows, size_t cols) {
    for (size_t c = 0; c < cols; ++c)
        output[c] = T(0);

    for (size_t r = 0; r < rows; ++r)
        for (size_t c = 0; c < cols; ++c)
            output[c] += input[r * cols + c];
}

// ── 3. 二维归约沿 axis=1 (行归约) ──────────────────────
template<typename T>
void reduceSumAxis1(const T* input, T* output,
                    size_t rows, size_t cols) {
    for (size_t r = 0; r < rows; ++r) {
        T sum = T(0);
        for (size_t c = 0; c < cols; ++c)
            sum += input[r * cols + c];
        output[r] = sum;
    }
}

// ── 4. 二维归约（通用版） ───────────────────────────────
template<typename T>
void reduceSum2D(const T* input, T* output,
                 size_t rows, size_t cols, size_t axis) {
    if (axis == 0) {
        reduceSumAxis0(input, output, rows, cols);
    } else if (axis == 1) {
        reduceSumAxis1(input, output, rows, cols);
    }
}

// ── 5. 二维求最大值沿 axis=1 ────────────────────────────
template<typename T>
void reduceMaxAxis1(const T* input, T* output,
                    size_t rows, size_t cols) {
    for (size_t r = 0; r < rows; ++r) {
        T maxVal = std::numeric_limits<T>::lowest();
        for (size_t c = 0; c < cols; ++c) {
            if (input[r * cols + c] > maxVal)
                maxVal = input[r * cols + c];
        }
        output[r] = maxVal;
    }
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
void printMat(const char* label, const T* data, size_t rows, size_t cols) {
    std::cout << "  " << label << ":\n";
    for (size_t r = 0; r < rows; ++r) {
        std::cout << "    [";
        for (size_t c = 0; c < cols; ++c)
            std::cout << std::setw(4) << data[r * cols + c] << (c < cols - 1 ? ", " : "");
        std::cout << "]\n";
    }
}

template<typename T>
bool compare(const T* a, const T* b, size_t n) {
    for (size_t i = 0; i < n; ++i)
        if (a[i] != b[i]) return false;
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
    std::cout << "========== 习题02: 多维归约 ==========\n\n";

    // ── 测试1: 一维归约 ─────────────────────────────────
    {
        std::cout << "── 测试1: 一维求和 ──\n";
        int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        constexpr size_t n = sizeof(data) / sizeof(data[0]);
        int result = reduceSum1D(data, n);
        printVec("输入", data, n);
        std::cout << "  结果: " << result << " (期望 55)\n";
        std::cout << "  " << (result == 55 ? "✓ 通过" : "✗ 失败") << "\n";
    }

    // ── 测试2: 二维沿 axis=0 (列求和) ───────────────────
    {
        std::cout << "\n── 测试2: 2D 沿 axis=0 (列求和) ──\n";
        int data[] = {
            1, 2, 3,
            4, 5, 6,
            7, 8, 9
        };
        int expected[] = {12, 15, 18};
        constexpr size_t rows = 3, cols = 3;
        int result[cols];

        printMat("输入矩阵", data, rows, cols);
        reduceSumAxis0(data, result, rows, cols);
        printVec("列求和", result, cols);
        testCase("axis=0 列求和", result, expected, cols);
    }

    // ── 测试3: 二维沿 axis=1 (行求和) ───────────────────
    {
        std::cout << "\n── 测试3: 2D 沿 axis=1 (行求和) ──\n";
        int data[] = {
            1, 2, 3,
            4, 5, 6,
            7, 8, 9
        };
        int expected[] = {6, 15, 24};
        constexpr size_t rows = 3, cols = 3;
        int result[rows];

        printMat("输入矩阵", data, rows, cols);
        reduceSumAxis1(data, result, rows, cols);
        printVec("行求和", result, rows);
        testCase("axis=1 行求和", result, expected, rows);
    }

    // ── 测试4: 二维通用归约 ─────────────────────────────
    {
        std::cout << "\n── 测试4: 通用 reduceSum2D ──\n";
        int data[] = {1, 2, 3, 4, 5, 6};  // shape (2, 3)
        constexpr size_t rows = 2, cols = 3;

        int result0[cols], result1[rows];
        int expected0[] = {5, 7, 9};   // axis=0: 列和
        int expected1[] = {6, 15};     // axis=1: 行和

        printMat("输入矩阵", data, rows, cols);

        reduceSum2D(data, result0, rows, cols, 0);
        printVec("axis=0 结果", result0, cols);
        testCase("通用 axis=0", result0, expected0, cols);

        reduceSum2D(data, result1, rows, cols, 1);
        printVec("axis=1 结果", result1, rows);
        testCase("通用 axis=1", result1, expected1, rows);
    }

    // ── 测试5: 最大值归约 ───────────────────────────────
    {
        std::cout << "\n── 测试5: 2D 沿 axis=1 (行最大值) ──\n";
        int data[] = {3, 7, 2, 9, 1, 4, 6, 8, 5};
        int expected[] = {7, 9, 8};
        constexpr size_t rows = 3, cols = 3;
        int result[rows];

        printMat("输入矩阵", data, rows, cols);
        reduceMaxAxis1(data, result, rows, cols);
        printVec("行最大值", result, rows);
        testCase("行最大值", result, expected, rows);
    }

    // ── 测试6: 维度变体 ─────────────────────────────────
    {
        std::cout << "\n── 测试6: 维度变体 ──\n";
        // 测试2×4 matrix
        int data[] = {1, 3, 5, 7, 2, 4, 6, 8};  // shape (2, 4)
        constexpr size_t rows = 2, cols = 4;

        printMat("输入矩阵 (2×4)", data, rows, cols);

        int result0[cols];
        reduceSumAxis0(data, result0, rows, cols);
        int expected0[] = {3, 7, 11, 15};
        printVec("axis=0 (列和)", result0, cols);
        testCase("2×4 axis=0", result0, expected0, cols);

        int result1[rows];
        reduceSumAxis1(data, result1, rows, cols);
        int expected1[] = {16, 20};
        printVec("axis=1 (行和)", result1, rows);
        testCase("2×4 axis=1", result1, expected1, rows);
    }

    std::cout << "\n── 全部测试完成 ──\n";
    return 0;
}
