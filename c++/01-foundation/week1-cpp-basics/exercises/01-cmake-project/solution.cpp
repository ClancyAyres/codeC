// solution.cpp — 参考答案
//
// 完整项目文件结构如下（需分别创建每个文件）：

// ===== CMakeLists.txt (顶层) =====
#if 0
cmake_minimum_required(VERSION 3.20)
project(cmake-exercise LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(libmath)
add_subdirectory(app)
#endif

// ===== libmath/math.hpp =====
#if 0
#pragma once

namespace math {
    int add(int a, int b);
    int multiply(int a, int b);
}
#endif

// ===== libmath/math.cpp =====
#if 0
#include "math.hpp"

namespace math {
    int add(int a, int b) {
        return a + b;
    }

    int multiply(int a, int b) {
        return a * b;
    }
}
#endif

// ===== libmath/CMakeLists.txt =====
#if 0
add_library(math STATIC math.cpp)
target_include_directories(math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#endif

// ===== app/main.cpp =====
#if 0
#include "math.hpp"
#include <iostream>

int main() {
    std::cout << "10 + 20 = " << math::add(10, 20) << '\n';
    std::cout << "10 * 20 = " << math::multiply(10, 20) << '\n';
    return 0;
}
#endif

// ===== app/CMakeLists.txt =====
#if 0
add_executable(app main.cpp)
target_link_libraries(app PRIVATE math)
#endif

// 说明：
// 1. 顶层 CMakeLists.txt 设置全局 C++ 标准为 17，并通过 add_subdirectory 管理子项目
// 2. libmath 被定义为 STATIC 静态库，PUBLIC 头文件目录让使用者自动获得 include 路径
// 3. app 通过 PRIVATE 链接 libmath（意味着 math 的符号只在 app 内部使用）
// 4. 使用 C++17 嵌套命名空间语法 math::add 而非 ::math::add
