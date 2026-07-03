// TODO: 搭建 CMake 项目结构
// 你需要创建以下文件：
//
// CMakeLists.txt (顶层)
//   - 设置 cmake_minimum_required(VERSION 3.20)
//   - project(cmake-exercise LANGUAGES CXX)
//   - 设置 C++17 标准
//   - add_subdirectory(libmath)
//   - add_subdirectory(app)
//
// libmath/math.hpp
//   - 声明 math::add(int, int) 和 math::multiply(int, int)
//
// libmath/math.cpp
//   - 实现 add() 返回 a + b
//   - 实现 multiply() 返回 a * b
//
// libmath/CMakeLists.txt
//   - add_library(math STATIC math.cpp)
//   - target_include_directories(math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
//
// app/main.cpp (当前文件)
//   - 包含 math.hpp
//   - 调用 math::add() 和 math::multiply() 并打印结果
//
// app/CMakeLists.txt
//   - add_executable(app main.cpp)
//   - target_link_libraries(app PRIVATE math)
//
// 构建命令:
//   mkdir build && cd build
//   cmake ..
//   cmake --build .
//   ./app/app
//
// 注意：本习题需要创建完整的目录结构和所有文件。
// 仅修改当前 main.cpp 是不够的。

#include <iostream>

int main() {
    // TODO: 包含 math.hpp 头文件
    // TODO: 调用 math::add(10, 20) 并打印结果
    // TODO: 调用 math::multiply(10, 20) 并打印结果

    std::cout << "完成 TODO 后，此程序应输出:\n";
    std::cout << "10 + 20 = 30\n";
    std::cout << "10 * 20 = 200\n";

    return 0;
}
