// 模块 1 示例: 初始化语法
// 相关规范: Core Guidelines ES.20, ES.23; Google Style: Variable Initialization
//
// C++ 有多种初始化方式。Java 只有 = 赋值一种。

#include <iostream>
#include <string>
#include <vector>

void show(const std::string& label, int value) {
    std::cout << "  " << label << " = " << value << '\n';
}

int main() {
    std::cout << "=== 初始化方式对比 ===\n";

    // ✅ 拷贝初始化: 最像 Java
    int x1 = 42;
    show("int x = 42", x1);

    // ✅ 直接初始化: 调用构造函数风格
    int x2(42);
    show("int x(42)", x2);

    // ✅ 列表初始化: C++11 推荐，禁止窄化
    int x3{42};
    show("int x{42}", x3);

    // ❌ 窄化转换被阻止
    // int x4{3.14};  // 编译错误: double → int 窄化

    // ⚠️ 拷贝初始化允许窄化（警告但不报错）
    int x5 = 3.14;
    show("int x = 3.14 (窄化!)", x5);  // 输出 3

    // ✅ 值初始化: 初始化为零
    int x6{};
    show("int x{}", x6);

    // ⚠️ 未初始化的局部变量 — 值不确定！
    int x7;
    // show("int x (未初始化)", x7);  // 使用未初始化值 = 未定义行为

    // === 类对象初始化 ===
    std::vector<int> v1 = {1, 2, 3};   // 拷贝初始化
    std::vector<int> v2{4, 5, 6};      // 列表初始化 (推荐)
    std::vector<int> v3(3, 7);          // 直接初始化: 3 个元素，每个 = 7

    std::cout << "v1: ";
    for (const auto& x : v1) std::cout << x << ' ';
    std::cout << '\n';

    std::cout << "v3: ";
    for (const auto& x : v3) std::cout << x << ' ';
    std::cout << '\n';

    return 0;
}
