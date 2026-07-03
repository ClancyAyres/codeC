// 故意 include 两次 math.hpp，验证头文件守卫生效
// 如果没有 #pragma once 或 #ifndef 守卫，以下代码将导致重复定义错误
#include "math.hpp"
#include "math.hpp"  // 第二次 include，守卫阻止重复处理

#include <iostream>

int main() {
    std::cout << "头文件守卫演示\n";
    std::cout << "math.hpp 被包含了两次，但因为 #pragma once 守卫，\n";
    std::cout << "其内容只被处理一次，不会产生重复定义错误。\n\n";

    std::cout << "calc::add(3, 4) = " << calc::add(3, 4) << '\n';
    std::cout << "calc::multiply(3, 4) = " << calc::multiply(3, 4) << '\n';

    return 0;
}
