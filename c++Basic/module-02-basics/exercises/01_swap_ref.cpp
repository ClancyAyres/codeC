// 模块 1 习题: swap 与引用
// 请在标注 TODO 的位置实现函数

#include <iostream>
#include <string>

// TODO: 实现 swapByValue — 这个版本交换函数内部的值，但不影响调用者
// void swapByValue( ??? ) { ... }

// TODO: 实现 swapByRef — 这个版本要真正交换两个外部变量
// void swapByRef( ??? ) { ... }

// TODO: 实现 appendSuffix — 在 str 后面追加 suffix
// void appendSuffix( ??? ) { ... }

int main() {
    // 测试 swapByValue
    int x1 = 10, y1 = 20;
    std::cout << "Before swapByValue: x1=" << x1 << ", y1=" << y1 << '\n';
    // TODO: 调用 swapByValue 并输出结果
    std::cout << "After  swapByValue: x1=" << x1 << ", y1=" << y1 << '\n';
    std::cout << "(如果 x1 y1 没变，说明按值传递不改变外部变量)\n\n";

    // 测试 swapByRef
    int x2 = 100, y2 = 200;
    std::cout << "Before swapByRef: x2=" << x2 << ", y2=" << y2 << '\n';
    // TODO: 调用 swapByRef 并输出结果
    std::cout << "After  swapByRef: x2=" << x2 << ", y2=" << y2 << '\n';

    // 测试 appendSuffix
    std::string filename = "report";
    // TODO: 调用 appendSuffix
    std::cout << "Filename: " << filename << '\n';

    return 0;
}
