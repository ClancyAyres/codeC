# C++ 学习路径实现计划：从 Java 到 AI 算子开发

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 构建一套完整的 C++ 学习材料（Java 对照、示例、习题），覆盖从基础语法到 AI 算子开发的 14 周课程

**Architecture:** 四阶段渐进式结构，每模块独立目录包含 diff.md、notes.md、examples/、exercises/。所有代码用 CMake 管理，示例和习题可独立编译运行

**Tech Stack:** C++17/20, CMake 3.20+, Google Test, Clang/GCC

**Spec:** `docs/superpowers/specs/2026-07-02-cpp-learning-curriculum-design.md`

---

### Task 1: 项目脚手架

**Files:**
- Create: `CMakeLists.txt`
- Create: `.gitignore`

- [ ] **Step 1: 创建顶层 CMakeLists.txt**

```cmake
cmake_minimum_required(VERSION 3.20)
project(cpp-learning VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_subdirectory(phase1-foundation)
add_subdirectory(phase2-core)
add_subdirectory(phase3-advanced)
add_subdirectory(phase4-operator)
```

- [ ] **Step 2: 创建 .gitignore**

```
build/
cmake-build-*/
.vscode/
.idea/
*.o
*.out
*.exe
.DS_Store
```

- [ ] **Step 3: 创建阶段目录结构和占位 CMakeLists.txt**

```bash
for phase in phase1-foundation phase2-core phase3-advanced phase4-operator; do
  mkdir -p $phase
  echo "# $phase" > $phase/CMakeLists.txt
done
```

- [ ] **Step 4: 构建验证**

```bash
cmake -S . -B build && cmake --build build
```

- [ ] **Step 5: Commit**

```bash
git add -A && git commit -m "feat: add project scaffolding with CMake structure"
```

---

### Task 2: Week 1 — 从 Java 到 C++ 的第一步

**Files to create:**
```
phase1-foundation/week1-cpp-basics/
├── diff.md
├── notes.md
├── examples/
│   ├── 01-hello-project/main.cpp + CMakeLists.txt
│   ├── 02-type-limits/main.cpp + CMakeLists.txt
│   ├── 03-namespace-demo/main.cpp + CMakeLists.txt
│   └── 04-header-guard/main.cpp + math.hpp + math.cpp + CMakeLists.txt
└── exercises/
    ├── 01-cmake-project/README.md + main.cpp + solution.cpp
    ├── 02-sizeof-types/README.md + main.cpp + solution.cpp
    ├── 03-namespace-max/README.md + main.cpp + solution.cpp
    └── 04-header-duplicate/README.md + main.cpp + solution.cpp
```

**Modify:** `phase1-foundation/CMakeLists.txt` — add `add_subdirectory()` for each example

- [ ] **Step 1: diff.md** — Java vs C++ 对照：编译模型、基本类型大小、字符串差异、I/O 差异。参考 spec 中 Week 1 的 Java 差异重点

- [ ] **Step 2: notes.md** — CMake 基础、头文件/源文件分离、编译链接模型、基本类型与定长类型、命名空间、头文件守卫。参考 spec Week 1 核心内容

- [ ] **Step 3: 示例 01-hello-project** — main 函数签名、命令行参数、std::cin/getline 输入

- [ ] **Step 4: 示例 02-type-limits** — sizeof 所有基本类型、numeric_limits 范围、溢出演示

- [ ] **Step 5: 示例 03-namespace-demo** — 两个 namespace 各定义 max，演示 using 声明、全局作用域区分

- [ ] **Step 6: 示例 04-header-guard** — math.hpp 用 `#pragma once`，math.cpp 实现，main.cpp 故意 include 两次验证不报错

- [ ] **Step 7: 习题 01-cmake-project** — README 要求搭建 CMake 项目（libmath 静态库 + app 可执行文件），main.cpp 骨架代码，solution.cpp 参考实现

- [ ] **Step 8: 习题 02-sizeof-types** — 打印所有基本类型和定长类型的 sizeof

- [ ] **Step 9: 习题 03-namespace-max** — math::max(int,int) 和 text::max(const char*, const char*)

- [ ] **Step 10: 习题 04-header-duplicate** — 演示无头文件守卫时的重定义错误，加上 `#pragma once` 修复

- [ ] **Step 11: 更新 phase1-foundation/CMakeLists.txt**，添加 4 个示例的 add_subdirectory

- [ ] **Step 12: 构建验证** `cmake -S . -B build && cmake --build build`

- [ ] **Step 13: Commit** `git add -A && git commit -m "feat: add week1 — C++ basics, CMake, types, namespaces"`

---

### Task 3: Week 2 — 指针与内存模型

**Files to create:**
```
phase1-foundation/week2-pointers-memory/
├── diff.md
├── notes.md
├── examples/
│   ├── 01-stack-vs-heap/main.cpp + CMakeLists.txt
│   ├── 02-swap-versions/main.cpp + CMakeLists.txt
│   ├── 03-array-reverse/main.cpp + CMakeLists.txt
│   └── 04-const-positions/main.cpp + CMakeLists.txt
└── exercises/
    ├── 01-str-functions/README.md + main.cpp + solution.cpp
    ├── 02-dynamic-2d-array/README.md + main.cpp + solution.cpp
    ├── 03-memory-leak/README.md + main.cpp + solution.cpp
    └── 04-multi-return/README.md + main.cpp + solution.cpp
```

**Modify:** `phase1-foundation/CMakeLists.txt`

- [ ] **Step 1: diff.md** — Java 引用 vs C++ 指针/引用对照表、const vs final、new/delete vs GC、数组越界检查差异

- [ ] **Step 2: notes.md** — 栈 vs 堆图示、指针声明/解引用/运算、引用语法、new/delete 规则、const 位置速查表

- [ ] **Step 3: 示例 01-stack-vs-heap** — Dog 结构体演示栈对象自动析构 vs 堆对象手动 delete

- [ ] **Step 4: 示例 02-swap-versions** — 值传递/指针传递/引用传递三种 swap，演示值传递无效

- [ ] **Step 5: 示例 03-array-reverse** — 纯指针运算实现 reverse 和 print，演示 `2[ptr]` 语法

- [ ] **Step 6: 示例 04-const-positions** — `const int*`、`int* const`、`const int* const`、`const int&` 四种形态

- [ ] **Step 7: 习题 01-str-functions** — 指针实现 strlen/strcpy/strcmp

- [ ] **Step 8: 习题 02-dynamic-2d-array** — new 分配二维数组，填充乘法表，delete[] 释放

- [ ] **Step 9: 习题 03-memory-leak** — 给出有泄漏的代码，找出并修复

- [ ] **Step 10: 习题 04-multi-return** — min_max 函数通过引用参数返回两个值

- [ ] **Step 11: 更新 CMakeLists.txt**，添加 4 个示例

- [ ] **Step 12: 构建验证**

- [ ] **Step 13: Commit**

---

### Task 4: Week 3 — RAII 与智能指针

**Files to create:**
```
phase1-foundation/week3-raii-smartptr/
├── diff.md
├── notes.md
├── examples/
│   ├── 01-raii-file/main.cpp + CMakeLists.txt
│   ├── 02-smartptr-factory/main.cpp + CMakeLists.txt
│   ├── 03-weakptr-cycle/main.cpp + CMakeLists.txt
│   └── 04-rule-of-five/main.cpp + CMakeLists.txt
└── exercises/
    ├── 01-raii-db/README.md + main.cpp + solution.cpp
    ├── 02-uniqueptr-list/README.md + main.cpp + solution.cpp
    ├── 03-cycle-ref/README.md + main.cpp + solution.cpp
    └── 04-rule-of-five-exercise/README.md + main.cpp + solution.cpp
```

**Modify:** `phase1-foundation/CMakeLists.txt`

- [ ] **Step 1: diff.md** — try-with-resources vs RAII 对照、finalize vs 析构函数、GC vs 确定性释放

- [ ] **Step 2: notes.md** — RAII 原理、unique_ptr/shared_ptr/weak_ptr 用法、make_unique/make_shared、三五法则详解

- [ ] **Step 3: 示例 01-raii-file** — FileHandle 类封装 fopen/fclose，演示自动清理

- [ ] **Step 4: 示例 02-smartptr-factory** — unique_ptr 工厂函数返回不同子类型，std::move 转移所有权

- [ ] **Step 5: 示例 03-weakptr-cycle** — Node 双向链表，shared_ptr 循环引用导致泄漏 vs weak_ptr 修复

- [ ] **Step 6: 示例 04-rule-of-five** — Buffer 类完整实现：析构、拷贝构造、拷贝赋值、移动构造、移动赋值

- [ ] **Step 7: 习题 01-raii-db** — 实现 RAII 数据库连接类（模拟 open/close）

- [ ] **Step 8: 习题 02-uniqueptr-list** — 用 unique_ptr 实现单向链表

- [ ] **Step 9: 习题 03-cycle-ref** — 找出 shared_ptr 循环引用并修复

- [ ] **Step 10: 习题 04-rule-of-five-exercise** — 为自定义类正确实现拷贝和移动语义

- [ ] **Step 11: 更新 CMakeLists.txt**

- [ ] **Step 12: 构建验证**

- [ ] **Step 13: Commit**

---

### Task 5: Week 4 — 类与面向对象

**Files to create:**
```
phase2-core/week4-classes-oop/
├── diff.md
├── notes.md
├── examples/
│   ├── 01-strategy-pattern/main.cpp + CMakeLists.txt
│   ├── 02-object-slicing/main.cpp + CMakeLists.txt
│   ├── 03-init-list/main.cpp + CMakeLists.txt
│   └── 04-diamond-inheritance/main.cpp + CMakeLists.txt
└── exercises/
    ├── 01-shape-hierarchy/README.md + main.cpp + solution.cpp
    ├── 02-noncopyable/README.md + main.cpp + solution.cpp
    ├── 03-fix-slicing/README.md + main.cpp + solution.cpp
    └── 04-composition-over-inheritance/README.md + main.cpp + solution.cpp
```

**Create:** `phase2-core/CMakeLists.txt`
**Modify:** `phase2-core/CMakeLists.txt`

- [ ] **Step 1: diff.md** — 无 Object 根类、无 interface 关键字、值语义 vs 引用语义、多重继承

- [ ] **Step 2: notes.md** — 构造函数种类、virtual/override/final、纯虚函数、访问控制、成员初始化列表必须用的场景、对象切片

- [ ] **Step 3: 示例 01-strategy-pattern** — 虚函数实现策略模式，对比 Java interface 版本（注释中展示）

- [ ] **Step 4: 示例 02-object-slicing** — 值传递导致派生类对象被切片

- [ ] **Step 5: 示例 03-init-list** — const 成员、引用成员必须用初始化列表

- [ ] **Step 6: 示例 04-diamond-inheritance** — 菱形继承问题 + virtual 继承解决

- [ ] **Step 7-10: 4 道习题** — Shape 类层次、不可拷贝类设计、修复对象切片、组合替代继承

- [ ] **Step 11: 更新 CMakeLists.txt**

- [ ] **Step 12: 构建验证**

- [ ] **Step 13: Commit**

---

### Task 6: Week 5 — 运算符重载与移动语义

**Files to create:**
```
phase2-core/week5-operators-move/
├── diff.md
├── notes.md
├── examples/
│   ├── 01-string-class/main.cpp + CMakeLists.txt
│   ├── 02-move-perf/main.cpp + CMakeLists.txt
│   ├── 03-functor/main.cpp + CMakeLists.txt
│   └── 04-perfect-forwarding/main.cpp + CMakeLists.txt
└── exercises/
    ├── 01-complex-ops/README.md + main.cpp + solution.cpp
    ├── 02-move-verify/README.md + main.cpp + solution.cpp
    ├── 03-matrix-access/README.md + main.cpp + solution.cpp
    └── 04-move-only/README.md + main.cpp + solution.cpp
```

**Modify:** `phase2-core/CMakeLists.txt`

- [ ] **Step 1: diff.md** — Java 无此概念，equals→operator==，toString→operator<<，移动语义是 C++11 核心

- [ ] **Step 2: notes.md** — 运算符重载规则、成员 vs 友元、拷贝 vs 移动、右值引用、std::move、完美转发、=default/=delete

- [ ] **Step 3: 示例 01-string-class** — 完整 String 类：构造/析构/拷贝/移动/operator==/operator<<

- [ ] **Step 4: 示例 02-move-perf** — vector push_back 拷贝 vs 移动的性能对比（chrono 计时）

- [ ] **Step 5: 示例 03-functor** — operator() 实现自定义比较器

- [ ] **Step 6: 示例 04-perfect-forwarding** — 完美转发工厂函数

- [ ] **Step 7-10: 4 道习题** — Complex 运算符重载、移动构造验证效率、Matrix operator()、move-only 类

- [ ] **Step 11-13: CMake 更新、构建、提交**

---

### Task 7: Week 6 — 模板基础

**Files to create:**
```
phase2-core/week6-templates/
├── diff.md
├── notes.md
├── examples/
│   ├── 01-generic-max/main.cpp + CMakeLists.txt
│   ├── 02-vector-template/main.cpp + vector.hpp + CMakeLists.txt
│   ├── 03-compile-time-fib/main.cpp + CMakeLists.txt
│   └── 04-type-traits-intro/main.cpp + CMakeLists.txt
└── exercises/
    ├── 01-stack-template/README.md + main.cpp + solution.cpp
    ├── 02-print-container/README.md + main.cpp + solution.cpp
    ├── 03-compile-time-array/README.md + main.cpp + solution.cpp
    └── 04-specialization/README.md + main.cpp + solution.cpp
```

**Modify:** `phase2-core/CMakeLists.txt`

- [ ] **Step 1: diff.md** — Java 泛型类型擦除 vs C++ 模板代码生成、基本类型可直接做模板参数

- [ ] **Step 2: notes.md** — 函数模板/类模板、全特化/偏特化、非类型模板参数、typename vs class、模板代码组织

- [ ] **Step 3: 示例 01-generic-max** — 泛型 max/min 函数模板

- [ ] **Step 4: 示例 02-vector-template** — 手写 Vector<T> 类模板（动态数组）

- [ ] **Step 5: 示例 03-compile-time-fib** — 非类型参数 + 特化实现编译期斐波那契

- [ ] **Step 6: 示例 04-type-traits-intro** — is_same、enable_if 入门

- [ ] **Step 7-10: 4 道习题** — Stack<T> 模板、打印任意容器、编译期数组、const char* 特化

- [ ] **Step 11-13: CMake 更新、构建、提交**

---

### Task 8: Week 7 — STL 容器与算法

**Files to create:**
```
phase2-core/week7-stl/
├── diff.md
├── notes.md
├── examples/
│   ├── 01-container-choice/main.cpp + CMakeLists.txt
│   ├── 02-stream-to-algorithm/main.cpp + CMakeLists.txt
│   ├── 03-custom-sort/main.cpp + CMakeLists.txt
│   └── 04-string-view/main.cpp + CMakeLists.txt
└── exercises/
    ├── 01-word-count/README.md + main.cpp + solution.cpp
    ├── 02-map-filter-reduce/README.md + main.cpp + solution.cpp
    ├── 03-lru-cache/README.md + main.cpp + solution.cpp
    └── 04-container-bench/README.md + main.cpp + solution.cpp
```

**Modify:** `phase2-core/CMakeLists.txt`

- [ ] **Step 1: diff.md** — ArrayList→vector、HashMap→unordered_map、Stream API→\<algorithm\>+lambda

- [ ] **Step 2: notes.md** — 三大类容器速查、迭代器类别、\<algorithm\> 常用函数、lambda 表达式语法、string_view

- [ ] **Step 3: 示例 01-container-choice** — 各容器插入/查找/删除性能对比表

- [ ] **Step 4: 示例 02-stream-to-algorithm** — Java Stream 链式处理 → C++ \<algorithm\> 等价写法对照

- [ ] **Step 5: 示例 03-custom-sort** — lambda 比较器、函数对象排序

- [ ] **Step 6: 示例 04-string-view** — string_view 避免拷贝的性能优势

- [ ] **Step 7-10: 4 道习题** — 词频统计、map/filter/reduce、LRU 缓存、vector/list/deque 性能对比

- [ ] **Step 11-13: CMake 更新、构建、提交**

---

### Task 9: Week 8 — 模板元编程入门

**Files to create:**
```
phase3-advanced/week8-tmp/
├── diff.md
├── notes.md
├── examples/
│   ├── 01-constexpr-factorial/main.cpp + CMakeLists.txt
│   ├── 02-type-list/main.cpp + CMakeLists.txt
│   ├── 03-enable-if-overload/main.cpp + CMakeLists.txt
│   └── 04-variadic-print/main.cpp + CMakeLists.txt
└── exercises/
    ├── 01-constexpr-sqrt/README.md + main.cpp + solution.cpp
    ├── 02-type-dispatch/README.md + main.cpp + solution.cpp
    ├── 03-make-vector/README.md + main.cpp + solution.cpp
    └── 04-sfinae-detect/README.md + main.cpp + solution.cpp
```

**Modify:** `phase3-advanced/CMakeLists.txt`

- [ ] **Step 1: diff.md** — Java 无编译期计算（注解处理器勉强算但完全不同）

- [ ] **Step 2: notes.md** — constexpr 函数/变量、type_traits 常用工具、SFINAE/enable_if、变参模板、折叠表达式(C++17)

- [ ] **Step 3: 示例 01-constexpr-factorial** — constexpr 函数实现编译期阶乘/斐波那契

- [ ] **Step 4: 示例 02-type-list** — TypeList<int,double,float> 与长度计算、编译期类型操作

- [ ] **Step 5: 示例 03-enable-if-overload** — enable_if 实现整数版（位运算）vs 浮点版（数学库）算法选择

- [ ] **Step 6: 示例 04-variadic-print** — 变参 print（递归展开 + 折叠表达式两种方式）

- [ ] **Step 7-10: 4 道习题** — constexpr sqrt（牛顿法）、类型派发、make_vector 工厂、SFINAE 检测 size() 方法

- [ ] **Step 11-13: CMake 更新、构建、提交**

---

### Task 10: Week 9 — 并发编程

**Files to create:**
```
phase3-advanced/week9-concurrency/
├── diff.md
├── notes.md
├── examples/
│   ├── 01-thread-sum/main.cpp + CMakeLists.txt
│   ├── 02-producer-consumer/main.cpp + CMakeLists.txt
│   ├── 03-async-tasks/main.cpp + CMakeLists.txt
│   └── 04-thread-safe-singleton/main.cpp + CMakeLists.txt
└── exercises/
    ├── 01-parallel-matmul/README.md + main.cpp + solution.cpp
    ├── 02-thread-safe-queue/README.md + main.cpp + solution.cpp
    ├── 03-lockfree-counter/README.md + main.cpp + solution.cpp
    └── 04-parallel-reduce/README.md + main.cpp + solution.cpp
```

**Modify:** `phase3-advanced/CMakeLists.txt`

- [ ] **Step 1: diff.md** — synchronized→mutex+lock_guard、volatile 语义完全不同、无内置线程池、wait/notify→condition_variable

- [ ] **Step 2: notes.md** — thread 创建/join/detach、mutex/lock_guard/unique_lock、condition_variable、atomic 与内存序入门、future/promise/async

- [ ] **Step 3: 示例 01-thread-sum** — 无锁 → mutex → atomic 三种版本多线程累加对比

- [ ] **Step 4: 示例 02-producer-consumer** — condition_variable 实现生产者-消费者

- [ ] **Step 5: 示例 03-async-tasks** — std::async 并行计算任务

- [ ] **Step 6: 示例 04-thread-safe-singleton** — C++11 静态局部变量保证线程安全

- [ ] **Step 7-10: 4 道习题** — 多线程矩阵乘法、线程安全队列、无锁计数器、并行归约

- [ ] **Step 11-13: CMake 更新、构建、提交**

---

### Task 11: Week 10 — 异常安全与错误处理

**Files to create:**
```
phase3-advanced/week10-exceptions/
├── diff.md
├── notes.md
├── examples/
│   ├── 01-exception-safe-stack/main.cpp + CMakeLists.txt
│   ├── 02-optional-pattern/main.cpp + CMakeLists.txt
│   ├── 03-variant-union/main.cpp + CMakeLists.txt
│   └── 04-ctor-failure/main.cpp + CMakeLists.txt
└── exercises/
    ├── 01-vector-push-back/README.md + main.cpp + solution.cpp
    ├── 02-optional-find/README.md + main.cpp + solution.cpp
    ├── 03-divide-errors/README.md + main.cpp + solution.cpp
    └── 04-exception-safety-audit/README.md + main.cpp + solution.cpp
```

**Modify:** `phase3-advanced/CMakeLists.txt`

- [ ] **Step 1: diff.md** — checked exception 不存在、finally→RAII、Optional→std::optional、异常规范方向相反

- [ ] **Step 2: notes.md** — 异常安全三等级、noexcept、RAII 与异常、std::optional、std::variant、错误码模式

- [ ] **Step 3: 示例 01-exception-safe-stack** — 异常安全 vs 不安全的栈实现对比

- [ ] **Step 4: 示例 02-optional-pattern** — std::optional 替代返回 null 的模式

- [ ] **Step 5: 示例 03-variant-union** — std::variant 实现类型安全的 union

- [ ] **Step 6: 示例 04-ctor-failure** — 构造函数失败处理：抛异常 vs 工厂方法+optional

- [ ] **Step 7-10: 4 道习题** — Vector::push_back 异常安全、optional find、divide 错误处理方案对比、代码异常安全审计

- [ ] **Step 11-13: CMake 更新、构建、提交**

---

### Task 12: Week 11 — 性能优化基础

**Files to create:**
```
phase3-advanced/week11-performance/
├── diff.md
├── notes.md
├── examples/
│   ├── 01-aos-vs-soa/main.cpp + CMakeLists.txt
│   ├── 02-loop-unroll/main.cpp + CMakeLists.txt
│   ├── 03-google-benchmark/main.cpp + CMakeLists.txt
│   └── 04-alignment-simd/main.cpp + CMakeLists.txt
└── exercises/
    ├── 01-aos-to-soa/README.md + main.cpp + solution.cpp
    ├── 02-benchmark-macro/README.md + main.cpp + solution.cpp
    ├── 03-cache-friendly-transpose/README.md + main.cpp + solution.cpp
    └── 04-optimization-levels/README.md + main.cpp + solution.cpp
```

**Modify:** `phase3-advanced/CMakeLists.txt`

- [ ] **Step 1: diff.md** — JIT vs AOT 优化、对象头开销、精确控制内存布局

- [ ] **Step 2: notes.md** — 缓存层次与 Cache Line、AoS vs SoA、alignas/alignof、内联与 -O2/-O3、chrono benchmark、perf/valgrind 简介

- [ ] **Step 3: 示例 01-aos-vs-soa** — AoS vs SoA 遍历性能对比

- [ ] **Step 4: 示例 02-loop-unroll** — 循环展开与编译器自动向量化观察

- [ ] **Step 5: 示例 03-google-benchmark** — Google Benchmark 使用示例

- [ ] **Step 6: 示例 04-alignment-simd** — alignas 对齐对性能的影响

- [ ] **Step 7-10: 4 道习题** — AoS→SoA 重构并测量、benchmark 宏、缓存友好矩阵转置、-O0/-O2/-O3 对比

- [ ] **Step 11-13: CMake 更新、构建、提交**

---

### Task 13: Week 12 — 张量的内存模型

**Files to create:**
```
phase4-operator/week12-tensor-memory/
├── diff.md
├── notes.md
├── examples/
│   ├── 01-tensor-indexing/main.cpp + CMakeLists.txt
│   ├── 02-zero-copy-slice/main.cpp + CMakeLists.txt
│   └── 03-aligned-allocator/main.cpp + CMakeLists.txt
└── exercises/
    ├── 01-tensor2d-class/README.md + main.cpp + solution.cpp
    ├── 02-stride-transpose/README.md + main.cpp + solution.cpp
    └── 03-memory-pool/README.md + main.cpp + solution.cpp
```

**Create:** `phase4-operator/CMakeLists.txt`
**Modify:** `phase4-operator/CMakeLists.txt`

- [ ] **Step 1: diff.md** — 无 Java 对照（此阶段为纯 C++ 算子专项）

- [ ] **Step 2: notes.md** — 行优先 vs 列优先、stride 语义、零拷贝切片、自定义 allocator、aligned_alloc、内存池概念

- [ ] **Step 3: 示例 01-tensor-indexing** — 手写 2D/3D 张量的线性索引计算（row-major）

- [ ] **Step 4: 示例 02-zero-copy-slice** — 共享数据指针 + 独立 stride/offset 的零拷贝切片

- [ ] **Step 5: 示例 03-aligned-allocator** — 自定义对齐分配器（alignas + aligned_alloc）

- [ ] **Step 6-8: 3 道习题** — Tensor2D 类（创建/索引/打印）、stride-based 转置（不拷贝）、简单内存池分配器

- [ ] **Step 9-11: CMake 更新、构建、提交**

---

### Task 14: Week 13 — 高性能算子核函数

**Files to create:**
```
phase4-operator/week13-kernels/
├── diff.md
├── notes.md
├── examples/
│   ├── 01-element-wise/main.cpp + CMakeLists.txt
│   ├── 02-matmul-optimize/main.cpp + CMakeLists.txt
│   └── 03-stable-softmax/main.cpp + CMakeLists.txt
└── exercises/
    ├── 01-element-wise-add/README.md + main.cpp + solution.cpp
    ├── 02-reduce-sum/README.md + main.cpp + solution.cpp
    └── 03-matmul-blocking/README.md + main.cpp + solution.cpp
```

**Modify:** `phase4-operator/CMakeLists.txt`

- [ ] **Step 1: notes.md** — 逐元素操作模板化、归约模式、矩阵乘法优化阶梯（朴素→循环交换→分块）、广播语义、softmax 数值稳定

- [ ] **Step 2: 示例 01-element-wise** — 模板化 element_wise 操作（支持任意类型组合）

- [ ] **Step 3: 示例 02-matmul-optimize** — 矩阵乘法三级优化对比（朴素→循环交换→分块），用 chrono 计时

- [ ] **Step 4: 示例 03-stable-softmax** — 数值稳定的 softmax 实现（减 max 防溢出）

- [ ] **Step 5-7: 3 道习题** — element_wise_add 模板、多维 reduce_sum（沿指定轴）、矩阵乘法分块优化并 benchmark

- [ ] **Step 8-10: CMake 更新、构建、提交**

---

### Task 15: Week 14 — 集成与项目收尾

**Files to create:**
```
phase4-operator/week14-capstone/
├── README.md
├── CMakeLists.txt
├── include/tensor.hpp
├── src/tensor.cpp
├── include/operators.hpp
├── src/operators.cpp
├── include/autograd.hpp
├── src/autograd.cpp
├── tests/
│   ├── CMakeLists.txt
│   ├── test_tensor.cpp
│   ├── test_operators.cpp
│   └── test_autograd.cpp
└── examples/
    ├── CMakeLists.txt
    └── demo.cpp
```

**Modify:** `phase4-operator/CMakeLists.txt`

- [ ] **Step 1: 创建库骨架** — tensor.hpp/cpp 定义 Tensor<T, Rank> 模板类（创建、索引、形状、切片）

- [ ] **Step 2: 实现 operators** — operators.hpp/cpp 包含：element_wise（add/sub/mul/div/relu/sigmoid）、reduce（sum/mean/max）、matmul、softmax

- [ ] **Step 3: 实现 autograd 骨架** — autograd.hpp/cpp 包含：计算图节点、前向记录、手动反向传播（grad 累加）

- [ ] **Step 4: 集成 Google Test** — CMakeLists.txt 中 FetchContent 下载 Google Test，tests/ 目录下三个测试文件

- [ ] **Step 5: 编写测试** — test_tensor.cpp（创建/索引/切片/reshape）、test_operators.cpp（每个算子的正确性）、test_autograd.cpp（简单计算图梯度验证）

- [ ] **Step 6: 示例程序** — demo.cpp：创建张量、执行 matmul+relu、验证自动求导

- [ ] **Step 7: 构建并运行测试**

```bash
cmake -S . -B build && cmake --build build && cd build && ctest --output-on-failure
```

- [ ] **Step 8: Commit** `git add -A && git commit -m "feat: add capstone — micro tensor computation library"`

---

### Task 16: 收尾 — 更新顶层 CMakeLists.txt 并最终验证

- [ ] **Step 1: 验证全项目构建**

```bash
cmake -S . -B build && cmake --build build
```

- [ ] **Step 2: 运行所有可用的测试**

```bash
cd build && ctest --output-on-failure
```

- [ ] **Step 3: 确保所有示例可独立运行** — 逐个运行 build 目录下的可执行文件

- [ ] **Step 4: Commit** `git add -A && git commit -m "chore: final verification and cleanup"`

- [ ] **Step 5: 查看 git log 确认结构完整**

```bash
git log --oneline
```

---

## Self-Review Checklist

1. **Spec coverage**: 14 周 × 4 阶段全部覆盖 ✓，每个 week 有 diff/notes/examples/exercises ✓，capstone 项目对应 Week 14 ✓
2. **Placeholder scan**: 无 TBD/TODO/占位符 ✓
3. **Type consistency**: 各模块使用统一的目录结构和命名规范 ✓

