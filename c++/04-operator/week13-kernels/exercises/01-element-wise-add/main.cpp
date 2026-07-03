// Exercise 01: 模板化逐元素加法
// TODO: 完善 elementWiseAdd 模板函数，支持 float 和 int 两种类型
// TODO: 实现 scalarMultiply（标量广播乘法）
// TODO: 实现 inPlaceRelu（原地 ReLU）
// TODO: 在 main() 中调用测试并验证结果

#include <iostream>
#include <vector>
#include <functional>
#include <iomanip>
#include <cstring>
#include <cmath>

// ── 函数模板1：逐元素加法 (TODO: 完成) ──────────────────

// TODO: 实现逐元素加法
// c[i] = a[i] + b[i]  for i in [0, n)
template<typename T>
void elementWiseAdd(const T* /* a */, const T* /* b */, T* /* c */, size_t /* n */) {
    // TODO: 填写循环实现
}

// ── 函数模板2：标量广播乘法 (TODO: 完成) ─────────────────
// TODO: 实现标量广播乘法
// out[i] = in[i] * scalar  for i in [0, n)
template<typename T>
void scalarMultiply(const T* /* in */, T /* scalar */, T* /* out */, size_t /* n */) {
    // TODO: 填写循环实现
}

// ── 函数模板3：原地 ReLU (TODO: 完成) ────────────────────
// TODO: 实现原地 ReLU
// data[i] = max(data[i], 0)  for i in [0, n)
template<typename T>
void inPlaceRelu(T* /* data */, size_t /* n */) {
    // TODO: 填写循环实现
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
        if (a[i] != b[i]) return false;
    return true;
}

// ── main ────────────────────────────────────────────────

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

        // TODO: 调用 elementWiseAdd
        // elementWiseAdd(a, b, c, n);
        // TODO: 验证结果
        std::cout << "  TODO: 实现 elementWiseAdd 后测试\n";
    }

    // ── 测试2: int 加法 ─────────────────────────────────
    {
        std::cout << "\n── 测试2: int 加法 ──\n";
        int a[] = {1, 2, 3, 4, 5};
        int b[] = {10, 20, 30, 40, 50};
        int expected[] = {11, 22, 33, 44, 55};
        constexpr size_t n = sizeof(a) / sizeof(a[0]);
        int c[n];

        // TODO: 调用 elementWiseAdd<int>
        std::cout << "  TODO: 实现并测试 int 版本\n";
    }

    // ── 测试3: 标量广播乘法 ─────────────────────────────
    {
        std::cout << "\n── 测试3: 标量广播乘法 ──\n";
        double in[] = {1.0, 2.0, 3.0, 4.0, 5.0};
        double scalar = 2.5;
        double expected[] = {2.5, 5.0, 7.5, 10.0, 12.5};
        constexpr size_t n = sizeof(in) / sizeof(in[0]);
        double out[n];

        // TODO: 调用 scalarMultiply
        std::cout << "  TODO: 实现 scalarMultiply 后测试\n";
    }

    // ── 测试4: 原地 ReLU ────────────────────────────────
    {
        std::cout << "\n── 测试4: 原地 ReLU ──\n";
        float data[] = {-3.0f, -1.0f, 0.0f, 2.0f, 5.0f, -0.5f};
        float expected[] = {0.0f, 0.0f, 0.0f, 2.0f, 5.0f, 0.0f};
        constexpr size_t n = sizeof(data) / sizeof(data[0]);

        // TODO: 调用 inPlaceRelu
        std::cout << "  TODO: 实现 inPlaceRelu 后测试\n";
    }

    std::cout << "\n提示：\n";
    std::cout << "  - 模板函数用 template<typename T> 声明\n";
    std::cout << "  - 使用数组指针还是 vector 都可以\n";
    std::cout << "  - 确保 float 和 int 版本都正确工作\n";
    std::cout << "  - ReLU(x) = max(0, x)，原地修改输入数据\n";

    return 0;
}
