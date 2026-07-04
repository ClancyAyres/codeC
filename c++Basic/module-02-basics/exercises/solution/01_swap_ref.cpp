// 模块 1 习题答案: swap 与引用
// 规范: Core Guidelines F.15, F.17; Google Style: Reference Arguments

#include <iostream>
#include <string>

void swapByValue(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
}

void swapByRef(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

void appendSuffix(std::string& str, const std::string& suffix) {
    str += suffix;
}

int main() {
    int x1 = 10, y1 = 20;
    std::cout << "Before swapByValue: x1=" << x1 << ", y1=" << y1 << '\n';
    swapByValue(x1, y1);
    std::cout << "After  swapByValue: x1=" << x1 << ", y1=" << y1 << '\n';
    std::cout << "(如果 x1 y1 没变，说明按值传递不改变外部变量)\n\n";

    int x2 = 100, y2 = 200;
    std::cout << "Before swapByRef: x2=" << x2 << ", y2=" << y2 << '\n';
    swapByRef(x2, y2);
    std::cout << "After  swapByRef: x2=" << x2 << ", y2=" << y2 << '\n';

    std::string filename = "report";
    appendSuffix(filename, ".pdf");
    std::cout << "Filename: " << filename << '\n';

    return 0;
}
