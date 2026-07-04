// 模块 1 示例: 引用 vs 指针
// 相关规范: Core Guidelines F.7, F.15, F.22; Google Style: Reference Arguments
//
// Java 开发者核心困惑: Java 中所有对象变量都是引用(类似 C++ 指针)，
// C++ 中对象变量默认是值，需要用 & (引用) 或 * (指针) 来间接访问。

#include <iostream>
#include <string>

// ✅ 正确: 只读参数用 const 引用 — Core Guideline F.15
void printName(const std::string& name) {
    std::cout << "Hello, " << name << '\n';
}

// ✅ 正确: 输出参数用指针 — Google Style: 可变的用指针
void parseName(const std::string& input, std::string* outFirst, std::string* outLast) {
    auto pos = input.find(' ');
    if (pos != std::string::npos) {
        if (outFirst) *outFirst = input.substr(0, pos);
        if (outLast) *outLast = input.substr(pos + 1);
    }
}

int main() {
    std::string name = "Clancy";

    // 引用: 对象的别名，不能为空，不能重新绑定
    std::string& ref = name;
    ref = "C++ Dev";

    // 指针: 可以为空，可以重新指向
    std::string* ptr = &name;
    *ptr = "Pointer";
    ptr = nullptr;

    printName(name);

    std::string first, last;
    parseName("John Doe", &first, &last);
    std::cout << "First: " << first << ", Last: " << last << '\n';

    return 0;
}
