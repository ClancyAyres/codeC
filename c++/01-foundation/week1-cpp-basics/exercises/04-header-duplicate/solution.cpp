// solution.cpp — 参考答案
//
// 完整项目文件如下：

// ===== config.hpp =====
#if 0
#pragma once  // ← 加上此行修复重复包含问题

const int VERSION = 1;
const char* APP_NAME = "C++ Learning";
#endif

// ===== math.hpp =====
#if 0
#pragma once

#include "config.hpp"

int add(int a, int b);
#endif

// ===== math.cpp =====
#if 0
#include "math.hpp"

int add(int a, int b) {
    return a + b;
}
#endif

// ===== string_utils.hpp =====
#if 0
#pragma once

#include "config.hpp"
#include <string>

std::string to_upper(std::string s);
#endif

// ===== string_utils.cpp =====
#if 0
#include "string_utils.hpp"
#include <cctype>

std::string to_upper(std::string s) {
    for (char& c : s) {
        if (c >= 'a' && c <= 'z') {
            c = c - 'a' + 'A';
        }
    }
    return s;
}
#endif

// ===== main.cpp =====
#if 0
// 同时包含两个头文件，它们都间接包含 config.hpp
// 因为有 #pragma once，config.hpp 只被处理一次
#include "math.hpp"
#include "string_utils.hpp"
#include <iostream>

int main() {
    std::cout << "应用: " << APP_NAME << " v" << VERSION << '\n';
    std::cout << "add(3, 5) = " << add(3, 5) << '\n';
    std::cout << "to_upper(\"hello\") = " << to_upper("hello") << '\n';
    return 0;
}
#endif

// 说明：
// 1. 每个 .hpp 文件都加了 #pragma once，这是头文件守卫的标准做法
// 2. math.hpp 和 string_utils.hpp 都包含 config.hpp
// 3. main.cpp 同时包含 math.hpp 和 string_utils.hpp
// 4. 没有 #pragma once 时，config.hpp 的内容会被处理两次
//    → VERSION 和 APP_NAME 被定义两次 → 编译错误
// 5. 加上 #pragma once 后，第二次遇到 #include "config.hpp" 会被跳过
//
// 编译命令（假设文件在同一目录）：
//   g++ -std=c++17 main.cpp math.cpp string_utils.cpp -o test
