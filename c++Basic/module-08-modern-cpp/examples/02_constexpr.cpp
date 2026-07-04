// 模块 7 示例: constexpr 编译期计算
// Java 完全没有的概念 — 函数可以在编译时求值

#include <iostream>
#include <array>

// ✅ constexpr 函数: 编译期求值
constexpr int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// ✅ constexpr 类: 可以在编译期使用
class Point {
public:
    constexpr Point(double x, double y) : x_(x), y_(y) {}
    constexpr double x() const { return x_; }
    constexpr double y() const { return y_; }
    constexpr double distanceSq() const { return x_ * x_ + y_ * y_; }

private:
    double x_, y_;
};

// ✅ if constexpr (C++17): 编译期分支
template<typename T>
auto getValue(T t) {
    if constexpr (std::is_pointer_v<T>) {
        return *t;  // 只对指针类型编译这个分支
    } else {
        return t;   // 只对非指针类型编译这个分支
    }
}

int main() {
    // ✅ 正确: constexpr 变量由编译期函数求值，零运行时开销 — Core Guidelines Per.1
    constexpr int fact5 = factorial(5);
    // ✅ 正确: constexpr 函数作模板参数，数组大小编译期确定 — Core Guidelines Per.1
    std::array<int, factorial(5)> arr{};
    std::cout << "5! = " << fact5 << " (array size: " << arr.size() << ")\n";

    // ✅ 正确: constexpr 对象 + constexpr 方法，编译期计算 distanceSq — Core Guidelines Per.1
    constexpr Point p(3.0, 4.0);
    constexpr double distSq = p.distanceSq();
    std::cout << "Point(" << p.x() << "," << p.y() << ") dist² = " << distSq << '\n';

    // ✅ 正确: if constexpr 在编译期选择分支，不匹配的代码不被实例化 — Core Guidelines Per.1
    int value = 42;
    int* ptr = &value;
    std::cout << "getValue(value): " << getValue(value) << '\n';  // 42
    std::cout << "getValue(ptr): " << getValue(ptr) << '\n';      // 42

    return 0;
}
