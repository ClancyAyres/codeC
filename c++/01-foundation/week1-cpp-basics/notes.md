# Week 1 知识点笔记：从 Java 到 C++ 的第一步

## 一、CMake 构建系统

CMake 是 C++ 生态系统中最主流的构建系统生成器。它不直接构建代码，而是生成 Makefile（或 Ninja、Xcode 项目等），再由这些工具执行实际编译。

### 1.1 核心命令

| 命令 | 用途 |
|------|------|
| `cmake_minimum_required(VERSION 3.20)` | 指定最低 CMake 版本 |
| `project(Name LANGUAGES CXX)` | 定义项目名称和语言 |
| `add_executable(target main.cpp)` | 创建可执行文件目标 |
| `add_library(target STATIC src.cpp)` | 创建静态库（`.a` / `.lib`） |
| `add_library(target SHARED src.cpp)` | 创建动态库（`.so` / `.dylib`） |
| `target_include_directories(target PUBLIC dir/)` | 添加头文件搜索路径 |
| `target_link_libraries(target PUBLIC other)` | 链接库依赖 |
| `set(CMAKE_CXX_STANDARD 17)` | 设置 C++ 标准 |
| `add_subdirectory(dir)` | 添加子目录（递归处理其中的 CMakeLists.txt） |

### 1.2 最小 CMake 项目

```cmake
cmake_minimum_required(VERSION 3.20)
project(hello LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hello main.cpp)
```

```cpp
// main.cpp
#include <iostream>
int main() {
    std::cout << "Hello, CMake!\n";
    return 0;
}
```

```bash
# 构建命令（始终在 build/ 目录中构建，保持源码目录干净）
mkdir build && cd build
cmake ..                              # 配置
cmake --build .                       # 编译
./hello                               # 运行
```

**构建目录分离是 C++ 最佳实践**：`build/` 目录包含所有生成文件（`.o`、Makefile、可执行文件），可随时删除，不影响源码。

### 1.3 库与可执行文件的分离

```
project/
├── CMakeLists.txt          # 顶层
├── libmath/
│   ├── CMakeLists.txt      # add_library(math math.cpp)
│   ├── math.hpp
│   └── math.cpp
└── app/
    ├── CMakeLists.txt      # add_executable(app main.cpp)
    └── main.cpp            # target_link_libraries(app PRIVATE math)
```

顶层 `CMakeLists.txt`：
```cmake
add_subdirectory(libmath)
add_subdirectory(app)
```

## 二、头文件与源文件分离

### 2.1 基本模式

```
math.hpp    ← 声明（接口）：函数签名、类定义
math.cpp    ← 实现（定义）：函数体
main.cpp    ← 使用者：只 include 头文件
```

```cpp
// math.hpp — 头文件
#pragma once

namespace calc {
    int add(int a, int b);
    int multiply(int a, int b);
}
```

```cpp
// math.cpp — 源文件
#include "math.hpp"

namespace calc {
    int add(int a, int b) {
        return a + b;
    }

    int multiply(int a, int b) {
        return a * b;
    }
}
```

```cpp
// main.cpp — 使用者
#include "math.hpp"
#include <iostream>

int main() {
    std::cout << calc::add(3, 4) << '\n';
    return 0;
}
```

### 2.2 为什么分离？

1. **编译速度**：修改 `.cpp` 只重新编译该文件，修改 `.hpp` 则所有包含它的文件都需重新编译
2. **封装**：使用者只看接口（`.hpp`），不需要知道实现细节
3. **分离编译**：每个 `.cpp` 独立编译为 `.o`，最后由链接器合并

## 三、编译链接模型

### 3.1 四阶段流程

```
源文件 (.cpp / .hpp)
    │
    ▼
┌──────────┐
│ 1. 预处理 │  处理 #include（文本替换）、#define（宏展开）、#ifdef（条件编译）
│ Preprocessor│  每个 .cpp 生成一个 .i 翻译单元
└──────────┘
    │
    ▼
┌──────────┐
│ 2. 编译   │  翻译单元 → 汇编代码 (.s)
│ Compiler  │  语法分析、语义分析、优化
└──────────┘
    │
    ▼
┌──────────┐
│ 3. 汇编   │  汇编代码 → 目标文件 (.o / .obj)
│ Assembler │  包含机器指令 + 符号表（未解析的外部引用）
└──────────┘
    │
    ▼
┌──────────┐
│ 4. 链接   │  多个 .o + 库 → 可执行文件
│ Linker    │  符号解析、地址重定位、合并段
└──────────┘
```

### 3.2 关键概念

- **翻译单元**（Translation Unit）：一个 `.cpp` 文件经过预处理后的结果，编译的基本单位
- **符号**（Symbol）：函数名、全局变量名等，链接器通过符号表解析引用
- **未定义引用**（Undefined Reference）：链接错误，通常意味着忘记链接某个 `.cpp` 或库
- **重复定义**（Multiple Definition）：同一个符号在多个 `.o` 中定义，通常意味着在头文件中定义了非内联函数

### 3.3 与 Java 的关键差异

```
Java:
  Main.java ──javac──▶ Main.class ──JVM──▶ 运行
  Utils.java ──javac──▶ Utils.class ─┘
  （javac 自动找到依赖的 .class）

C++:
  main.cpp ──g++──▶ main.o ──┐
  math.cpp ──g++──▶ math.o ──┤ ld ──▶ a.out
  （需要手动指定所有 .cpp 和链接的库）
```

## 四、基本类型

### 4.1 内置类型的大小保证

C++ 标准只保证**最小范围**，不保证确切大小：

| 类型 | 最小位数 | 典型大小（64 位系统） |
|------|---------|---------------------|
| `char` | 8 位 | 1 字节 |
| `short` | 16 位 | 2 字节 |
| `int` | 16 位 | 4 字节 |
| `long` | 32 位 | 8 字节（macOS/Linux），4 字节（Windows） |
| `long long` | 64 位 | 8 字节 |
| `float` | — | 4 字节（IEEE 754） |
| `double` | — | 8 字节（IEEE 754） |

### 4.2 定长类型（推荐）

```cpp
#include <cstdint>

int8_t    i8;     // 正好 8 位，有符号
uint8_t   u8;     // 正好 8 位，无符号
int16_t   i16;
uint16_t  u16;
int32_t   i32;
uint32_t  u32;
int64_t   i64;
uint64_t  u64;

// 最快的至少 N 位类型
int_fast32_t fast32;

// 最小的至少 N 位类型
int_least32_t least32;
```

### 4.3 `sizeof` 运算符

`sizeof` 是编译期运算符，返回类型或变量占用的字节数：

```cpp
sizeof(int)         // 返回 size_t
sizeof(variable)    // 变量的类型大小
sizeof(array)       // 整个数组的大小（非指针！）
```

### 4.4 类型范围

```cpp
#include <climits>   // INT_MAX, INT_MIN, UINT_MAX 等
#include <cfloat>    // FLT_MAX, DBL_MIN 等
#include <limits>    // std::numeric_limits<T>

std::numeric_limits<int>::max();      // int 最大值
std::numeric_limits<int>::min();      // int 最小值
std::numeric_limits<double>::epsilon(); // double 精度
```

## 五、命名空间

### 5.1 定义

```cpp
namespace math {
    int max(int a, int b) {
        return a > b ? a : b;
    }

    namespace advanced {   // 嵌套命名空间
        double sqrt(double x) { /* ... */ }
    }
}

// C++17 嵌套命名空间简写
namespace app::core::utils {
    void init();
}
```

### 5.2 使用方式

```cpp
// 方式 1：完全限定名（最明确）
int result = math::max(3, 5);

// 方式 2：using 声明（推荐，只引入需要的符号）
using math::max;
int result = max(3, 5);

// 方式 3：using 指令（引入整个命名空间）
using namespace std;  // 头文件中绝对禁止！
```

### 5.3 全局命名空间

```cpp
int max(int a, int b) { return a > b ? a : b; }  // 全局

namespace math {
    int max(int a, int b) { return a > b ? a : b; }
}

int main() {
    ::max(3, 5);       // 显式调用全局命名空间中的 max
    math::max(3, 5);   // 调用 math 命名空间中的 max
}
```

### 5.4 匿名命名空间

```cpp
namespace {
    // 仅在当前翻译单元可见（替代 static 的现代方式）
    void internalHelper() { /* ... */ }
}
```

## 六、头文件守卫

### 6.1 问题

如果 `a.hpp` 包含 `b.hpp`，`c.hpp` 也包含 `b.hpp`，当 `main.cpp` 同时包含 `a.hpp` 和 `c.hpp` 时，`b.hpp` 的内容会被处理两次，导致重复定义错误。

### 6.2 解决方案

```cpp
// 传统方式（可移植，支持所有编译器）
#ifndef MATH_HPP
#define MATH_HPP

// 头文件内容...

#endif  // MATH_HPP
```

```cpp
// 现代方式（更简洁，几乎所有编译器都支持）
#pragma once

// 头文件内容...
```

### 6.3 对比

| 特性 | `#ifndef` | `#pragma once` |
|------|-----------|----------------|
| 标准化 | C++ 标准的一部分 | 非标准但普遍支持 |
| 可移植性 | 100% 可移植 | 几乎所有现代编译器都支持 |
| 简洁性 | 需要 3 行 | 1 行 |
| 唯一性问题 | 需要确保宏名唯一 | 编译器自动处理 |
| 符号链接支持 | 手动处理 | 有些编译器对符号链接处理不同 |

**推荐**：两者都用——`#pragma once` 放在第一行（现代编译器），`#ifndef` 作为后备（但现代项目中通常只用 `#pragma once`）。

### 6.4 头文件不应包含的内容

- 函数定义（非 `inline`）：会导致多重定义
- 全局变量定义（非 `extern`）：同上
- `using namespace` 指令：会污染所有包含者的命名空间
