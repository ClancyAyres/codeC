// 模块 1 示例: 头文件与源文件分离
// 相关规范: Core Guidelines SF.1, SF.8; Google Style: Header Files
//
// 本文件模拟了 .h 和 .cpp 的分离模式。
// 在实际项目中，注释中标记为 [.h] 的内容应放在 MyMath.h 中，
// [.cpp] 的内容应放在 MyMath.cpp 中。
// 这里合并为一个文件方便编译运行。

#include <iostream>
#include <string>

// ===== [.h 文件内容] =====
// MyMath.h — 只放声明
class MyMath {
public:
    // 简单函数可以在头文件中定义（隐式 inline）
    static int add(int a, int b) { return a + b; }

    // 复杂函数只在头文件声明，在 .cpp 定义
    static int factorial(int n);
};

// ===== [.cpp 文件内容] =====
// MyMath.cpp — 只放定义
// #include "MyMath.h"

// ✅ 复杂实现放在 .cpp 中 — Core Guideline SF.1
int MyMath::factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}
// ⚠️ 如果 factorial 的实现放在 .h 中，每个包含 .h 的 .cpp 都会生成一份副本

int main() {
    std::cout << "add(3, 4) = " << MyMath::add(3, 4) << '\n';
    std::cout << "factorial(5) = " << MyMath::factorial(5) << '\n';
    return 0;
}
