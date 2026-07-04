# 模块 1: C++ 前置基础 — 写给 Java 开发者

本模块专为 Java 开发者补充 C++ 独有的基础概念，这些概念在 Java 中不存在或完全不同。

## 知识点: 编译模型 — .h 与 .cpp

C++ 使用"声明与定义分离"的编译模型，Java 没有这个概念。

| 概念 | Java | C++ |
|------|------|-----|
| 文件单元 | `.java` 文件（声明即定义） | `.h` 声明 + `.cpp` 定义 |
| 依赖引入 | `import java.util.List;` | `#include <vector>` |
| 声明和定义 | 不区分 | ⚠️ 必须区分，否则链接报错 |
| 编译单元 | 每个 `.java` 独立编译 | 每个 `.cpp` 独立编译，最后链接 |
| 符号重复 | 编译期报错 | ⚠️ 链接期报 "multiple definition" |

**头文件 (.h/.hpp):** 放类声明、函数声明、模板定义
**源文件 (.cpp):** 放函数实现、静态变量定义

```cpp
// myclass.h — 声明
#pragma once  // ✅ 防止重复包含（代替 #ifndef 守卫）
class MyClass {
public:
    void doSomething();  // 只声明
};

// myclass.cpp — 定义
#include "myclass.h"
void MyClass::doSomething() {  // 定义
    // 实现
}
```

### 相关编码规范

| 规范来源 | 编号 | 规则 |
|----------|------|------|
| C++ Core Guidelines | SF.1 | 如果项目没有约定，用 `.cpp` 作为源文件后缀，`.h` 作为头文件后缀 |
| C++ Core Guidelines | SF.8 | 所有头文件用 `#pragma once` 或 include guard |
| Google Style Guide | - | 头文件应该是 self-contained（以 #pragma once 开头） |

## 知识点: 预处理器

C++ 编译前会先运行预处理器。Java 没有这个阶段。

| 指令 | 作用 | 说明 |
|------|------|------|
| `#include <file>` | 插入系统头文件 | 从系统路径搜索 |
| `#include "file"` | 插入项目头文件 | 从当前目录搜索 |
| `#pragma once` | 头文件只包含一次 | ✅ 现代推荐做法 |
| `#define NAME value` | 定义宏 | ⚠️ 优先用 const/constexpr 替代 |
| `#ifdef / #ifndef / #endif` | 条件编译 | 用于平台相关代码 |

```cpp
// ❌ 旧式 include guard（不推荐）
#ifndef MYCLASS_H
#define MYCLASS_H
// ...
#endif

// ✅ 现代写法
#pragma once
```

## 知识点: 基本类型与大小

| 类型 | C++ 大小 | Java 大小 | 说明 |
|------|----------|-----------|------|
| `bool` | 1 byte (通常) | JVM 依赖 | ⚠️ 不等于 Java boolean |
| `char` | 1 byte | 2 bytes (UTF-16) | ⚠️ 完全不同！C++ char 是单字节 |
| `short` | ≥2 bytes | 2 bytes | 固定 |
| `int` | ≥2 bytes，通常 4 | 4 bytes | ⚠️ C++ 平台相关 |
| `long` | ≥4 bytes | 8 bytes | ⚠️ 大小不同！Linux 64位 = 8，Win64 = 4 |
| `long long` | ≥8 bytes | 无 | C++ 独有 |
| `float` | 4 bytes | 4 bytes | 同 |
| `double` | 8 bytes | 8 bytes | 同 |
| `size_t` | 平台相关 | 无 | 无符号整数，sizeof 的返回类型 |
| `std::string` | 动态 | `String` (不可变) | ⚠️ C++ string 是可变的 |

```cpp
#include <cstddef>   // size_t
#include <cstdint>   // int32_t, int64_t (推荐，大小固定)
int32_t fixed = 42;  // ✅ 确定是 32 位
```

### 相关编码规范

| 规范来源 | 编号 | 规则 |
|----------|------|------|
| C++ Core Guidelines | SL.1 | 尽可能使用标准库 |
| Google Style Guide | - | 需要固定大小整数时用 `<cstdint>` (int32_t 等)，否则用 int |

## 知识点: 初始化语法

C++ 有多种初始化方式，各自有细微差异。Java 只有 `=` 赋值一种。

| 语法 | 示例 | 说明 |
|------|------|------|
| 拷贝初始化 | `int x = 5;` | 最像 Java |
| 直接初始化 | `int x(5);` | 调用构造函数 |
| 列表初始化 (推荐) | `int x{5};` | ⚠️ 不允许窄化转换（安全） |
| 值初始化 | `int x{};` | 初始化为 0 |
| 默认初始化 | `int x;` | ⚠️ 局部变量不初始化 → 未定义行为！ |

```cpp
int a = 5;     // ✅ OK
int b(5);      // ✅ OK
int c{5};      // ✅ 推荐: 不允许窄化
int d{3.14};   // ❌ 编译错误: double→int 是窄化
int e = 3.14;  // ⚠️ 编译通过但有警告，e = 3
int f{};       // ✅ f = 0
int g;         // ⚠️ 局部变量未初始化，值是垃圾值！
```

### 相关编码规范

| 规范来源 | 编号 | 规则 |
|----------|------|------|
| C++ Core Guidelines | ES.23 | 优先用 {} 初始化 |
| C++ Core Guidelines | ES.20 | 总是初始化对象 |
| Google Style Guide | - | 基本类型用 `=` 初始化，类用 `{}` |

## 知识点: static 的三种含义

Java 的 `static` 只有一种含义（类级别）。C++ 有三种完全不同场景：

| 场景 | 含义 | 示例 |
|------|------|------|
| 文件作用域 | 符号仅当前 .cpp 可见（内部链接） | `static int count = 0;` |
| 函数内 | 变量在多次调用间保持值 | `static int calls = 0;` |
| 类内 | 所有实例共享（同 Java） | `static int instanceCount;` |

```cpp
// 1. 文件作用域 static — 类似 Java 的 package-private（但不完全一样）
static int fileScopedVar = 0;  // 其他 .cpp 文件不可见

// 2. 函数内 static — Java 没有这个用法
int getNextId() {
    static int id = 0;  // 只在第一次调用时初始化
    return ++id;
}

// 3. 类内 static — 和 Java 一样
class Config {
    static int defaultPort;  // 所有实例共享
};
int Config::defaultPort = 8080;  // ⚠️ 必须在 .cpp 中定义
```

## 知识点: enum class vs enum

| 特性 | `enum` (C 风格) | `enum class` (C++11) |
|------|-----------------|---------------------|
| 作用域 | 全局，易冲突 | 枚举值在类作用域内 |
| 隐式转 int | ✅ 可以 | ❌ 禁止（需显式转换） |
| 类型安全 | ❌ 弱 | ✅ 强类型 |
| 推荐 | ❌ | ✅ |

```cpp
// ❌ C 风格 — 枚举值泄漏到全局
enum Color { RED, GREEN, BLUE };
int x = RED;  // 隐式转换, x = 0

// ✅ C++11 — 安全
enum class Color { Red, Green, Blue };
Color c = Color::Red;
// int y = c;  // ❌ 编译错误，不能隐式转换
int y = static_cast<int>(c);  // ✅ 显式转换
```

## 知识点: 类型转换

| C++ 转换 | 用途 | 风险 |
|----------|------|------|
| `static_cast<T>(x)` | 相关类型转换（int→double, 基类→派生类） | 编译期检查，无运行时开销 |
| `dynamic_cast<T>(x)` | 多态类型安全下行转换 | 运行时检查，失败返回 nullptr（指针）或抛异常（引用） |
| `const_cast<T>(x)` | 去除 const | ⚠️ 极度危险，仅用于兼容旧 API |
| `reinterpret_cast<T>(x)` | 位模式重解释 | ⚠️ 极度危险，几乎永远不应该用 |
| `(T)x` (C 风格) | 上面所有 | ⚠️ 不推荐，不如显式指定 |

```cpp
double d = 3.14;
int i = static_cast<int>(d);  // ✅ 明确的窄化转换

// 多态下行转换
Base* base = new Derived();
Derived* derived = dynamic_cast<Derived*>(base);
if (derived) {
    // 安全使用
}
```

## 知识点: C 数组 vs std::array vs std::vector

| 特性 | `int arr[N]` | `std::array<int,N>` | `std::vector<int>` |
|------|-------------|---------------------|-------------------|
| 大小 | 编译期固定 | 编译期固定 | 运行时可变 |
| 边界检查 | ❌ 无 | `.at()` 有, `[]` 无 | `.at()` 有, `[]` 无 |
| 退化为指针 | ⚠️ 是（传给函数时丢失大小） | ✅ 否 | ✅ 否 |
| 知道自身大小 | ❌ | ✅ `.size()` | ✅ `.size()` |
| 推荐 | ❌ | ✅ 固定大小 | ✅ 动态大小 |

```cpp
// ❌ C 风格
int arr[5] = {1, 2, 3, 4, 5};
int size = sizeof(arr) / sizeof(arr[0]);  // 笨办法

// ✅ std::array (固定大小)
std::array<int, 5> arr = {1, 2, 3, 4, 5};
std::cout << arr.size();  // 5

// ✅ std::vector (动态大小) — 最常用
std::vector<int> vec = {1, 2, 3};
vec.push_back(4);
```

## 知识点: main 函数签名

```cpp
// ✅ 标准形式 1: 无命令行参数
int main() { return 0; }

// ✅ 标准形式 2: 带命令行参数
int main(int argc, char* argv[]) {
    // argc: 参数个数（含程序名）
    // argv[0]: 程序名, argv[1..]: 参数
    return 0;
}

// ⚠️ main 函数 return 0 表示成功，非零表示失败
// 不写 return 时，main 隐式 return 0 (C++ 特有)
```

## 常见 Bug（Code Review 回退清单）

- [ ] `.h` 文件忘了 `#pragma once` — 可能重复定义
- [ ] 局部变量未初始化就使用 — 未定义行为（Java 编译器会阻止）
- [ ] `long` 类型在不同平台大小不同 — 跨平台 bug
- [ ] C 风格枚举值全局污染 — 用 `enum class`
- [ ] C 风格类型转换 `(T)x` — 用 `static_cast` 等
- [ ] C 数组传给函数后丢失大小信息 — 用 `std::array`/`std::vector`
- [ ] 头文件中写 `using namespace std;` — 污染所有包含该头文件的文件
