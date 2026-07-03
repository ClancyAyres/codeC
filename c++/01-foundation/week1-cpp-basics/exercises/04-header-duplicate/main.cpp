// TODO: 创建以下文件，并演示头文件守卫的作用

// ===== config.hpp（先不加守卫，编译观察错误） =====
// const int VERSION = 1;
// const char* APP_NAME = "C++ Learning";

// ===== math.hpp =====
// #include "config.hpp"
// int add(int a, int b);

// ===== string_utils.hpp =====
// #include "config.hpp"
// #include <string>
// std::string to_upper(std::string s);

// ===== main.cpp（当前文件） =====
// 同时包含 math.hpp 和 string_utils.hpp
// 如果 config.hpp 没有头文件守卫，VERSION 和 APP_NAME 会被定义两次 → 编译错误

// 第一步：先编译不加守卫的版本，观察重复定义错误
// 第二步：在 config.hpp 顶部加上 #pragma once，重新编译应该成功

#include <iostream>

int main() {
    // TODO: 完成上述所有文件的创建

    // TODO: 打印 VERSION 和 APP_NAME
    // TODO: 调用 add(3, 5) 并打印结果
    // TODO: 调用 to_upper("hello") 并打印结果

    std::cout << "请完成以上 TODO\n";
    return 0;
}
