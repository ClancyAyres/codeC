// Exercise 02: 多维归约 — 沿指定轴求和
// TODO: 实现 reduceSum1D (一维归约)
// TODO: 实现 reduceSum2D (二维归约，沿任意轴)
// TODO: 实现 reduceMax2D (二维求最大值，沿指定轴)
// TODO: 在 main() 中测试各种归约操作

#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <limits>

// ── 1. 一维归约 (TODO: 完成) ────────────────────────────
template<typename T>
T reduceSum1D(const T* /* data */, size_t /* n */) {
    // TODO: 求和 data[0..n-1]，返回结果
    T sum = T(0);
    return sum;
}

// ── 2. 二维归约沿 axis=0 (列归约) (TODO: 完成) ──────────
// 输入 shape (rows, cols)，axis=0 归约 → 输出 shape (cols)
// output[c] = sum(input[r*cols + c] for r in 0..rows-1)
template<typename T>
void reduceSumAxis0(const T* /* input */, T* /* output */,
                    size_t /* rows */, size_t /* cols */) {
    // TODO: 每列求和
}

// ── 3. 二维归约沿 axis=1 (行归约) (TODO: 完成) ──────────
// 输入 shape (rows, cols)，axis=1 归约 → 输出 shape (rows)
// output[r] = sum(input[r*cols + c] for c in 0..cols-1)
template<typename T>
void reduceSumAxis1(const T* /* input */, T* /* output */,
                    size_t /* rows */, size_t /* cols */) {
    // TODO: 每行求和
}

// ── 4. 二维归约（通用版，沿任意轴）(TODO: 完成) ─────────
// axis=0 → 归约行 (保留列)，输出 shape (cols)
// axis=1 → 归约列 (保留行)，输出 shape (rows)
template<typename T>
void reduceSum2D(const T* input, T* output,
                 size_t rows, size_t cols, size_t axis) {
    if (axis == 0) {
        reduceSumAxis0(input, output, rows, cols);
    } else if (axis == 1) {
        reduceSumAxis1(input, output, rows, cols);
    }
}

// ── 5. 二维求最大值沿 axis=1 (TODO: 完成) ───────────────
template<typename T>
void reduceMaxAxis1(const T* /* input */, T* /* output */,
                    size_t /* rows */, size_t /* cols */) {
    // TODO: 每行求最大值
    // 提示: 初始化为 std::numeric_limits<T>::lowest()
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

// ── main ────────────────────────────────────────────────

int main() {
    std::cout << "========== 习题02: 多维归约 ==========\n\n";

    // ── 测试1: 一维归约 ─────────────────────────────────
    {
        std::cout << "── 测试1: 一维求和 ──\n";
        int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        constexpr size_t n = sizeof(data) / sizeof(data[0]);
        // TODO: 调用 reduceSum1D，期望 55
        std::cout << "  TODO: 实现 reduceSum1D 后测试\n";
    }

    // ── 测试2: 二维沿 axis=0 (列求和) ───────────────────
    {
        std::cout << "\n── 测试2: 2D 沿 axis=0 (列求和) ──\n";
        int data[] = {
            1, 2, 3,
            4, 5, 6,
            7, 8, 9
        };
        // 列和: [1+4+7=12, 2+5+8=15, 3+6+9=18]
        int expected[] = {12, 15, 18};
        constexpr size_t rows = 3, cols = 3;
        int result[cols];

        // TODO: 调用 reduceSumAxis0
        std::cout << "  TODO: 实现 reduceSumAxis0 后测试\n";
    }

    // ── 测试3: 二维沿 axis=1 (行求和) ───────────────────
    {
        std::cout << "\n── 测试3: 2D 沿 axis=1 (行求和) ──\n";
        int data[] = {
            1, 2, 3,
            4, 5, 6,
            7, 8, 9
        };
        // 行和: [6, 15, 24]
        int expected[] = {6, 15, 24};
        constexpr size_t rows = 3, cols = 3;
        int result[rows];

        // TODO: 调用 reduceSumAxis1
        std::cout << "  TODO: 实现 reduceSumAxis1 后测试\n";
    }

    // ── 测试4: 二维通用归约 ─────────────────────────────
    {
        std::cout << "\n── 测试4: 通用 reduceSum2D ──\n";
        int data[] = {1, 2, 3, 4, 5, 6};  // shape (2, 3)
        constexpr size_t rows = 2, cols = 3;

        int result0[cols], result1[rows];
        int expected0[] = {5, 7, 9};   // axis=0: 列和
        int expected1[] = {6, 15};     // axis=1: 行和

        // TODO: 调用 reduceSum2D
        std::cout << "  TODO: 实现 reduceSum2D 后测试\n";
    }

    // ── 测试5: 最大值归约 ───────────────────────────────
    {
        std::cout << "\n── 测试5: 2D 沿 axis=1 (行最大值) ──\n";
        int data[] = {3, 7, 2, 9, 1, 4, 6, 8, 5};
        int expected[] = {7, 9, 8};  // 每行最大值
        constexpr size_t rows = 3, cols = 3;
        int result[rows];

        // TODO: 调用 reduceMaxAxis1
        std::cout << "  TODO: 实现 reduceMaxAxis1 后测试\n";
    }

    std::cout << "\n提示：\n";
    std::cout << "  - axis=0: 沿行归约 → 输出大小 = cols\n";
    std::cout << "  - axis=1: 沿列归约 → 输出大小 = rows\n";
    std::cout << "  - 求和初始化为 T(0)\n";
    std::cout << "  - 求最大值初始化为 std::numeric_limits<T>::lowest()\n";

    return 0;
}
