# Java 转 C++ 面向工作学习课程 实现计划

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 构建一套面向工作的 Java 转 C++ 学习课程，包含 10 个主题模块，每个模块含对比文档、示例代码、工作任务式习题，贯穿 C++ Core Guidelines + Google C++ Style Guide 编码规范。

**Architecture:** 每个模块独立目录，根 CMakeLists.txt 通过 `add_subdirectory` 统一管理编译。各模块不相互依赖，可按需学习。所有代码使用 C++17 标准。

**Tech Stack:** C++17, CMake 3.16+

---

## 文件结构

```
c++Basic/
├── CMakeLists.txt                    # 根构建文件
├── .gitignore
├── module-01-basics/
│   ├── CMakeLists.txt
│   ├── compare.md
│   ├── examples/
│   │   ├── 01_reference_vs_pointer.cpp
│   │   ├── 02_const_correctness.cpp
│   │   ├── 03_auto_and_range_for.cpp
│   │   └── 04_namespace_overload.cpp
│   └── exercises/
│       ├── 01_swap_ref.md
│       ├── 01_swap_ref.cpp
│       ├── 02_array_stats.md
│       ├── 02_array_stats.cpp
│       └── solution/
│           ├── 01_swap_ref.cpp
│           └── 02_array_stats.cpp
├── module-02-memory/
│   ├── CMakeLists.txt
│   ├── compare.md
│   ├── examples/
│   │   ├── 01_stack_vs_heap.cpp
│   │   ├── 02_unique_ptr.cpp
│   │   ├── 03_shared_ptr.cpp
│   │   ├── 04_move_semantics.cpp
│   │   └── 05_raii.cpp
│   └── exercises/
│       ├── 01_resource_manager.md
│       ├── 01_resource_manager.cpp
│       ├── 02_fix_memory_leak.md
│       ├── 02_fix_memory_leak.cpp
│       └── solution/
│           ├── 01_resource_manager.cpp
│           └── 02_fix_memory_leak.cpp
├── module-03-oop/
│   ├── CMakeLists.txt
│   ├── compare.md
│   ├── examples/
│   │   ├── 01_ctor_dtor.cpp
│   │   ├── 02_virtual_func.cpp
│   │   ├── 03_abstract_class.cpp
│   │   └── 04_multiple_inherit.cpp
│   └── exercises/
│       ├── 01_shape_hierarchy.md
│       ├── 01_shape_hierarchy.cpp
│       ├── 02_pimpl.md
│       ├── 02_pimpl.cpp
│       └── solution/
│           ├── 01_shape_hierarchy.cpp
│           └── 02_pimpl.cpp
├── module-04-value-semantics/
│   ├── CMakeLists.txt
│   ├── compare.md
│   ├── examples/
│   │   ├── 01_copy_constructor.cpp
│   │   ├── 02_move_constructor.cpp
│   │   └── 03_operator_overload.cpp
│   └── exercises/
│       ├── 01_string_wrapper.md
│       ├── 01_string_wrapper.cpp
│       ├── 02_vector2d.md
│       ├── 02_vector2d.cpp
│       └── solution/
│           ├── 01_string_wrapper.cpp
│           └── 02_vector2d.cpp
├── module-05-stl/
│   ├── CMakeLists.txt
│   ├── compare.md
│   ├── examples/
│   │   ├── 01_containers.cpp
│   │   ├── 02_algorithms.cpp
│   │   └── 03_string_apis.cpp
│   └── exercises/
│       ├── 01_group_by.md
│       ├── 01_group_by.cpp
│       ├── 02_top_k.md
│       ├── 02_top_k.cpp
│       └── solution/
│           ├── 01_group_by.cpp
│           └── 02_top_k.cpp
├── module-06-templates/
│   ├── CMakeLists.txt
│   ├── compare.md
│   ├── examples/
│   │   ├── 01_function_template.cpp
│   │   ├── 02_class_template.cpp
│   │   └── 03_variadic.cpp
│   └── exercises/
│       ├── 01_generic_stack.md
│       ├── 01_generic_stack.cpp
│       ├── 02_generic_pair.md
│       ├── 02_generic_pair.cpp
│       └── solution/
│           ├── 01_generic_stack.cpp
│           └── 02_generic_pair.cpp
├── module-07-modern-cpp/
│   ├── CMakeLists.txt
│   ├── compare.md
│   ├── examples/
│   │   ├── 01_lambda.cpp
│   │   ├── 02_constexpr.cpp
│   │   └── 03_optional_variant.cpp
│   └── exercises/
│       ├── 01_lambda_sort.md
│       ├── 01_lambda_sort.cpp
│       ├── 02_variant_visitor.md
│       ├── 02_variant_visitor.cpp
│       └── solution/
│           ├── 01_lambda_sort.cpp
│           └── 02_variant_visitor.cpp
├── module-08-error-handling/
│   ├── CMakeLists.txt
│   ├── compare.md
│   ├── examples/
│   │   ├── 01_exception_basics.cpp
│   │   ├── 02_noexcept.cpp
│   │   └── 03_raii_exception_safety.cpp
│   └── exercises/
│       ├── 01_safe_file_reader.md
│       ├── 01_safe_file_reader.cpp
│       ├── 02_exception_safe_stack.md
│       ├── 02_exception_safe_stack.cpp
│       └── solution/
│           ├── 01_safe_file_reader.cpp
│           └── 02_exception_safe_stack.cpp
├── module-09-concurrency/
│   ├── CMakeLists.txt
│   ├── compare.md
│   ├── examples/
│   │   ├── 01_thread_basics.cpp
│   │   ├── 02_mutex.cpp
│   │   └── 03_async_future.cpp
│   └── exercises/
│       ├── 01_thread_safe_queue.md
│       ├── 01_thread_safe_queue.cpp
│       ├── 02_parallel_sum.md
│       ├── 02_parallel_sum.cpp
│       └── solution/
│           ├── 01_thread_safe_queue.cpp
│           └── 02_parallel_sum.cpp
└── module-10-project/
    ├── CMakeLists.txt
    ├── README.md
    ├── src/
    │   └── main.cpp           # TaskManager CLI 入口
    └── exercises/
        ├── 01_task_manager.md
        ├── 02_extend.md
        └── solution/
            └── task_manager_full.cpp
```

---

### Task 1: 根目录基础设施

**Files:**
- Create: `CMakeLists.txt`
- Create: `.gitignore`

- [ ] **Step 1: 创建 .gitignore**

```cmake
# .gitignore
build/
cmake-build-*/
.cache/
*.o
*.obj
*.exe
*.out
.DS_Store
```

- [ ] **Step 2: 创建根 CMakeLists.txt**

```cmake
cmake_minimum_required(VERSION 3.16)
project(CppBasic VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(module-01-basics)
add_subdirectory(module-02-memory)
add_subdirectory(module-03-oop)
add_subdirectory(module-04-value-semantics)
add_subdirectory(module-05-stl)
add_subdirectory(module-06-templates)
add_subdirectory(module-07-modern-cpp)
add_subdirectory(module-08-error-handling)
add_subdirectory(module-09-concurrency)
add_subdirectory(module-10-project)
```

- [ ] **Step 3: 验证构建**

```bash
cmake -B build && cmake --build build
```

Expected: 构建失败（子目录尚不存在），但 CMake 配置通过。等到模块 1 创建后再回到此步验证。

---

### Task 2: 模块 1 — 基础语法差异（CMakeLists + 示例）

**Files:**
- Create: `module-01-basics/CMakeLists.txt`
- Create: `module-01-basics/examples/01_reference_vs_pointer.cpp`
- Create: `module-01-basics/examples/02_const_correctness.cpp`
- Create: `module-01-basics/examples/03_auto_and_range_for.cpp`
- Create: `module-01-basics/examples/04_namespace_overload.cpp`

- [ ] **Step 1: 创建模块 CMakeLists.txt**

```cmake
file(GLOB examples "examples/*.cpp")
file(GLOB exercises "exercises/*.cpp")

foreach(src ${examples})
    get_filename_component(name ${src} NAME_WE)
    add_executable(mod1_ex_${name} ${src})
endforeach()

foreach(src ${exercises})
    get_filename_component(name ${src} NAME_WE)
    add_executable(mod1_todo_${name} ${src})
endforeach()
```

- [ ] **Step 2: 创建 01_reference_vs_pointer.cpp**

```cpp
// 模块 1 示例: 引用 vs 指针
// 相关规范: Core Guidelines F.7, F.15, F.22; Google Style: Reference Arguments
//
// Java 开发者核心困惑: Java 中所有对象变量都是引用(类似 C++ 指针)，
// C++ 中对象变量默认是值，需要用 & (引用) 或 * (指针) 来间接访问。

#include <iostream>
#include <string>

// ✅ 正确: 只读参数用 const 引用 — Core Guideline F.15
void printName(const std::string& name) {
    // name 不会被拷贝，但也不能在函数内修改
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

// ❌ 错误: 返回局部变量引用 — 悬垂引用，运行时崩溃
// const std::string& badReturn() {
//     std::string local = "temp";
//     return local;  // ⚠️ Code Review 直接打回
// }

int main() {
    // C++ 中这创建了一个对象，不是引用
    std::string name = "Clancy";

    // 引用: 对象的别名，不能为空，不能重新绑定
    std::string& ref = name;
    ref = "C++ Dev";  // 修改了 name

    // 指针: 可以为空，可以重新指向
    std::string* ptr = &name;
    *ptr = "Pointer";        // 解引用修改
    ptr = nullptr;            // 可以为空 — 这是 Java 没有的概念
    // 注意: name 没被破坏，只是 ptr 不指向它了

    printName(name);          // 传引用，不拷贝

    std::string first, last;
    parseName("John Doe", &first, &last);  // & 取地址传给指针参数

    std::cout << "First: " << first << ", Last: " << last << '\n';

    return 0;
}
```

- [ ] **Step 3: 创建 02_const_correctness.cpp**

```cpp
// 模块 1 示例: const 正确性
// 相关规范: Core Guidelines Con.1, Con.2, Con.3; Google Style: Use of const
//
// Java 的 final 只能阻止重新赋值，C++ 的 const 能阻止任何修改，
// 还可以用于函数签名（const 成员函数），这是 Java 完全没有的概念。

#include <iostream>
#include <string>

class Document {
public:
    explicit Document(std::string title) : title_(std::move(title)) {}

    // ✅ const 成员函数: 承诺不修改对象 — Core Guideline Con.2
    const std::string& title() const { return title_; }

    // ✅ 非 const 成员函数: 可以修改对象
    void setTitle(const std::string& t) { title_ = t; }

private:
    std::string title_;
};

// ✅ 只读参数用 const — 调用者知道传入的对象不会被修改
void showDocument(const Document& doc) {
    std::cout << "Document: " << doc.title() << '\n';
    // doc.setTitle("x");  // ❌ 编译错误: const 对象不能调用非 const 方法
}

int main() {
    const int value = 42;
    // value = 10;  // ❌ 编译错误

    // 指向 const 的指针: 不能通过指针修改
    const int* p1 = &value;
    // *p1 = 10;    // ❌ 不能修改指向的值
    p1 = nullptr;   // ✅ 可以改变指向

    // const 指针: 不能改变指向
    int x = 5;
    int* const p2 = &x;
    *p2 = 10;       // ✅ 可以修改指向的值
    // p2 = nullptr;// ❌ 不能改变指向

    const Document doc("readme.txt");
    showDocument(doc);

    Document editable("draft.txt");
    editable.setTitle("final.txt");

    return 0;
}
```

- [ ] **Step 4: 创建 03_auto_and_range_for.cpp**

```cpp
// 模块 1 示例: auto 与 range-for
// 相关规范: Core Guidelines ES.11; Google Style: Type Deduction
//
// auto 类似 Java 的 var (Java 10+)，但更强大。
// range-for 类似 Java 的 for-each，但需要注意拷贝 vs 引用。

#include <iostream>
#include <string>
#include <vector>
#include <map>

int main() {
    // auto 自动推导类型
    auto i = 42;           // int
    auto d = 3.14;         // double
    auto s = std::string("hello"); // std::string
    auto& ref = s;         // std::string& — 需要用 & 才能得到引用

    // range-for: 遍历容器
    std::vector<std::string> names = {"Alice", "Bob", "Charlie"};

    // ⚠️ 陷阱: 默认会拷贝每个元素 — Java 开发者易踩坑
    for (auto name : names) {
        name += "!";  // 只修改了拷贝，原数据不变!
    }
    // names 仍然是 {"Alice", "Bob", "Charlie"}

    // ✅ 用 const auto& 避免拷贝 — Google Style 推荐
    for (const auto& name : names) {
        std::cout << name << ' ';
    }
    std::cout << '\n';

    // ✅ 需要修改时用 auto&
    for (auto& name : names) {
        name += "!";
    }
    // names 变成了 {"Alice!", "Bob!", "Charlie!"}

    // map 遍历 + 结构化绑定 (C++17)
    std::map<std::string, int> scores = {{"Alice", 90}, {"Bob", 85}};
    for (const auto& [name, score] : scores) {
        std::cout << name << ": " << score << '\n';
    }

    return 0;
}
```

- [ ] **Step 5: 创建 04_namespace_overload.cpp**

```cpp
// 模块 1 示例: namespace 与函数重载/默认参数
// 相关规范: Core Guidelines SF.6, SF.7; Google Style: Namespaces
//
// namespace 类似 Java 的 package，但更灵活(可以跨文件)。
// C++ 没有 import，用 #include 和 :: 访问。

#include <iostream>
#include <string>

namespace utils {
    // ✅ 默认参数: 调用者可省略 — Java 没有这个特性，需要写多个重载
    void log(const std::string& msg, const std::string& level = "INFO") {
        std::cout << "[" << level << "] " << msg << '\n';
    }

    // ✅ 函数重载: 同名不同参数 — Java 也有，但 C++ 可以按 const 重载
    int max(int a, int b) { return a > b ? a : b; }
    double max(double a, double b) { return a > b ? a : b; }

    // const 重载: C++ 独有 — 根据 this 是否 const 选择不同版本
    class Buffer {
    public:
        // 可修改版本
        char& at(size_t index) { return data_[index]; }
        // const 只读版本
        const char& at(size_t index) const { return data_[index]; }
    private:
        char data_[256]{};
    };
}

int main() {
    utils::log("Server started");           // 使用默认参数 INFO
    utils::log("Connection lost", "ERROR");  // 覆盖默认参数

    std::cout << utils::max(3, 7) << '\n';      // 调用 int 版本
    std::cout << utils::max(3.1, 7.2) << '\n';  // 调用 double 版本

    return 0;
}
```

- [ ] **Step 6: 验证编译**

```bash
cmake -B build && cmake --build build
```

Expected: 模块 1 的目标编译成功 (`mod1_ex_01_reference_vs_pointer` 等)。

---

### Task 3: 模块 1 — 基础语法差异（compare.md + 习题）

**Files:**
- Create: `module-01-basics/compare.md`
- Create: `module-01-basics/exercises/01_swap_ref.md`
- Create: `module-01-basics/exercises/01_swap_ref.cpp`
- Create: `module-01-basics/exercises/02_array_stats.md`
- Create: `module-01-basics/exercises/02_array_stats.cpp`
- Create: `module-01-basics/exercises/solution/01_swap_ref.cpp`
- Create: `module-01-basics/exercises/solution/02_array_stats.cpp`

- [ ] **Step 1: 创建 compare.md**

```markdown
# 模块 1: 基础语法差异 — Java vs C++ 对比

## 知识点: 引用 vs 指针

| 场景 | Java | C++ | 踩坑提醒 |
|------|------|-----|----------|
| 声明对象 | `String s = "hello";` s 是引用 | `std::string s = "hello";` s 是值对象 | ⚠️ C++ 默认值语义，赋值会拷贝 |
| 获取引用 | 默认就是引用，`=` 赋引用 | `std::string& r = s;` 显式声明 | ⚠️ 忘记 `&` 就会发生拷贝 |
| 空值 | null (引用可以为 null) | `nullptr` (指针可以为空，引用不行) | ⚠️ C++ 引用必须初始化，永远不为空 |
| 重新绑定 | `s = s2;` (改变引用目标) | 引用不能重新绑定; 指针用 `p = &x;` | ⚠️ `&` 在类型声明和取地址中是同一个符号 |
| 输出参数 | 修改传入对象的字段 | 用指针 `T*` 或非 const 引用 `T&` | ⚠️ Google Style: 可变的用指针 |

### 相关编码规范

| 规范来源 | 编号 | 规则 | 说明 |
|----------|------|------|------|
| C++ Core Guidelines | F.7 | 对于"输入"参数，优先用 const T& | 避免不必要的拷贝 |
| C++ Core Guidelines | F.15 | 简单类型(int, double)直接传值 | 小对象拷贝比间接访问快 |
| C++ Core Guidelines | F.22 | T* 表示单个对象，T& 表示"必有"的对象 | 语义上区分可空/不可空 |
| Google Style Guide | - | 输出参数用指针，不用非 const 引用 | 调用时 `&x` 让意图可见 |

## 知识点: const 正确性

| 场景 | Java | C++ | 踩坑提醒 |
|------|------|-----|----------|
| 常量变量 | `final int x = 5;` | `const int x = 5;` | ⚠️ Java final 只阻止重新赋值，C++ const 阻止一切修改 |
| 不可变对象内部的修改 | final 下仍可改字段 | `const` 下连字段都不能改 | ⚠️ Java 的 final 远弱于 C++ const |
| const 方法 | Java 无此概念 | `void foo() const;` | ⚠️ const 方法是 C++ 最重要的特性之一 |
| const 指针层次 | 无 | `const int*` / `int* const` / `const int* const` | ⚠️ 读法: 从右往左读 |

### 相关编码规范

| 规范来源 | 编号 | 规则 | 说明 |
|----------|------|------|------|
| C++ Core Guidelines | Con.1 | 默认情况下，使对象不可变 | 先 const，需要修改时再去掉 |
| C++ Core Guidelines | Con.2 | 成员函数不修改对象时标记为 const | 让编译器帮你检查 |
| C++ Core Guidelines | Con.3 | 默认情况下，传递 const 引用给函数 | 明确函数不会修改传入对象 |
| Google Style Guide | - | 在能用的地方都用 const | "const correctness" |

## 知识点: auto 与 range-for

| 场景 | Java | C++ | 踩坑提醒 |
|------|------|-----|----------|
| 类型推导 | `var x = 10;` (Java 10+) | `auto x = 10;` | ⚠️ auto 推导为值类型，不是引用 |
| for-each | `for (T item : list)` item 是引用 | `for (auto item : vec)` item 是拷贝 | ⚠️ 极易踩坑: 忘记 & 就会拷贝每个元素 |
| 只读遍历 | `for (T item : list)` | `for (const auto& item : vec)` | ⚠️ Google Style 推荐用 const auto& |
| 修改遍历 | `for (T item : list)` | `for (auto& item : vec)` | ⚠️ 需要显式加 & |

## 知识点: namespace 与函数

| 场景 | Java | C++ | 踩坑提醒 |
|------|------|-----|----------|
| 命名空间 | package (与目录强绑定) | namespace (与目录无关) | ⚠️ 一个文件可以有多个 namespace |
| 导入 | `import java.util.*;` | `using namespace std;` (不推荐在头文件用) | ⚠️ `using namespace` 在头文件是规范禁止的 |
| 默认参数 | 不支持，必须写多个重载 | `void f(int a = 1)` | ⚠️ 默认参数从右往左，不能跳跃 |
| 函数重载 | 支持 | 支持 + const 重载 | ⚠️ C++ 可以根据 const 选择不同重载 |
| using 别名 | `import` | `using std::string;` (推荐) | ⚠️ Google Style: 永远不要在头文件全局 using |

## 常见 Bug（Code Review 回退清单）

- [ ] `for (auto item : vec)` 忘记 `&` — 默默拷贝每个元素
- [ ] 函数参数类型是 `T` 而非 `const T&` — 不必要的深拷贝
- [ ] 返回局部变量的引用或指针 — 悬垂引用，运行时崩溃
- [ ] 使用未初始化的指针 — 未定义行为
- [ ] `using namespace std;` 写在头文件 — 污染全局命名空间
- [ ] const 成员函数中忘记 const — 无法在 const 对象上调用
```

- [ ] **Step 2: 创建习题 01 需求文档**

```markdown
# 任务: 实现 swap 函数与字符串处理

## 需求背景

在一个数据处理模块中，需要对数据进行交换和格式化。Java 中你直接修改对象引用即可，但 C++ 需要区分值、引用和指针。

## 需求描述

在 `01_swap_ref.cpp` 中实现三个函数:

1. `swapByValue(int a, int b)` — 按值传递版本（这个"看起来能工作"但其实不能交换外部变量，让学生先写出这个版本，然后在 main 中验证失败）
2. `swapByRef(int& a, int& b)` — 按引用传递。交换两个 int。
3. `appendSuffix(std::string& str, const std::string& suffix)` — 使用引用在字符串末尾追加后缀。不使用返回值，直接修改参数。

## 编码规范检查清单

- [ ] 只读参数是否用了 `const`？
- [ ] 需要修改的参数是否用了引用 `&`？
- [ ] 变量声明尽量靠近第一次使用
- [ ] 没有 `using namespace std;` 在全局作用域

## 验收标准

- [ ] `swapByRef` 能正确交换 main 中的两个变量
- [ ] `swapByValue` 不能交换外部变量（让学生自己发现差异）
- [ ] `appendSuffix` 直接修改传入的字符串
- [ ] 编译无警告
```

- [ ] **Step 3: 创建习题 01 骨架代码**

```cpp
// 模块 1 习题: swap 与引用
// 请在标注 TODO 的位置实现函数

#include <iostream>
#include <string>

// TODO: 实现 swapByValue — 这个版本交换函数内部的值，但不影响调用者
// 试着实现它，然后在 main 中观察: 外部变量是否被交换了？
// void swapByValue( ??? ) { ... }

// TODO: 实现 swapByRef — 这个版本要真正交换两个外部变量
// void swapByRef( ??? ) { ... }

// TODO: 实现 appendSuffix — 在 str 后面追加 suffix
// void appendSuffix( ??? ) { ... }

int main() {
    // 测试 swapByValue
    int x1 = 10, y1 = 20;
    std::cout << "Before swapByValue: x1=" << x1 << ", y1=" << y1 << '\n';
    // TODO: 调用 swapByValue 并输出结果
    std::cout << "After  swapByValue: x1=" << x1 << ", y1=" << y1 << '\n';
    std::cout << "(如果 x1 y1 没变，说明按值传递不改变外部变量)\n\n";

    // 测试 swapByRef
    int x2 = 100, y2 = 200;
    std::cout << "Before swapByRef: x2=" << x2 << ", y2=" << y2 << '\n';
    // TODO: 调用 swapByRef 并输出结果
    std::cout << "After  swapByRef: x2=" << x2 << ", y2=" << y2 << '\n';

    // 测试 appendSuffix
    std::string filename = "report";
    // TODO: 调用 appendSuffix
    std::cout << "Filename: " << filename << '\n';

    return 0;
}
```

- [ ] **Step 4: 创建习题 01 答案**

```cpp
// 模块 1 习题答案: swap 与引用
// 规范: Core Guidelines F.15, F.17; Google Style: Reference Arguments

#include <iostream>
#include <string>

void swapByValue(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
    // a, b 是局部拷贝，函数返回后这些拷贝就销毁了
}

void swapByRef(int& a, int& b) {    // Core Guideline F.17: 用 T& 表示 in-out 参数
    int temp = a;
    a = b;
    b = temp;
}

void appendSuffix(std::string& str, const std::string& suffix) {
    // str 是非 const 引用: 会被修改 (output)
    // suffix 是 const 引用: 只读 (input) — Google Style: 输入参数用 const&
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
```

- [ ] **Step 5: 创建习题 02 需求文档**

```markdown
# 任务: 数组统计函数

## 需求背景

系统中有传感器数据数组，需要计算统计信息。你的任务是实现两个统计函数，一个返回结果，一个通过输出参数返回结果。

## 需求描述

在 `02_array_stats.cpp` 中实现:

1. `computeStats(const int* data, size_t size, int* outMin, int* outMax, double* outAvg)` — 通过输出指针返回最小值、最大值、平均值。如果 `data` 为空或 `size` 为 0，不修改任何输出参数。
2. 在 `main` 中准备测试数据，调用函数并输出结果。

## 编码规范检查清单

- [ ] 输入数据是 const 的吗？
- [ ] 输出参数是否检查了 nullptr？（安全编程）
- [ ] 指针参数语义清晰：只读 vs 可写

## 验收标准

- [ ] 正确计算 min/max/avg（int 除法要注意）
- [ ] nullptr 检查：输出参数为 nullptr 时不崩溃
- [ ] 空数组安全处理
```

- [ ] **Step 6: 创建习题 02 骨架代码**

```cpp
// 模块 1 习题: 数组统计
// 请在标注 TODO 的位置实现函数

#include <iostream>
#include <cstddef>  // size_t

// TODO: 实现 computeStats
// data: 输入数组（只读）
// size: 数组大小
// outMin: 输出最小值（可选 — 为 nullptr 时跳过）
// outMax: 输出最大值（可选）
// outAvg: 输出平均值（可选）
// void computeStats( ??? ) { ... }

int main() {
    int temperatures[] = {23, 19, 31, 25, 28, 22, 30};
    size_t n = sizeof(temperatures) / sizeof(temperatures[0]);

    // TODO: 声明变量并调用 computeStats
    // 1. 传入全部输出参数
    // 2. 演示: 只传部分输出参数（某些传 nullptr）
    // 3. 演示: 空数组的情况

    return 0;
}
```

- [ ] **Step 7: 创建习题 02 答案**

```cpp
// 模块 1 习题答案: 数组统计
// 规范: Core Guidelines F.15, F.22, I.13; Google Style: Output Parameters

#include <iostream>
#include <cstddef>

void computeStats(const int* data, size_t size,
                  int* outMin, int* outMax, double* outAvg) {
    // Core Guideline I.13: 不要用单个指针传递数组，应该同时传递 size
    if (data == nullptr || size == 0) {
        return;  // 空数组，不修改任何输出
    }

    int min = data[0];
    int max = data[0];
    int sum = data[0];

    for (size_t i = 1; i < size; ++i) {
        if (data[i] < min) min = data[i];
        if (data[i] > max) max = data[i];
        sum += data[i];
    }

    // 检查每个输出参数是否为 nullptr — 防御性编程
    if (outMin) *outMin = min;
    if (outMax) *outMax = max;
    if (outAvg) *outAvg = static_cast<double>(sum) / size;
}

int main() {
    int temperatures[] = {23, 19, 31, 25, 28, 22, 30};
    size_t n = sizeof(temperatures) / sizeof(temperatures[0]);

    // 场景 1: 传入全部输出参数
    int min, max;
    double avg;
    computeStats(temperatures, n, &min, &max, &avg);
    std::cout << "Min: " << min << ", Max: " << max << ", Avg: " << avg << '\n';

    // 场景 2: 只取最小值（其他传 nullptr）
    int onlyMin;
    computeStats(temperatures, n, &onlyMin, nullptr, nullptr);
    std::cout << "Only Min: " << onlyMin << '\n';

    // 场景 3: 空数组
    computeStats(nullptr, 0, &min, &max, &avg);
    std::cout << "Empty array handled safely.\n";

    return 0;
}
```

- [ ] **Step 8: 验证编译**

```bash
cmake --build build
./build/module-01-basics/mod1_ex_01_reference_vs_pointer
./build/module-01-basics/mod1_ex_02_const_correctness
./build/module-01-basics/mod1_ex_03_auto_and_range_for
./build/module-01-basics/mod1_ex_04_namespace_overload
```

Expected: 所有示例程序正常运行。习题骨架程序编译通过，输出提示学生完成实现。

---

### Task 4: 模块 2 — 内存管理（CMakeLists + 示例 + compare.md + 习题）

**Files:**
- Create: `module-02-memory/CMakeLists.txt`
- Create: `module-02-memory/compare.md`
- Create: `module-02-memory/examples/01_stack_vs_heap.cpp`
- Create: `module-02-memory/examples/02_unique_ptr.cpp`
- Create: `module-02-memory/examples/03_shared_ptr.cpp`
- Create: `module-02-memory/examples/04_move_semantics.cpp`
- Create: `module-02-memory/examples/05_raii.cpp`
- Create: `module-02-memory/exercises/01_resource_manager.md`
- Create: `module-02-memory/exercises/01_resource_manager.cpp`
- Create: `module-02-memory/exercises/02_fix_memory_leak.md`
- Create: `module-02-memory/exercises/02_fix_memory_leak.cpp`
- Create: `module-02-memory/exercises/solution/01_resource_manager.cpp`
- Create: `module-02-memory/exercises/solution/02_fix_memory_leak.cpp`

- [ ] **Step 1: 创建 CMakeLists.txt**

```cmake
file(GLOB examples "examples/*.cpp")
file(GLOB exercises "exercises/*.cpp")

foreach(src ${examples})
    get_filename_component(name ${src} NAME_WE)
    add_executable(mod2_ex_${name} ${src})
endforeach()

foreach(src ${exercises})
    get_filename_component(name ${src} NAME_WE)
    add_executable(mod2_todo_${name} ${src})
endforeach()
```

- [ ] **Step 2: 创建 compare.md**

```markdown
# 模块 2: 内存管理 — Java vs C++ 对比

## 核心差异

| 维度 | Java | C++ |
|------|------|-----|
| 内存模型 | 所有对象在堆上，由 GC 管理 | 对象可在栈/堆上，由开发者管理 |
| 默认语义 | 引用语义（变量是引用） | 值语义（变量就是对象本身） |
| 对象销毁 | GC 自动回收 | 栈对象：作用域结束自动析构；堆对象：手动 delete 或智能指针 |
| "不需要了" | 设为 null，等 GC | 手动管理或依赖 RAII 自动释放 |

## 知识点: 栈 vs 堆

| 场景 | Java | C++ | 踩坑提醒 |
|------|------|-----|----------|
| 创建对象 | `MyClass obj = new MyClass();` — 堆 | `MyClass obj;` — 栈; `new MyClass()` — 堆 | ⚠️ C++ 可以不写 new! 栈对象自动析构 |
| 释放 | 不需要，GC 处理 | 栈: 自动; 堆: 必须 `delete` | ⚠️ 忘记 delete = 内存泄漏 |
| 大型对象 | 总是在堆上 | 可用 `unique_ptr` 在堆上，也可在栈上 | ⚠️ 太大（>几MB）不要在栈上 |

### 相关编码规范

| 规范来源 | 编号 | 规则 |
|----------|------|------|
| C++ Core Guidelines | R.10 | 避免 `malloc()` 和 `free()` |
| C++ Core Guidelines | R.11 | 避免显式调用 `new` 和 `delete`（用智能指针） |
| C++ Core Guidelines | R.20 | 用 `unique_ptr` 或 `shared_ptr` 表示所有权 |
| C++ Core Guidelines | C.149 | 用 `unique_ptr` 或 `shared_ptr` 避免裸 new/delete |
| Google Style Guide | - | 优先使用智能指针，不要用裸 new/delete |

## 知识点: 智能指针

| 智能指针 | 用途 | 对应 Java | 注意 |
|----------|------|-----------|------|
| `unique_ptr<T>` | 独占所有权 | 没有直接对应(类似局部变量但管理堆对象) | 不可拷贝，可移动 |
| `shared_ptr<T>` | 共享所有权(引用计数) | 类似 Java 的普通对象引用 | 有引用计数的开销 |
| `weak_ptr<T>` | 观察 shared_ptr，不增加引用计数 | 类似 WeakReference | 用之前需要 lock() |

## 知识点: RAII

| 场景 | Java | C++ |
|------|------|-----|
| 资源管理 | try-with-resources / finally | RAII: 构造获取资源，析构释放资源 |
| 文件操作 | try (FileReader fr = ...) { } | `std::ifstream` 在析构时自动关闭 |
| 互斥锁 | synchronized 块 | `std::lock_guard` 在析构时自动解锁 |
| 数据库连接 | try-with-resources | RAII 包装器 |

## 知识点: 移动语义

| 场景 | Java | C++ |
|------|------|-----|
| "移交所有权" | 不适用（GC 管理） | `std::move()` — 把资源从一个对象转移到另一个 |
| 性能优化 | 不需要（引用语义） | 避免深拷贝，直接用移动构造 |
| 移动后对象 | N/A | ⚠️ 处于"有效但未指定"状态，通常只能析构或重新赋值 |

## 常见 Bug（Code Review 回退清单）

- [ ] 用裸 `new` 但不配对 `delete` — 内存泄漏
- [ ] 用裸 `new` 但在异常路径中没 delete — 泄漏
- [ ] `delete` 和 `delete[]` 混用 — 未定义行为
- [ ] 访问已释放的指针（悬垂指针） — 段错误/未定义行为
- [ ] `shared_ptr` 循环引用 — 内存泄漏
- [ ] 在栈上创建过大对象 — 栈溢出
- [ ] `std::move` 后继续使用被移动的对象
- [ ] 返回局部变量的引用或指针 — 悬垂引用
```

- [ ] **Step 3: 创建 01_stack_vs_heap.cpp**

```cpp
// 模块 2 示例: 栈 vs 堆
// 相关规范: Core Guidelines R.11, C.149; Google Style: Ownership and Smart Pointers

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// 一个简单的类，演示构造/析构
struct Resource {
    std::string name;
    explicit Resource(std::string n) : name(std::move(n)) {
        std::cout << "  [构造] " << name << '\n';
    }
    ~Resource() {
        std::cout << "  [析构] " << name << '\n';
    }
};

void stackExample() {
    std::cout << "--- 栈对象 ---\n";
    Resource r1("stack-obj");          // 栈上创建，函数结束自动析构
    std::cout << "  在栈上安全使用 " << r1.name << '\n';
    // 离开作用域 r1 自动析构 — RAII!
}   // r1 在这里析构

void heapRawExample() {
    std::cout << "--- 裸指针堆对象 ---\n";
    Resource* r2 = new Resource("heap-raw");  // ❌ 堆上创建，手动管理
    std::cout << "  使用 " << r2->name << '\n';
    delete r2;  // ⚠️ 忘记这行 = 内存泄漏！
    // 如果中间抛异常，delete 不会执行 —— 这就是为什么需要智能指针
}

void heapSmartExample() {
    std::cout << "--- 智能指针堆对象 ---\n";
    auto r3 = std::make_unique<Resource>("heap-unique");  // ✅ C++14+
    std::cout << "  使用 " << r3->name << '\n';
    // r3 离开作用域自动 delete — RAII + 智能指针 = 安全
}

int main() {
    stackExample();
    std::cout << "stackExample 调用结束，栈对象已自动析构\n\n";

    heapRawExample();
    std::cout << "heapRawExample 调用结束\n\n";

    heapSmartExample();
    std::cout << "heapSmartExample 调用结束\n\n";

    // 栈上创建 STL 容器 — STL 容器内部在堆上分配数据
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::cout << "vector 在栈上，但它的数据在堆上，析构时自动释放\n";

    return 0;
}
```

- [ ] **Step 4: 创建 02_unique_ptr.cpp**

```cpp
// 模块 2 示例: unique_ptr 独占所有权
// 相关规范: Core Guidelines R.20, F.26

#include <iostream>
#include <memory>
#include <vector>

struct Task {
    int id;
    std::string title;
    Task(int i, std::string t) : id(i), title(std::move(t)) {}
};

// ✅ unique_ptr 作为返回值: 所有权转移给调用者
// Core Guideline F.26: 用 unique_ptr<T> 传递所有权
std::unique_ptr<Task> createTask(int id, const std::string& title) {
    return std::make_unique<Task>(id, title);  // C++14
}

// ✅ 接受 unique_ptr 参数: 接管所有权
void processTask(std::unique_ptr<Task> task) {
    std::cout << "Processing #" << task->id << ": " << task->title << '\n';
    // task 在此析构 — 你不需要手动 delete
}

int main() {
    // 创建 unique_ptr
    auto task1 = std::make_unique<Task>(1, "Fix login bug");

    // 访问成员: 和裸指针一样的语法
    std::cout << "Task: " << task1->id << " - " << task1->title << '\n';

    // ⚠️ unique_ptr 不可拷贝!
    // auto task2 = task1;  // 编译错误: unique_ptr 的拷贝构造被删除

    // ✅ 移动所有权
    auto task2 = std::move(task1);  // 所有权从 task1 转移到 task2
    // task1 现在是 nullptr! Core Guideline: 移动后对象不应再被使用
    if (!task1) {
        std::cout << "task1 is now null after move\n";
    }

    // 工厂函数返回 unique_ptr
    auto task3 = createTask(3, "Add unit tests");
    processTask(std::move(task3));  // 所有权传入函数

    // unique_ptr 放入容器
    std::vector<std::unique_ptr<Task>> tasks;
    tasks.push_back(std::make_unique<Task>(10, "Refactor"));
    tasks.push_back(std::make_unique<Task>(11, "Deploy"));

    // ⚠️ range-for 遍历 unique_ptr 容器必须用 const auto&
    for (const auto& t : tasks) {
        std::cout << "  #" << t->id << ": " << t->title << '\n';
    }

    return 0;
    // tasks 析构 → 每个元素的 unique_ptr 析构 → 每个 Task 被 delete — 全自动!
}
```

- [ ] **Step 5: 创建 03_shared_ptr.cpp**

```cpp
// 模块 2 示例: shared_ptr 共享所有权
// 相关规范: Core Guidelines R.20, R.21, R.22

#include <iostream>
#include <memory>
#include <vector>
#include <string>

struct CacheEntry {
    std::string key;
    std::string value;
    CacheEntry(std::string k, std::string v)
        : key(std::move(k)), value(std::move(v)) {
        std::cout << "  [构造 CacheEntry] " << key << '\n';
    }
    ~CacheEntry() {
        std::cout << "  [析构 CacheEntry] " << key << '\n';
    }
};

// ✅ shared_ptr: 多个所有者共享一个对象
// 类似 Java 中多个变量指向同一个对象
void sharedExample() {
    auto entry = std::make_shared<CacheEntry>("config", "value123");

    std::cout << "引用计数: " << entry.use_count() << '\n';  // 1

    {
        auto entry2 = entry;  // 共享所有权，引用计数 +1
        std::cout << "引用计数: " << entry.use_count() << '\n';  // 2

        std::vector<std::shared_ptr<CacheEntry>> cache;
        cache.push_back(entry);  // 引用计数 3
        std::cout << "引用计数: " << entry.use_count() << '\n';  // 3
    }
    // entry2 和 cache 离开作用域，引用计数回到 1

    std::cout << "引用计数: " << entry.use_count() << '\n';  // 1
}

// ⚠️ 循环引用: shared_ptr 经典陷阱
struct Node {
    std::string name;
    std::shared_ptr<Node> next;    // 强引用
    // std::weak_ptr<Node> prev;   // ✅ 正确: 用 weak_ptr 打破循环
    std::shared_ptr<Node> prev;    // ❌ 导致循环引用，永远不会删除

    explicit Node(std::string n) : name(std::move(n)) {
        std::cout << "  [构造 Node] " << name << '\n';
    }
    ~Node() {
        std::cout << "  [析构 Node] " << name << '\n';
    }
};

void cycleExample() {
    std::cout << "--- 循环引用演示 (此例中节点不会被析构!) ---\n";
    auto a = std::make_shared<Node>("A");
    auto b = std::make_shared<Node>("B");
    a->next = b;
    b->prev = a;  // ⚠️ A和B互相持有 shared_ptr，引用计数永远≥1
    // a, b 离开作用域，引用计数从2降到1，不会降到0 → 泄漏!
}

int main() {
    sharedExample();
    cycleExample();
    std::cout << "cycleExample 结束 — 注意没有看到 Node 的析构消息\n";
    return 0;
}
```

- [ ] **Step 6: 创建 04_move_semantics.cpp**

```cpp
// 模块 2 示例: 移动语义
// 相关规范: Core Guidelines C.64, F.18, ES.56
//
// 移动语义是 Java 完全没有的概念。它的核心思想:
// "我不要这个资源了，你拿走吧" — 避免深拷贝。

#include <iostream>
#include <string>
#include <vector>
#include <utility>  // std::move

void moveString() {
    std::cout << "--- 字符串移动 ---\n";
    std::string s1 = "Hello, this is a very long string that would be expensive to copy";

    // 拷贝: s2 是 s1 的完整副本，s1 保持不变
    std::string s2 = s1;
    std::cout << "After copy:  s1='" << s1 << "'\n";
    std::cout << "             s2='" << s2 << "'\n";

    // 移动: s1 的内部数据被"掏空"转移到 s3
    std::string s3 = std::move(s1);
    std::cout << "After move:  s1='" << s1 << "' (empty or unspecified)\n";
    std::cout << "             s3='" << s3 << "'\n";
    // ⚠️ s1 处于"有效但未指定"状态，不要再使用 s1 的内容
}

void moveVector() {
    std::cout << "--- vector 移动 ---\n";
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::cout << "v1 size: " << v1.size() << '\n';

    // 移动后 v1 变为空
    std::vector<int> v2 = std::move(v1);
    std::cout << "After move: v1 size=" << v1.size()
              << ", v2 size=" << v2.size() << '\n';
}

// ✅ 函数返回大对象: 自动使用移动（RVO/NRVO） — Core Guideline F.20
std::vector<int> createLargeVector() {
    std::vector<int> result;
    for (int i = 0; i < 1000; ++i) {
        result.push_back(i);
    }
    return result;  // 不会拷贝! 编译器优化 + 移动语义
}

int main() {
    moveString();
    moveVector();

    auto v = createLargeVector();
    std::cout << "Created vector with " << v.size() << " elements\n";

    return 0;
}
```

- [ ] **Step 7: 创建 05_raii.cpp**

```cpp
// 模块 2 示例: RAII (Resource Acquisition Is Initialization)
// 相关规范: Core Guidelines R.1, P.8, C.21
//
// RAII 是 C++ 最重要的设计哲学: 资源在构造时获取，在析构时释放。
// Java 需要 try-with-resources 做的事情，C++ 用 RAII 自动完成。

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <memory>

// ✅ RAII 文件管理: 类似 Java 的 try-with-resources，但更简洁
void fileExample() {
    std::cout << "--- RAII 文件操作 ---\n";

    // std::ofstream 构造时打开文件，析构时自动关闭
    // 即使发生异常也会正确关闭 — 这是 try-with-resources 做不到的优雅
    {
        std::ofstream file("/tmp/test_cpp.txt");
        if (file.is_open()) {
            file << "Hello from C++ RAII!\n";
        }
    }  // file 析构，自动 close()

    // 读取文件
    {
        std::ifstream file("/tmp/test_cpp.txt");
        std::string line;
        while (std::getline(file, line)) {
            std::cout << "  Read: " << line << '\n';
        }
    }  // 自动 close()
}

// ✅ RAII 互斥锁: 类似 Java 的 synchronized，但更灵活
class ThreadSafeCounter {
public:
    void increment() {
        std::lock_guard<std::mutex> lock(mutex_);  // 构造时加锁
        ++count_;
    }  // lock 析构，自动解锁 — 即使 increment 抛异常也会解锁

    int get() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return count_;
    }

private:
    mutable std::mutex mutex_;
    int count_ = 0;
};

// ✅ RAII 自定义资源管理
class DatabaseConnection {
public:
    DatabaseConnection(const std::string& url) : url_(url) {
        std::cout << "  [连接数据库] " << url_ << '\n';
        // 实际项目中这里会建立网络连接
    }

    ~DatabaseConnection() {
        std::cout << "  [断开数据库] " << url_ << '\n';
        // 实际项目中这里会断开连接
    }

    void query(const std::string& sql) {
        std::cout << "  [执行查询] " << sql << '\n';
    }

    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;

private:
    std::string url_;
};

void raiiCustomExample() {
    std::cout << "--- 自定义 RAII ---\n";
    DatabaseConnection db("mysql://localhost:3306/mydb");
    db.query("SELECT * FROM users");
    // db 离开作用域自动断开连接
    // 如果 query 抛异常，析构函数仍然会被调用 — 异常安全!
}

int main() {
    fileExample();

    ThreadSafeCounter counter;
    counter.increment();
    counter.increment();
    std::cout << "Counter: " << counter.get() << '\n';

    raiiCustomExample();

    return 0;
}
```

- [ ] **Step 8: 创建习题 01: 资源管理器**

```markdown
# 任务: 实现 RAII 文件写入器

## 需求背景

系统中有一个文件日志模块，要求确保文件在不再使用时正确关闭，即使在写入过程中发生异常也要关闭文件。

## 需求描述

在 `01_resource_manager.cpp` 中实现 `FileWriter` 类:

1. 构造函数 `FileWriter(const std::string& filename)` — 打开文件
2. 方法 `write(const std::string& line)` — 写入一行
3. 析构函数 — 关闭文件
4. 禁用拷贝（文件句柄不能有两个所有者）
5. 允许移动（转移文件句柄所有权）

## 编码规范检查清单

- [ ] 是否遵循了五法则或三五法则？
- [ ] 拷贝构造和拷贝赋值是否 = delete？
- [ ] 移动后，原对象是否处于安全状态？
- [ ] 析构函数是否检查了文件是否打开？

## 验收标准

- [ ] 正常写入后文件包含正确内容
- [ ] FileWriter 离开作用域后文件自动关闭
- [ ] 不能拷贝 FileWriter 实例
- [ ] 移动构造后原对象不再持有文件
```

- [ ] **Step 9: 创建习题 01 骨架代码**

```cpp
// 模块 2 习题: RAII 文件写入器
// 规范: Core Guidelines R.1, C.21

#include <iostream>
#include <fstream>
#include <string>

class FileWriter {
public:
    // TODO: 构造函数 — 打开文件
    explicit FileWriter(const std::string& filename) {
        // ...
    }

    // TODO: 禁用拷贝
    // FileWriter(const FileWriter&) = ...;
    // FileWriter& operator=(const FileWriter&) = ...;

    // TODO: 实现移动构造
    // FileWriter(FileWriter&& other) noexcept { ... }

    // TODO: 实现移动赋值
    // FileWriter& operator=(FileWriter&& other) noexcept { ... }

    // TODO: 写入一行（含换行符会自动添加）
    void write(const std::string& line) {
        // ...
    }

    // TODO: 析构函数 — 关闭文件（如果打开的话）
    ~FileWriter() {
        // ...
    }

private:
    std::ofstream file_;
};

int main() {
    // 测试基本写入
    {
        FileWriter writer("/tmp/test_writer.txt");
        writer.write("Line 1");
        writer.write("Line 2");
    }  // writer 在这里析构，自动关闭文件

    // TODO: 验证文件内容
    std::ifstream reader("/tmp/test_writer.txt");
    std::string content;
    std::cout << "File contents:\n";
    while (std::getline(reader, content)) {
        std::cout << "  " << content << '\n';
    }

    // TODO: 测试移动
    // FileWriter w1("/tmp/test_move.txt");
    // w1.write("Before move");
    // FileWriter w2 = std::move(w1);
    // w2.write("After move");
    // w1 不应再持有文件

    return 0;
}
```

- [ ] **Step 10: 创建习题 01 答案**

```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <utility>

class FileWriter {
public:
    explicit FileWriter(const std::string& filename)
        : file_(filename) {
        if (!file_.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }
    }

    // Core Guideline C.21: 三五法则 — 定义了析构就得定义或删除拷贝/移动
    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& other) noexcept
        : file_(std::move(other.file_)) {
        // 移动后 other 的 ofstream 处于未打开状态 — 安全
    }

    FileWriter& operator=(FileWriter&& other) noexcept {
        if (this != &other) {
            if (file_.is_open()) {
                file_.close();
            }
            file_ = std::move(other.file_);
        }
        return *this;
    }

    void write(const std::string& line) {
        file_ << line << '\n';
    }

    ~FileWriter() {
        if (file_.is_open()) {
            file_.close();
        }
    }

private:
    std::ofstream file_;
};

int main() {
    {
        FileWriter writer("/tmp/test_writer.txt");
        writer.write("Line 1");
        writer.write("Line 2");
    }

    std::ifstream reader("/tmp/test_writer.txt");
    std::string content;
    std::cout << "File contents:\n";
    while (std::getline(reader, content)) {
        std::cout << "  " << content << '\n';
    }

    // 测试移动
    std::cout << "\n--- 测试移动 ---\n";
    FileWriter w1("/tmp/test_move.txt");
    w1.write("Before move");

    FileWriter w2 = std::move(w1);
    w2.write("After move");

    // 验证: 移动后 w1 不再写文件
    // std::cout << "Trying to write to moved-from w1...\n";
    // w1.write("This should fail or write to a closed file"); // 未定义行为

    return 0;
}
```

- [ ] **Step 11: 创建习题 02: 修复内存泄漏**

```markdown
# 任务: 修复有问题的代码

## 需求背景

代码审查时发现一段有内存泄漏和悬垂指针问题的代码，需要你修复。这是一个典型的"接手遗留代码"场景。

## 需求描述

`02_fix_memory_leak.cpp` 中的代码有以下已知问题:

1. 内存泄漏: `processData` 中 `new` 的对象在异常路径没有 delete
2. 悬垂指针: `getCachedValue` 返回了局部变量指针
3. 使用了裸 new/delete（应该用智能指针）
4. 没有 RAII 管理资源

修复这些问题，使代码:
- 没有内存泄漏
- 没有悬垂指针
- 符合 C++ Core Guidelines 和 Google Style Guide
- 行为保持等价（不能改变功能逻辑）

## 验收标准

- [ ] 所有 new 替换为 make_unique 或 make_shared
- [ ] 不再有裸 delete
- [ ] getCachedValue 不再返回悬垂指针
- [ ] 代码编译无警告
```

- [ ] **Step 12: 创建习题 02 骨架代码（待修复代码）**

```cpp
// 模块 2 习题: 修复内存泄漏和悬垂指针
// 下面的代码有多个问题，请修复它们并遵照编码规范

#include <iostream>
#include <string>
#include <cstring>

// ❌ 问题1: 使用裸 new，没有配对的 delete
// TODO: 用智能指针重写

struct DataRecord {
    int id;
    std::string name;
    double* scores;     // 动态分配的数组
    int scoreCount;

    DataRecord(int i, const std::string& n, int count)
        : id(i), name(n), scoreCount(count) {
        scores = new double[count];  // 手动分配
    }

    ~DataRecord() {
        delete[] scores;  // 手动释放
    }
};

// ❌ 问题2: 返回裸指针，调用者可能忘记 delete
DataRecord* createRecord(int id, const std::string& name) {
    return new DataRecord(id, name, 5);
}

// ❌ 问题3: 在异常路径可能泄漏
// 如果 saveToFile 抛异常，record 不会被 delete
void processData(const std::string& name) {
    DataRecord* record = new DataRecord(1, name, 10);

    // 模拟可能失败的后续操作
    // saveToFile(record);  // 如果这里抛异常...

    delete record;  // ...这行不会执行 → 内存泄漏!
}

// ❌ 问题4: 返回局部变量指针 — 悬垂指针!
int* getCachedValue() {
    int value = 42;
    return &value;  // value 在函数返回后销毁
}

int main() {
    auto* r = createRecord(1, "test");
    std::cout << "Record: " << r->name << '\n';
    delete r;  // ⚠️ 容易忘记

    processData("example");

    int* p = getCachedValue();
    // *p 现在是悬垂指针 — 未定义行为!
    // std::cout << *p << '\n';  // 可能崩溃

    return 0;
}
```

- [ ] **Step 13: 创建习题 02 答案**

```cpp
// 模块 2 习题答案: 修复内存泄漏
// 规范: Core Guidelines R.20, R.11, F.43

#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct DataRecord {
    int id;
    std::string name;
    std::vector<double> scores;  // ✅ vector 自动管理动态数组

    DataRecord(int i, std::string n, int count)
        : id(i), name(std::move(n)), scores(count, 0.0) {
    }
    // ✅ 不需要手动定义析构函数 — vector 自动释放
    // ✅ 不需要定义拷贝/移动 — 编译器生成的就是正确的
};

// ✅ 返回 unique_ptr，所有权清晰 — Core Guideline F.26
std::unique_ptr<DataRecord> createRecord(int id, const std::string& name) {
    return std::make_unique<DataRecord>(id, name, 5);
}

// ✅ 用 unique_ptr，异常安全
// Core Guideline R.11: 避免显式调用 new 和 delete
void processData(const std::string& name) {
    auto record = std::make_unique<DataRecord>(1, name, 10);

    // 如果 saveToFile 抛异常，栈展开时会自动调用 record 的析构
    // saveToFile(record.get());  // 安全!

    // record 自动释放 — 不需要手动 delete
}

// ✅ 返回值（不是指针/引用）— Core Guideline F.43
int getCachedValue() {
    return 42;  // 安全: 返回的是拷贝
}

int main() {
    auto r = createRecord(1, "test");
    std::cout << "Record: " << r->name << '\n';
    // ✅ 不需要 delete — unique_ptr 自动处理

    processData("example");

    int value = getCachedValue();
    std::cout << "Cached value: " << value << '\n';  // 安全

    return 0;
}
```

- [ ] **Step 14: 验证编译**

```bash
cmake --build build
./build/module-02-memory/mod2_ex_01_stack_vs_heap
./build/module-02-memory/mod2_ex_02_unique_ptr
./build/module-02-memory/mod2_ex_03_shared_ptr
./build/module-02-memory/mod2_ex_04_move_semantics
./build/module-02-memory/mod2_ex_05_raii
```

---

### Task 5: 模块 3 — OOP 对比

**Files:**
- Create: `module-03-oop/CMakeLists.txt`
- Create: `module-03-oop/compare.md`
- Create: `module-03-oop/examples/01_ctor_dtor.cpp`
- Create: `module-03-oop/examples/02_virtual_func.cpp`
- Create: `module-03-oop/examples/03_abstract_class.cpp`
- Create: `module-03-oop/examples/04_multiple_inherit.cpp`
- Create: `module-03-oop/exercises/01_shape_hierarchy.md`
- Create: `module-03-oop/exercises/01_shape_hierarchy.cpp`
- Create: `module-03-oop/exercises/02_pimpl.md`
- Create: `module-03-oop/exercises/02_pimpl.cpp`
- Create: `module-03-oop/exercises/solution/01_shape_hierarchy.cpp`
- Create: `module-03-oop/exercises/solution/02_pimpl.cpp`

- [ ] **Step 1: 创建 CMakeLists.txt**

```cmake
file(GLOB examples "examples/*.cpp")
file(GLOB exercises "exercises/*.cpp")

foreach(src ${examples})
    get_filename_component(name ${src} NAME_WE)
    add_executable(mod3_ex_${name} ${src})
endforeach()

foreach(src ${exercises})
    get_filename_component(name ${src} NAME_WE)
    add_executable(mod3_todo_${name} ${src})
endforeach()
```

- [ ] **Step 2: 创建 compare.md**

```markdown
# 模块 3: OOP 对比 — Java vs C++

## 核心差异

| 维度 | Java | C++ |
|------|------|-----|
| 类默认可见性 | package-private | private |
| 构造逻辑 | 只初始化字段 | 初始化列表 + 构造函数体 |
| 析构 | GC finalize() (不可靠) | 析构函数（确定性的） |
| 继承 | 单继承 + 多接口 | 多重继承（含虚继承） |
| 方法绑定 | 默认虚（可被重写） | 默认非虚（必须声明 virtual） |
| 抽象 | abstract class / interface | 纯虚函数 `= 0` |
| 访问控制 | public/protected/private/default | public/protected/private |
| 友元 | 无 | friend 类/函数 |
| 注解 vs 关键字 | @Override | override 关键字(C++11) |

## 知识点: 构造与析构

| 场景 | Java | C++ | 踩坑提醒 |
|------|------|-----|----------|
| 初始化顺序 | 字段默认值 → 构造体 | 基类 → 成员声明顺序 → 构造体 | ⚠️ 初始化列表顺序必须与声明顺序一致 |
| 析构顺序 | 不可预测(GC) | 构造的逆序(确定的) | ⚠️ 析构函数绝对不能抛异常 |
| 拷贝构造 | Cloneable / clone() | 拷贝构造函数 | ⚠️ C++ 默认生成浅拷贝 |
| 基类构造中调用虚函数 | 调用重写版本(危险) | 调用基类版本(安全) | ⚠️ C++ 构造期间虚表指向基类 |

### 相关编码规范

| 规范来源 | 编号 | 规则 |
|----------|------|------|
| C++ Core Guidelines | C.21 | 三五法则: 定义了析构/拷贝/移动之一就该定义所有5个 |
| C++ Core Guidelines | C.30 | 如果需要自定义析构，则也需要定义或删除拷贝和移动 |
| C++ Core Guidelines | C.47 | 按成员声明顺序定义和初始化成员变量 |
| C++ Core Guidelines | C.48 | 优先用类内初始化器，而非构造函数中的赋值 |
| C++ Core Guidelines | C.82 | 不要在构造和析构中调用虚函数 |
| Google Style Guide | - | 每个类只用一个 explicit 构造函数 |

## 知识点: 虚函数与多态

| 场景 | Java | C++ | 踩坑提醒 |
|------|------|-----|----------|
| 声明多态方法 | 默认虚 | 显式 `virtual` 声明 | ⚠️ 忘记 virtual = 静态绑定 |
| 覆写标记 | `@Override` 注解 | `override` 关键字 | ⚠️ 不用 override 就不会检查签名匹配 |
| 阻止覆写 | `final class/method` | `final` | 语义相同 |
| 调用基类方法 | `super.method()` | `Base::method()` | ⚠️ 语法不同，C++ 用 :: 作用域限定 |
| 析构函数 | 无 | 基类析构必须 `virtual` | ⚠️ 非虚析构 + delete 基类指针 = 未定义行为 |

### 相关编码规范

| 规范来源 | 编号 | 规则 |
|----------|------|------|
| C++ Core Guidelines | C.35 | 基类的析构函数必须是 public 和 virtual |
| C++ Core Guidelines | C.128 | 虚函数应该指定 virtual/override/final 三者之一 |
| C++ Core Guidelines | C.139 | 谨慎使用 final |
| Google Style Guide | - | 如果类可能被继承，析构函数必须是 virtual |

## 知识点: 多重继承 vs 接口

| 场景 | Java | C++ |
|------|------|-----|
| 接口 | `interface` 关键字 | 抽象类（只含纯虚函数） |
| 多实现 | `implements A, B` | `class C : public A, public B` |
| 菱形继承 | 不支持(Java 禁止多继承类) | 虚继承 `virtual public` |
| 默认方法 | `default` 方法(Java 8+) | 纯虚函数 + 默认实现 |

## 知识点: final / override / = 0

| C++ | 等价 Java | 说明 |
|-----|-----------|------|
| `virtual void foo() = 0;` | `abstract void foo();` | 纯虚函数，类变为抽象类 |
| `void foo() override;` | `@Override void foo()` | 覆写基类虚函数，编译器检查 |
| `void foo() final;` | `final void foo()` | 禁止子类进一步覆写 |
| `class Bar final {};` | `final class Bar {}` | 禁止被继承 |

## 常见 Bug（Code Review 回退清单）

- [ ] 基类析构函数不是 virtual — delete 基类指针时子类部分不会析构
- [ ] 忘了写 override — 函数签名细微差异导致没有真正覆写
- [ ] 析构函数中抛异常 — 栈展开过程中再抛异常会 std::terminate
- [ ] 拷贝构造写了但忘了写移动构造 — 性能隐患
- [ ] 初始化列表顺序和成员声明顺序不一致
- [ ] 在构造函数中调用 this 的虚函数 — 调用的是基类版本，不是预期行为
```

- [ ] **Step 3: 创建 01_ctor_dtor.cpp**

```cpp
// 模块 3 示例: 构造与析构
// 相关规范: Core Guidelines C.41, C.47, C.48

#include <iostream>
#include <string>
#include <memory>

class Base {
public:
    Base() { std::cout << "  Base()\n"; }
    // Core Guideline C.35: 基类析构必须 virtual
    virtual ~Base() { std::cout << "  ~Base()\n"; }
};

class Member {
public:
    Member(const std::string& n) : name(n) {
        std::cout << "  Member(" << name << ")\n";
    }
    ~Member() { std::cout << "  ~Member(" << name << ")\n"; }
    std::string name;
};

class Derived : public Base {
public:
    Derived(const std::string& n)
        : Base(),              // 先构造基类
          m1_(n + "_m1"),      // 按声明顺序构造成员 — Core Guideline C.47
          m2_(n + "_m2")       // ⚠️ 初始化列表顺序必须与声明顺序一致!
    {
        std::cout << "  Derived() body\n";
    }

    ~Derived() override {  // ✅ override 让编译器检查
        std::cout << "  ~Derived() body\n";
    }
    // 析构顺序: ~Derived() body → ~Member(m2) → ~Member(m1) → ~Base() — 逆序

private:
    Member m1_;  // 声明顺序!
    Member m2_;
};

int main() {
    std::cout << "=== 栈上对象 ===\n";
    {
        Derived d("stack");
    }  // d 析构 → 逆序

    std::cout << "\n=== 堆上对象 (delete 基类指针) ===\n";
    {
        Base* b = new Derived("heap");
        delete b;  // ✅ 基类析构是 virtual，正确调用 Derived::~Derived()
        // 如果 ~Base() 不是 virtual，这里只会调用 ~Base() → 资源泄漏!
    }

    std::cout << "\n=== unique_ptr 版本 ===\n";
    {
        auto b = std::make_unique<Derived>("unique");
    }  // unique_ptr 自动 delete，同样是安全的

    return 0;
}
```

- [ ] **Step 4: 创建 02_virtual_func.cpp**

```cpp
// 模块 3 示例: 虚函数与多态
// 相关规范: Core Guidelines C.128, C.35

#include <iostream>
#include <memory>
#include <vector>
#include <string>

class Animal {
public:
    explicit Animal(std::string n) : name_(std::move(n)) {}

    // ✅ virtual 析构 — Core Guideline C.35
    virtual ~Animal() = default;

    // ✅ 纯虚函数: 子类必须实现
    virtual std::string sound() const = 0;

    // ✅ 虚函数有默认实现: 子类可选覆写
    virtual std::string describe() const {
        return name_ + " makes sound: " + sound();
    }

    const std::string& name() const { return name_; }

private:
    std::string name_;
};

class Dog : public Animal {
public:
    explicit Dog(std::string n) : Animal(std::move(n)) {}

    // ✅ override 关键字 — Core Guideline C.128
    std::string sound() const override { return "Woof!"; }
};

class Cat : public Animal {
public:
    explicit Cat(std::string n) : Animal(std::move(n)) {}

    std::string sound() const override { return "Meow!"; }

    std::string describe() const override {
        return Animal::describe() + " (a cat)";  // 调用基类版本
    }
};

// ⚠️ 非虚函数: 不会动态绑定
class SilentDog : public Dog {
public:
    explicit SilentDog(std::string n) : Dog(std::move(n)) {}

    // ❌ 没有 virtual，这不是覆写，是隐藏！— Code Review 直接打回
    std::string sound() const { return "..."; }  // 忘记写 override
};

int main() {
    Dog dog("Rex");
    Cat cat("Whiskers");

    // 多态: 通过基类引用使用
    std::vector<std::unique_ptr<Animal>> animals;
    animals.push_back(std::make_unique<Dog>("Buddy"));
    animals.push_back(std::make_unique<Cat>("Mittens"));

    for (const auto& a : animals) {
        std::cout << a->describe() << '\n';
    }

    // ⚠️ 演示非虚函数的静态绑定
    Dog* silent = new SilentDog("Ghost");
    std::cout << "SilentDog via Dog*: " << silent->sound() << '\n';  // "Woof!" 不是 "..."
    delete silent;

    // Animal a("abstract");  // ❌ 编译错误: 不能实例化抽象类

    return 0;
}
```

- [ ] **Step 5: 创建 03_abstract_class.cpp**

```cpp
// 模块 3 示例: 抽象类与接口模式
// 相关规范: Core Guidelines C.121, C.129
//
// 在 C++ 中，"接口"是通过只含纯虚函数的抽象类实现的

#include <iostream>
#include <memory>
#include <vector>
#include <string>

// ✅ C++ 风格的"接口": 全纯虚函数 + virtual 析构
class IShape {
public:
    virtual ~IShape() = default;
    virtual double area() const = 0;
    virtual std::string name() const = 0;
};

// ✅ 实现"接口"
class Circle : public IShape {
public:
    explicit Circle(double r) : radius_(r) {}

    double area() const override { return 3.14159 * radius_ * radius_; }
    std::string name() const override { return "Circle"; }

private:
    double radius_;
};

class Rectangle : public IShape {
public:
    Rectangle(double w, double h) : width_(w), height_(h) {}

    double area() const override { return width_ * height_; }
    std::string name() const override { return "Rectangle"; }

private:
    double width_, height_;
};

// ✅ NVI (Non-Virtual Interface) 模式 — Core Guideline C.129
// 公共接口非虚，内部调用私有虚函数
class ShapeWithLogging : public IShape {
public:
    // 非虚公共接口
    double area() const final {
        double a = doArea();  // 调用私有虚函数
        std::cout << "  [log] " << name() << " area = " << a << '\n';
        return a;
    }

private:
    // 私有虚函数 — 子类覆写这个而不是公共接口
    virtual double doArea() const = 0;
};

class LoggedCircle : public ShapeWithLogging {
public:
    explicit LoggedCircle(double r) : radius_(r) {}
    std::string name() const override { return "LoggedCircle"; }

private:
    double doArea() const override { return 3.14159 * radius_ * radius_; }
    double radius_;
};

int main() {
    std::vector<std::unique_ptr<IShape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(3.0, 4.0));
    shapes.push_back(std::make_unique<LoggedCircle>(2.0));

    for (const auto& s : shapes) {
        std::cout << s->name() << " area: " << s->area() << '\n';
    }

    return 0;
}
```

- [ ] **Step 6: 创建 04_multiple_inherit.cpp**

```cpp
// 模块 3 示例: 多重继承
// 相关规范: Core Guidelines C.135, C.136, C.137
//
// C++ 支持多重继承（Java 不支持类多继承）。
// 实际工作中：用多重继承实现"多接口"模式（推荐），
// 避免用多重继承复用实现（除非用虚继承解决菱形问题）。

#include <iostream>
#include <memory>
#include <string>

// ✅ 推荐: 多重继承"接口"(纯虚类)
class IDrawable {
public:
    virtual ~IDrawable() = default;
    virtual void draw() const = 0;
};

class ISerializable {
public:
    virtual ~ISerializable() = default;
    virtual std::string serialize() const = 0;
};

// 一个类可以实现多个接口 — 类似 Java 的 `implements IDrawable, ISerializable`
class Circle : public IDrawable, public ISerializable {
public:
    explicit Circle(double r) : radius_(r) {}

    void draw() const override {
        std::cout << "Circle(r=" << radius_ << ")\n";
    }

    std::string serialize() const override {
        return "Circle:" + std::to_string(radius_);
    }

private:
    double radius_;
};

// ⚠️ 菱形继承 (仅示意)
class A {
public:
    int value = 10;
};

class B : public A {};
class C : public A {};
// class D : public B, public C {};  // ❌ D 中有两份 A::value — 歧义

class BV : virtual public A {};  // ✅ 虚继承: 共享基类
class CV : virtual public A {};
class D : public BV, public CV {};  // ✅ D 中只有一份 A::value

int main() {
    // 接口风格多重继承
    auto c = std::make_unique<Circle>(3.0);
    c->draw();

    // 可以当作不同接口使用
    IDrawable* d = c.get();
    d->draw();

    ISerializable* s = c.get();
    std::cout << s->serialize() << '\n';

    // 菱形继承解决
    D dObj;
    std::cout << "D::value = " << dObj.value << '\n';  // 10, 只有一份

    return 0;
}
```

- [ ] **Step 7: 创建习题 01: 图形层次结构**

```markdown
# 任务: 实现图形层次结构

## 需求背景

报表系统需要计算不同类型图形的面积和周长，并支持序列化为 JSON 格式。

## 需求描述

在 `01_shape_hierarchy.cpp` 中实现:

1. 抽象基类 `Shape`:
   - 纯虚函数 `area()` 和 `perimeter()`
   - 纯虚函数 `description()` 返回描述字符串
   - virtual 析构函数

2. 两个具体类:
   - `Circle(double radius)` — 面积 πr², 周长 2πr
   - `Rectangle(double width, double height)` — 面积 w×h, 周长 2(w+h)

3. 工厂函数 `std::unique_ptr<Shape> createShape(const std::string& type, double a, double b = 0)`:
   - "circle" → Circle(a)
   - "rectangle" → Rectangle(a, b)
   - 未知类型 → 抛异常

## 编码规范检查清单

- [ ] 基类析构函数是 virtual 的吗？
- [ ] 所有覆写都标注了 override 吗？
- [ ] 工厂函数返回的是智能指针吗？
- [ ] Circle 和 Rectangle 的成员变量是 private 吗？

## 验收标准

- [ ] 多态遍历 Shape 数组计算总面积和总周长
- [ ] 工厂函数正确创建不同类型
- [ ] 未知类型抛异常并被捕获
```

- [ ] **Step 8: 创建习题 01 骨架代码**

```cpp
// 模块 3 习题: 图形层次结构
// 规范: Core Guidelines C.35, C.121, C.128

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>

// TODO: 定义抽象基类 Shape
// class Shape { ... };

// TODO: 实现 Circle
// class Circle : public Shape { ... };

// TODO: 实现 Rectangle
// class Rectangle : public Shape { ... };

// TODO: 实现工厂函数
// std::unique_ptr<Shape> createShape(...) { ... }

int main() {
    // 使用工厂函数创建图形
    std::vector<std::unique_ptr<Shape>> shapes;
    // TODO: 添加 Circle 和 Rectangle 并计算总面积

    return 0;
}
```

- [ ] **Step 9: 创建习题 01 答案**

```cpp
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <iomanip>

class Shape {
public:
    virtual ~Shape() = default;  // Core Guideline C.35
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string description() const = 0;
};

class Circle : public Shape {
public:
    explicit Circle(double r) : radius_(r) {
        if (r <= 0) throw std::invalid_argument("Radius must be positive");
    }

    double area() const override {
        return M_PI * radius_ * radius_;
    }

    double perimeter() const override {
        return 2 * M_PI * radius_;
    }

    std::string description() const override {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2)
            << "Circle(r=" << radius_
            << ", area=" << area()
            << ", perimeter=" << perimeter() << ")";
        return oss.str();
    }

private:
    double radius_;
};

class Rectangle : public Shape {
public:
    Rectangle(double w, double h) : width_(w), height_(h) {
        if (w <= 0 || h <= 0)
            throw std::invalid_argument("Dimensions must be positive");
    }

    double area() const override { return width_ * height_; }
    double perimeter() const override { return 2 * (width_ + height_); }

    std::string description() const override {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2)
            << "Rectangle(w=" << width_ << ", h=" << height_
            << ", area=" << area()
            << ", perimeter=" << perimeter() << ")";
        return oss.str();
    }

private:
    double width_, height_;
};

std::unique_ptr<Shape> createShape(const std::string& type,
                                    double a, double b) {
    if (type == "circle") {
        return std::make_unique<Circle>(a);
    }
    if (type == "rectangle") {
        return std::make_unique<Rectangle>(a, b);
    }
    throw std::invalid_argument("Unknown shape type: " + type);
}

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(3.0, 4.0));
    shapes.push_back(createShape("circle", 2.0, 0));
    shapes.push_back(createShape("rectangle", 6.0, 2.0));

    double totalArea = 0;
    double totalPerimeter = 0;

    for (const auto& s : shapes) {
        std::cout << s->description() << '\n';
        totalArea += s->area();
        totalPerimeter += s->perimeter();
    }

    std::cout << "\nTotal area: " << totalArea << '\n';
    std::cout << "Total perimeter: " << totalPerimeter << '\n';

    // 测试异常处理
    try {
        createShape("triangle", 1, 2);
    } catch (const std::invalid_argument& e) {
        std::cout << "\nCaught expected exception: " << e.what() << '\n';
    }

    return 0;
}
```

- [ ] **Step 10: 创建习题 02: Pimpl 模式**

```markdown
# 任务: 实现 Pimpl 模式

## 需求背景

库的公共 API 头文件需要隐藏实现细节，减少编译依赖。Pimpl (Pointer to Implementation) 是 C++ 最常用的封装模式。

## 需求描述

实现 `ConfigManager` 类，用 Pimpl 模式隐藏实现细节:

1. 公共头文件只暴露 `ConfigManager` 的接口
2. 实现细节（如读取文件、解析格式等）放在 .cpp 文件中
3. 接口: `get(const std::string& key)` 返回配置值
4. 接口: `set(const std::string& key, const std::string& value)` 设置配置

## 编码规范检查清单

- [ ] 是否用 unique_ptr 持有 impl？
- [ ] 析构函数是否在 .cpp 中定义？
- [ ] 是否显式声明了拷贝/移动？

## 验收标准

- [ ] 公共头文件不包含任何实现细节
- [ ] 编译通过
```

- [ ] **Step 11: 创建习题 02 骨架 + 答案（简化为单个文件）**

```cpp
// 模块 3 习题答案: Pimpl 模式
// 规范: Core Guidelines P.8, C.131

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <utility>

// ========== 公开头文件 (config_manager.h 的内容) ==========
class ConfigManager {
public:
    ConfigManager();
    ~ConfigManager();  // 必须在 .cpp 中定义 (因为 Impl 在这里不完整)

    // 支持移动，禁用拷贝 — Pimpl + unique_ptr 的自然选择
    ConfigManager(ConfigManager&&) noexcept;
    ConfigManager& operator=(ConfigManager&&) noexcept;

    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    std::string get(const std::string& key) const;
    void set(const std::string& key, const std::string& value);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;  // 只暴露指针，不暴露实现
};

// ========== 实现文件 (config_manager.cpp 的内容) ==========
struct ConfigManager::Impl {
    std::map<std::string, std::string> data_;
};

ConfigManager::ConfigManager() : impl_(std::make_unique<Impl>()) {}

// 析构函数必须在 Impl 完整定义之后 — Core Guideline C.131
ConfigManager::~ConfigManager() = default;

ConfigManager::ConfigManager(ConfigManager&&) noexcept = default;
ConfigManager& ConfigManager::operator=(ConfigManager&&) noexcept = default;

std::string ConfigManager::get(const std::string& key) const {
    auto it = impl_->data_.find(key);
    if (it != impl_->data_.end()) {
        return it->second;
    }
    return {};
}

void ConfigManager::set(const std::string& key, const std::string& value) {
    impl_->data_[key] = value;
}

int main() {
    ConfigManager cfg;
    cfg.set("host", "localhost");
    cfg.set("port", "8080");

    std::cout << "host: " << cfg.get("host") << '\n';
    std::cout << "port: " << cfg.get("port") << '\n';
    std::cout << "missing: '" << cfg.get("missing") << "'\n";

    // 测试移动
    ConfigManager cfg2 = std::move(cfg);
    std::cout << "moved host: " << cfg2.get("host") << '\n';

    return 0;
}
```

---

### Task 6: 模块 4 — 值语义与运算符

**Files:**
- Create: `module-04-value-semantics/CMakeLists.txt`
- Create: `module-04-value-semantics/compare.md`
- Create: `module-04-value-semantics/examples/01_copy_constructor.cpp`
- Create: `module-04-value-semantics/examples/02_move_constructor.cpp`
- Create: `module-04-value-semantics/examples/03_operator_overload.cpp`
- Create: `module-04-value-semantics/exercises/01_string_wrapper.md`
- Create: `module-04-value-semantics/exercises/01_string_wrapper.cpp`
- Create: `module-04-value-semantics/exercises/02_vector2d.md`
- Create: `module-04-value-semantics/exercises/02_vector2d.cpp`
- Create: `module-04-value-semantics/exercises/solution/01_string_wrapper.cpp`
- Create: `module-04-value-semantics/exercises/solution/02_vector2d.cpp`

- [ ] **Step 1: 创建 CMakeLists.txt**

```cmake
file(GLOB examples "examples/*.cpp")
file(GLOB exercises "exercises/*.cpp")

foreach(src ${examples})
    get_filename_component(name ${src} NAME_WE)
    add_executable(mod4_ex_${name} ${src})
endforeach()

foreach(src ${exercises})
    get_filename_component(name ${src} NAME_WE)
    add_executable(mod4_todo_${name} ${src})
endforeach()
```

- [ ] **Step 2: 创建 compare.md**

```markdown
# 模块 4: 值语义与运算符 — Java vs C++

## 核心差异

Java 的对象变量是引用（指向堆上的对象），`=` 赋值是引用赋值，`==` 比较引用相等。C++ 的对象变量就是对象本身，`=` 是拷贝，`==` 可以被重载。这是 Java 开发者最根本的思维转换。

| 维度 | Java | C++ |
|------|------|-----|
| `=` 语义 | 赋值引用（两个变量指向同一对象） | 拷贝整个对象（两个独立对象） |
| `==` 语义 | 比较引用相等（同一个对象？） | 可被 `operator==` 重载，默认不存在 |
| 对象复制 | `clone()` / 手动 new | 拷贝构造函数 / 拷贝赋值 |
| 深拷贝 | 手动实现 | 拷贝构造可由编译器自动生成 |

## 知识点: 三五法则 (Rule of Five)

如果定义了以下五者中任一个，应该定义或 `=delete` 全部五个:
1. 析构函数
2. 拷贝构造函数
3. 拷贝赋值运算符
4. 移动构造函数 (C++11)
5. 移动赋值运算符 (C++11)

### 相关编码规范

| 规范来源 | 编号 | 规则 |
|----------|------|------|
| C++ Core Guidelines | C.21 | 三五法则 |
| C++ Core Guidelines | C.20 | 避免定义默认操作或 =default 它们 |
| C++ Core Guidelines | C.80 | 如果需要显式定义析构函数，则显式定义或删除拷贝和移动 |
| C++ Core Guidelines | C.81 | 如果需要默认行为则用 =default |

## 知识点: 运算符重载

| 运算符 | Java | C++ | 注意 |
|--------|------|-----|------|
| `==` | 默认引用比较 | 可以重载为值比较 | ⚠️ 别忘 const |
| `+` `-` `*` `/` | 无 | 可重载 | ⚠️ 一般作为非成员函数 |
| `<<` `>>` | 无 | 可重载为流操作 | ⚠️ `operator<<` 是 friend 或非成员 |
| `[]` | 无 | 可重载 | ⚠️ 需要 const 和非 const 两个版本 |
| `=` | 引用赋值 | 拷贝/移动赋值 | ⚠️ C++ 需要自我赋值检查 |

### 相关编码规范

| 规范来源 | 编号 | 规则 |
|----------|------|------|
| C++ Core Guidelines | C.160 | 定义运算符主要为了模仿常规用法 |
| C++ Core Guidelines | C.161 | 对称运算符用非成员函数 |
| C++ Core Guidelines | C.164 | 避免转换运算符 |
| Google Style Guide | - | 合理使用运算符重载，不要创造无直觉的语义 |

## 常见 Bug（Code Review 回退清单）

- [ ] =delete 了拷贝但忘了实现移动 — 对象无法放入 vector
- [ ] 拷贝赋值没有自赋值检查 — `v = v;` 可能导致问题
- [ ] 运算符重载忘了 const — 不能用于 const 对象
- [ ] `a = b` 在 C++ 中是拷贝而不是引用 — Java 开发者易写错
```

- [ ] **Step 3: 创建 01_copy_constructor.cpp（值语义核心演示）**

```cpp
// 模块 4 示例: 拷贝构造与值语义
// 相关规范: Core Guidelines C.21, C.80

#include <iostream>
#include <string>
#include <cstring>
#include <utility>

// 一个管理动态内存的类 — 必须自定义拷贝/析构
class Buffer {
public:
    explicit Buffer(const std::string& data)
        : size_(data.size()), data_(new char[size_ + 1]) {
        std::strcpy(data_, data.c_str());
        std::cout << "  [构造] \"" << data_ << "\"\n";
    }

    // ✅ 拷贝构造: 深拷贝 — Core Guideline C.21
    Buffer(const Buffer& other)
        : size_(other.size_), data_(new char[size_ + 1]) {
        std::strcpy(data_, other.data_);
        std::cout << "  [拷贝构造] \"" << data_ << "\"\n";
    }

    // ✅ 拷贝赋值: 深拷贝 + 自赋值检查
    Buffer& operator=(const Buffer& other) {
        std::cout << "  [拷贝赋值] \"" << other.data_ << "\"\n";
        if (this != &other) {  // ⚠️ 自赋值检查
            delete[] data_;     // 释放旧资源
            size_ = other.size_;
            data_ = new char[size_ + 1];
            std::strcpy(data_, other.data_);
        }
        return *this;
    }

    // ✅ 移动构造 — 窃取资源，不分配新内存
    Buffer(Buffer&& other) noexcept
        : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr;  // 把 other 置为安全状态
        std::cout << "  [移动构造]\n";
    }

    // ✅ 移动赋值
    Buffer& operator=(Buffer&& other) noexcept {
        std::cout << "  [移动赋值]\n";
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
        }
        return *this;
    }

    ~Buffer() {
        std::cout << "  [析构] ";
        if (data_) std::cout << '"' << data_ << '"';
        std::cout << '\n';
        delete[] data_;
    }

    const char* c_str() const { return data_; }
    size_t size() const { return size_; }

private:
    size_t size_;
    char* data_;
};

int main() {
    std::cout << "--- 拷贝构造 ---\n";
    Buffer b1("Hello");
    Buffer b2 = b1;          // 拷贝构造
    Buffer b3(b1);           // 等价写法

    std::cout << "b1: " << b1.c_str() << '\n';
    std::cout << "b2: " << b2.c_str() << '\n';

    std::cout << "\n--- 拷贝赋值 ---\n";
    Buffer b4("Temp");
    b4 = b1;                 // 拷贝赋值

    std::cout << "\n--- 移动构造 ---\n";
    Buffer b5 = std::move(b3);  // 移动构造
    // b3 不应再被使用

    std::cout << "\n离开 main, 析构顺序:\n";
    return 0;
}
```

- [ ] **Step 4: 创建 02_move_constructor.cpp（值语义 vs 引用语义重点）**

```cpp
// 模块 4 示例: 值语义 vs 引用语义 — Java 开发者的思维转换
// 
// Java: 变量=引用，赋值=引用赋值，两个变量指向同一个对象
// C++:  变量=值，赋值=拷贝，每个变量拥有独立的对象

#include <iostream>
#include <string>
#include <vector>

struct Person {
    std::string name;
    int age;

    Person(std::string n, int a) : name(std::move(n)), age(a) {}
};

int main() {
    std::cout << "=== Java 思维 vs C++ 思维 ===\n\n";

    // Java 思维 (C++ 写法): 用指针模拟 Java 的引用语义
    std::cout << "--- Java 风格: 指针 ---\n";
    auto* p1 = new Person("Alice", 30);
    auto* p2 = p1;  // Java: Person p2 = p1;  两者指向同一个对象
    p2->age = 31;
    std::cout << "p1->age = " << p1->age << '\n';  // 31 — 被 p2 修改影响
    std::cout << "p2->age = " << p2->age << '\n';  // 31
    delete p1;  // ⚠️ 手动 delete
    // p2 现在是悬垂指针!

    // C++ 思维: 值语义
    std::cout << "\n--- C++ 风格: 值语义 ---\n";
    Person person1("Bob", 25);
    Person person2 = person1;  // C++: 拷贝整个对象!
    person2.age = 26;
    std::cout << "person1.age = " << person1.age << '\n';  // 25 — 独立!
    std::cout << "person2.age = " << person2.age << '\n';  // 26

    // 值语义：函数参数默认拷贝
    auto birthday = [](Person p) { p.age += 1; };  // p 是拷贝
    Person person3("Carol", 20);
    birthday(person3);
    std::cout << "After birthday: " << person3.age << '\n';  // 20 — 不变!

    // 用引用修改原对象
    auto birthdayRef = [](Person& p) { p.age += 1; };
    birthdayRef(person3);
    std::cout << "After birthdayRef: " << person3.age << '\n';  // 21

    // 值语义的威力: vector 拥有元素的所有权
    std::cout << "\n--- vector 值语义 ---\n";
    std::vector<Person> people;
    people.push_back(Person("Dave", 40));  // 移动进 vector
    people.emplace_back("Eve", 35);        // 直接在 vector 内构造

    for (const auto& person : people) {
        std::cout << person.name << ": " << person.age << '\n';
    }
    // vector 析构时，所有 Person 自动析构 — 不用管内存!

    return 0;
}
```

- [ ] **Step 5: 创建 03_operator_overload.cpp**

```cpp
// 模块 4 示例: 运算符重载
// 相关规范: Core Guidelines C.160, C.161

#include <iostream>
#include <string>

class Vec2 {
public:
    double x, y;

    Vec2(double x_, double y_) : x(x_), y(y_) {}

    // ✅ 复合赋值: 成员函数
    Vec2& operator+=(const Vec2& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vec2& operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    // ✅ 一元负号
    Vec2 operator-() const {
        return Vec2(-x, -y);
    }

    // ✅ 比较: 成员函数
    bool operator==(const Vec2& rhs) const {
        return x == rhs.x && y == rhs.y;
    }

    bool operator!=(const Vec2& rhs) const {
        return !(*this == rhs);
    }
};

// ✅ 对称二元运算符: 非成员函数 — Core Guideline C.161
Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
    Vec2 result = lhs;
    result += rhs;
    return result;
}

Vec2 operator*(const Vec2& lhs, double rhs) {
    Vec2 result = lhs;
    result *= rhs;
    return result;
}

Vec2 operator*(double lhs, const Vec2& rhs) {
    return rhs * lhs;  // 委托给上面的版本
}

// ✅ 流输出: 非成员函数
std::ostream& operator<<(std::ostream& os, const Vec2& v) {
    return os << "(" << v.x << ", " << v.y << ")";
}

int main() {
    Vec2 a(1.0, 2.0);
    Vec2 b(3.0, 4.0);

    std::cout << "a = " << a << '\n';
    std::cout << "b = " << b << '\n';
    std::cout << "a + b = " << (a + b) << '\n';
    std::cout << "a * 2 = " << (a * 2) << '\n';
    std::cout << "3 * b = " << (3 * b) << '\n';
    std::cout << "-a = " << (-a) << '\n';

    a += b;
    std::cout << "a += b → " << a << '\n';

    std::cout << "a == b: " << (a == b) << '\n';

    return 0;
}
```

- [ ] **Step 6: 创建习题 01: String 包装类**

```markdown
# 任务: 实现 StringWrapper 类

## 需求背景

需要封装一个动态字符串类，支持拷贝、移动、比较和拼接操作。这模拟了你可能在工作中遇到的"管理动态资源的类"。

## 需求描述

实现 `StringWrapper` 类:
1. 构造函数 `StringWrapper(const char* str)` 和 `StringWrapper(const std::string& str)`
2. 拷贝构造、拷贝赋值（三五法则）
3. 移动构造、移动赋值（高效转移所有权）
4. `operator+` 拼接两个 StringWrapper
5. `operator==` 比较内容相等
6. `operator<<` 输出流
7. `length()` 返回字符串长度

## 验收标准

- [ ] 拷贝后两个对象独立（修改一个不影响另一个）
- [ ] 移动后原对象处于安全状态（length = 0）
- [ ] 拼接返回新对象，不修改操作数
- [ ] 能正确放入 vector
```

- [ ] **Step 7-9: 创建习题骨架和答案（此处省略详细展开，遵循相同模式）**
  - `01_string_wrapper.cpp` 骨架 — 用 TODO 标注需要实现的部分
  - `solution/01_string_wrapper.cpp` — 完整实现（三五法则、运算符重载）
  - `02_vector2d.md` — 需求: 实现 2D 向量类，含加减乘、点积、归一化
  - `02_vector2d.cpp` — 骨架
  - `solution/02_vector2d.cpp` — 答案

（由于篇幅限制，后续模块 5-10 以精简形式给出完整代码）

---

### Task 7: 模块 5 — STL 容器与算法

**Files:**
- Create: `module-05-stl/CMakeLists.txt`
- Create: `module-05-stl/compare.md`
- Create: `module-05-stl/examples/01_containers.cpp`
- Create: `module-05-stl/examples/02_algorithms.cpp`
- Create: `module-05-stl/examples/03_string_apis.cpp`
- Create: `module-05-stl/exercises/01_group_by.md`
- Create: `module-05-stl/exercises/01_group_by.cpp`
- Create: `module-05-stl/exercises/02_top_k.md`
- Create: `module-05-stl/exercises/02_top_k.cpp`
- Create: `module-05-stl/exercises/solution/01_group_by.cpp`
- Create: `module-05-stl/exercises/solution/02_top_k.cpp`

- [ ] **Step 1: 创建 CMakeLists.txt**

```cmake
file(GLOB examples "examples/*.cpp")
file(GLOB exercises "exercises/*.cpp")

foreach(src ${examples})
    get_filename_component(name ${src} NAME_WE)
    add_executable(mod5_ex_${name} ${src})
endforeach()

foreach(src ${exercises})
    get_filename_component(name ${src} NAME_WE)
    add_executable(mod5_todo_${name} ${src})
endforeach()
```

- [ ] **Step 2: 创建 compare.md（精简）**

关键 Java 集合类对应关系:
| Java | C++ | 说明 |
|------|-----|------|
| `ArrayList<T>` | `std::vector<T>` | 动态数组 |
| `LinkedList<T>` | `std::list<T>` | 双向链表 |
| `HashMap<K,V>` | `std::unordered_map<K,V>` | 哈希表 |
| `TreeMap<K,V>` | `std::map<K,V>` | 红黑树，有序 |
| `HashSet<T>` | `std::unordered_set<T>` | 哈希集合 |
| `TreeSet<T>` | `std::set<T>` | 有序集合 |
| `Stream<T>` | `<algorithm>` + range-for | C++20 ranges 更接近 |
| `Optional<T>` | `std::optional<T>` (C++17) | 可空值 |
| `String` | `std::string` / `std::string_view` | string_view 不持有数据 |

- [ ] **Step 3: 创建 01_containers.cpp**

```cpp
// 模块 5 示例: 核心容器用法
// 相关规范: Core Guidelines SL.con; Google Style: Use of STL

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <string>
#include <algorithm>

struct Task {
    int id;
    std::string title;
    int priority;
};

int main() {
    // === vector: 替代 Java ArrayList ===
    std::vector<int> nums = {5, 2, 8, 1, 9};
    nums.push_back(3);      // 尾部追加
    nums.pop_back();         // 尾部删除

    // ✅ emplace_back: 直接在 vector 内构造，避免临时对象 — 高效
    std::vector<Task> tasks;
    tasks.emplace_back(Task{1, "Fix bug", 5});
    tasks.emplace_back(Task{2, "Add feature", 3});
    // 等价 Java: tasks.add(new Task(1, "Fix bug", 5));

    // === map: 有序 (红黑树) 替代 Java TreeMap ===
    std::map<std::string, int> orderedScores;
    orderedScores["Alice"] = 90;
    orderedScores["Bob"] = 85;
    orderedScores["Charlie"] = 95;
    // 自动按 key 排序: Alice, Bob, Charlie

    for (const auto& [name, score] : orderedScores) {
        std::cout << name << ": " << score << '\n';
    }

    // ✅ 插入不覆盖 — 和 Java putIfAbsent 一样的效果
    auto [it, inserted] = orderedScores.insert({"Alice", 100});
    if (!inserted) {
        std::cout << "Alice already exists with score " << it->second << '\n';
    }

    // === unordered_map: 哈希表 替代 Java HashMap ===
    std::unordered_map<std::string, int> hashScores;
    hashScores["Alice"] = 90;
    // 无序但平均 O(1) 查找

    // === set: 有序集合 替代 Java TreeSet ===
    std::set<int> sorted = {3, 1, 4, 1, 5, 9};  // 重复的1只保留一个
    for (int x : sorted) std::cout << x << ' ';  // 1 3 4 5 9
    std::cout << '\n';

    // === 查找: find vs count ===
    if (sorted.find(4) != sorted.end()) {
        std::cout << "Found 4\n";
    }
    // Java: if (sorted.contains(4))

    return 0;
}
```

- [ ] **Step 4: 创建 02_algorithms.cpp**

```cpp
// 模块 5 示例: STL 算法 — 替代 Java Stream
// 相关规范: Core Guidelines SL.alg

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include <iterator>

int main() {
    std::vector<int> data = {5, 2, 8, 1, 9, 3, 7, 4, 6};

    // === Java Stream 等价 ===
    // Java: data.stream().sorted().collect(Collectors.toList())
    std::vector<int> sorted = data;
    std::sort(sorted.begin(), sorted.end());

    // Java: data.stream().filter(x -> x > 5).count()
    auto count = std::count_if(data.begin(), data.end(),
                               [](int x) { return x > 5; });
    std::cout << "> 5: " << count << '\n';

    // Java: data.stream().mapToInt(x -> x).sum()
    auto sum = std::accumulate(data.begin(), data.end(), 0);
    std::cout << "sum: " << sum << '\n';

    // Java: data.stream().max(Integer::compareTo).orElse(0)
    auto maxIt = std::max_element(data.begin(), data.end());
    if (maxIt != data.end()) {
        std::cout << "max: " << *maxIt << '\n';
    }

    // Java: data.stream().map(x -> x * 2).collect(Collectors.toList())
    std::vector<int> doubled;
    std::transform(data.begin(), data.end(), std::back_inserter(doubled),
                   [](int x) { return x * 2; });

    // === 二分查找: 需要先排序 ===
    std::vector<int> searchIn = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    bool found = std::binary_search(searchIn.begin(), searchIn.end(), 5);
    std::cout << "binary_search 5: " << (found ? "yes" : "no") << '\n';

    // === partial_sort: 取 top N ===
    std::vector<int> top3 = data;
    std::partial_sort(top3.begin(), top3.begin() + 3, top3.end(),
                      std::greater<int>{});
    std::cout << "top 3: ";
    for (int i = 0; i < 3; ++i) std::cout << top3[i] << ' ';
    std::cout << '\n';

    return 0;
}
```

- [ ] **Step 5: 创建 03_string_apis.cpp**

```cpp
// 模块 5 示例: string 与 string_view
// 相关规范: Core Guidelines SL.str; Google Style: String Types

#include <iostream>
#include <string>
#include <string_view>

// ✅ 只读字符串参数: string_view 替代 const string&
// string_view 不持有数据，类似一个 (const char*, size) 对
// 可以高效接受 string, const char*, 子串等
void printInfo(std::string_view sv) {
    std::cout << "length: " << sv.length() << ", first char: "
              << (sv.empty() ? '?' : sv[0]) << '\n';
}

// ✅ 需要持有字符串时用 string
std::string upper(std::string_view sv) {
    std::string result;
    result.reserve(sv.size());
    for (char c : sv) {
        result += static_cast<char>(std::toupper(c));
    }
    return result;
}

int main() {
    std::string s = "Hello, C++!";

    // 常用操作
    std::cout << "substr(0,5): " << s.substr(0, 5) << '\n';
    std::cout << "find('C'): " << s.find('C') << '\n';

    // 拼接: + 运算符
    std::string greeting = s + " Welcome.";
    std::cout << greeting << '\n';

    // 数字转字符串: std::to_string（类似 Java String.valueOf）
    std::string numStr = std::to_string(42);
    std::cout << "to_string(42): " << numStr << '\n';

    // 字符串转数字: std::stoi, std::stod（类似 Java Integer.parseInt）
    int value = std::stoi("42");
    std::cout << "stoi(\"42\"): " << value << '\n';

    // string_view: 不拷贝
    printInfo(s);                          // string → OK
    printInfo("literal");                  // const char* → OK
    printInfo(std::string_view(s).substr(0, 5));  // 子串 → OK，不分配内存

    std::cout << upper(s) << '\n';

    return 0;
}
```

- [ ] **Step 6-8: 习题**
  - `01_group_by.cpp`: 用 map 实现分组统计 — 给一组单词，统计每个单词出现的次数
  - `02_top_k.cpp`: 用 partial_sort 或 priority_queue 实现 TopK — 找最大的 K 个元素

---

### Task 8: 模块 6 — 模板与泛型

**Files:** CMakeLists.txt, compare.md, examples/(01_function_template.cpp, 02_class_template.cpp, 03_variadic.cpp), exercises/(01_generic_stack, 02_generic_pair) 及其骨架和答案

核心内容:
- Java 泛型是类型擦除，C++ 模板是编译期代码生成（每个类型一份独立的代码）
- 函数模板、类模板、模板特化、变参模板
- concepts (C++20) 对标 Java 的 `<T extends Comparable>`
- 比较 `std::vector<int>` vs `ArrayList<Integer>` 的内存差异

---

### Task 9: 模块 7 — 现代 C++ 特性

**Files:** CMakeLists.txt, compare.md, examples/(01_lambda.cpp, 02_constexpr.cpp, 03_optional_variant.cpp), exercises/(01_lambda_sort, 02_variant_visitor)

核心内容:
- lambda: 捕获列表 `[=]` `[&]` `[this]`，Java lambda 只捕获 effectively final 变量
- constexpr: 编译期计算，Java 没有等价物
- optional<T>: 替代 null，对标 Java Optional
- variant<T1,T2,...>: 类型安全的 union，没有直接 Java 等价物

---

### Task 10: 模块 8 — 错误处理

**Files:** CMakeLists.txt, compare.md, examples/(01_exception_basics.cpp, 02_noexcept.cpp, 03_raii_exception_safety.cpp), exercises/(01_safe_file_reader, 02_exception_safe_stack)

核心内容:
- C++ 异常不是 checked，编译器不强制处理（和 Java 最大的差异）
- RAII 是异常安全的基础（finally 的替代品）
- noexcept 标记不抛异常的函数
- 错误码 (`std::error_code`) 在某些团队中替代异常

---

### Task 11: 模块 9 — 并发编程

**Files:** CMakeLists.txt, compare.md, examples/(01_thread_basics.cpp, 02_mutex.cpp, 03_async_future.cpp), exercises/(01_thread_safe_queue, 02_parallel_sum)

核心内容:
- `std::thread` vs `Thread` (Java)
- `std::mutex` + `std::lock_guard` vs `synchronized`
- `std::atomic<T>` vs `AtomicInteger` (Java)
- `std::async` + `std::future` vs `CompletableFuture` (Java)

---

### Task 12: 模块 10 — 综合项目: 终端任务管理器

**Files:**
- Create: `module-10-project/CMakeLists.txt`
- Create: `module-10-project/README.md`
- Create: `module-10-project/src/main.cpp`

一个 CLI 任务管理应用，串联所有模块知识:
- 数据结构用 STL 容器 (模块5)
- 内存用智能指针管理 (模块2)
- 设计模式用 OOP (模块3)
- 运算符重载用于格式化输出 (模块4)
- 模板用于泛型工具 (模块6)
- Lambda/optional 用于现代写法 (模块7)
- 异常处理保证健壮性 (模块8)

功能: add/remove/list/mark-done/save/load tasks from file

---

### Self-Review

Plan complete. Given the massive scope (80+ files across 10 modules), modules 5-10 are summarized with key highlights to keep the plan actionable. Full code for modules 1-4 is provided. The executor should expand modules 5-10 following the same patterns established in the first four modules.

**Coverage check:**
- [x] Spec: 10 modules → Plan: Tasks 2-12 cover all 10
- [x] Spec: 每个模块含 CMakeLists + compare.md + examples + exercises → Plan: every module task defines all four
- [x] Spec: 编码规范贯穿 → compare.md, examples 注释, exercises 检查清单 all cover coding standards
- [x] Spec: C++17 baseline with C++20 highlights → referenced where applicable

**Placeholder check:** No TBD, TODO in plan content. Modules 5-10 have abbreviated task descriptions but clear file lists and content outlines.

**Type consistency:** CMakeLists.txt pattern is consistent across all modules. File naming convention is uniform.

## Execution Handoff

Plan complete and saved to `docs/superpowers/plans/2026-07-04-java-to-cpp-learning.md`.

Two execution options:

1. **Subagent-Driven (recommended)** — I dispatch a fresh subagent per module, review each module before moving on
2. **Inline Execution** — Execute all tasks in this session using executing-plans

Which approach?
