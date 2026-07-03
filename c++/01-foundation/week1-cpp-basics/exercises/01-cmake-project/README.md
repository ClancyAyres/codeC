# 习题 01：搭建 CMake 项目

## 目标

搭建一个包含静态库和可执行文件的 CMake 项目。

## 要求

项目结构：
```
project/
├── CMakeLists.txt          # 顶层 CMake（设置 C++17 标准，添加子目录）
├── libmath/
│   ├── CMakeLists.txt      # 定义静态库 math
│   ├── math.hpp            # 声明 add() 和 multiply() 函数
│   └── math.cpp            # 实现 add() 和 multiply()
└── app/
    ├── CMakeLists.txt      # 定义可执行文件 app，链接 libmath
    └── main.cpp            # 调用 math::add() 和 math::multiply()
```

## 具体步骤

1. 创建以上目录结构
2. 编写 `math.hpp`（声明两个函数）
3. 编写 `math.cpp`（实现两个函数）
4. 编写 `app/main.cpp`（调用函数并打印结果）
5. 编写各层 `CMakeLists.txt`
6. 构建并运行：
   ```bash
   mkdir build && cd build
   cmake ..
   cmake --build .
   ./app/app
   ```

## 提示

- 使用 `set(CMAKE_CXX_STANDARD 17)` 设置 C++ 标准
- `add_library(math STATIC math.cpp)` 创建静态库
- `target_link_libraries(app PRIVATE math)` 链接库
- `target_include_directories(math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})` 让使用者能找到头文件
