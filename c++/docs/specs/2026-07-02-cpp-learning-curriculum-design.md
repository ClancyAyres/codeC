# C++ 学习路径设计：从 Java 到 AI 算子开发

## 背景

- 学习者有 1-2 年 Java 工作经验，熟悉面向对象编程
- 即将转入 AI 算子开发，具体方向待定（可能涉及 PyTorch/TensorRT/MLIR 等框架）
- 学习时间：3-4 个月
- 学习风格：混合式（每阶段先讲核心概念和差异，立刻跟上示例和习题）

## 目标

产出物：
1. 一套结构化、可复用的 C++ 学习材料（差异对照、示例代码、习题）
2. 一个微型张量计算库作为结业项目（约 500-800 行）

## 整体架构

```
c++/
├── phase1-foundation/        # 阶段一：地基（3 周）
│   ├── week1-cpp-basics/
│   ├── week2-pointers-memory/
│   └── week3-raii-smartptr/
├── phase2-core/              # 阶段二：核心（4 周）
│   ├── week4-classes-oop/
│   ├── week5-operators-move/
│   ├── week6-templates/
│   └── week7-stl/
├── phase3-advanced/          # 阶段三：进阶（4 周）
│   ├── week8-tmp/
│   ├── week9-concurrency/
│   ├── week10-exceptions/
│   └── week11-performance/
├── phase4-operator/          # 阶段四：算子实战（3 周）
│   ├── week12-tensor-memory/
│   ├── week13-kernels/
│   └── week14-capstone/
├── CMakeLists.txt            # 顶层 CMake
├── docs/
│   └── superpowers/
│       └── specs/
│           └── 2026-07-02-cpp-learning-curriculum-design.md
└── .gitignore
```

## 模块统一结构

每个模块（week）目录下包含：

```
weekN-topic/
├── diff.md          # Java vs C++ 差异对照（核心差异 + 代码对比）
├── notes.md         # 知识点笔记（详细讲解）
├── examples/        # 3-5 个可独立编译运行的示例程序
│   ├── 01-example-name/
│   │   ├── main.cpp
│   │   └── CMakeLists.txt
│   └── ...
├── exercises/       # 3-5 道编程习题
│   ├── 01-exercise-name/
│   │   ├── README.md    # 题目描述
│   │   ├── main.cpp     # 骨架代码
│   │   └── solution.cpp # 参考答案
│   └── ...
```

每个示例/习题用 CMake 独立管理，可在模块级别一键构建。

## 阶段一：地基（3 周）

### Week 1：从 Java 到 C++ 的第一步

**核心内容：**
- CMake 构建系统基础（`CMakeLists.txt`、`add_executable`、`target_include_directories`）
- 头文件（`.h`/`.hpp`）与源文件（`.cpp`）分离
- 编译链接模型：预处理 → 编译 → 汇编 → 链接
- `main` 函数、命令行参数
- 基本类型：`int`、`long`、`float`、`double`、`char`、`bool`
- 类型大小与平台相关性（`sizeof`）
- 命名空间（`namespace`）
- `std::cout`/`std::cin` 基本 I/O

**Java 差异重点：**
- JVM 字节码 vs 原生机器码
- classpath vs 头文件包含
- 基本类型大小不固定（`int` 至少 16 位但不固定）
- 没有 `byte`/`boolean` 关键字，用 `int8_t`/`bool`
- `package` → `namespace`
- 字符串：`std::string` vs `String`（可变性、编码差异）

**示例（4 个）：**
1. 完整 CMake 项目骨架：Hello World + 库分离
2. 基本类型边界测试：各类型取值范围、溢出行为
3. 命名空间隔离：同名符号在不同 namespace
4. 头文件守卫与循环依赖演示

**习题（4 道）：**
1. 搭建一个 CMake 项目，包含 `libmath` 库和 `app` 可执行文件
2. 写一个函数打印所有基本类型的 `sizeof`
3. 用命名空间实现两个版本的 `max` 函数
4. 模拟头文件重复包含问题并修复

---

### Week 2：指针与内存模型

**核心内容：**
- 栈内存 vs 堆内存
- 指针声明、取地址（`&`）、解引用（`*`）
- 指针运算（`+`、`-`、`[]`）
- `new`/`delete`、`new[]`/`delete[]`
- 引用（左值引用）作为别名
- 指针 vs 引用的选择
- 数组与指针的关系
- `const` 的各种位置（`const T*`、`T* const`、`const T&`）
- `nullptr`

**Java 差异重点：**
- Java 所有对象在堆上；C++ 对象可在栈上
- Java 引用 ≈ C++ 指针（但不可运算）
- C++ 引用 ≠ Java 引用（C++ 引用是别名，不可为空）
- 没有 GC，`new` 必须配 `delete`
- `final` → `const`（但 `const` 远比 `final` 强大）
- 没有数组越界检查（默认）

**示例（4 个）：**
1. 栈对象 vs 堆对象的生命周期演示
2. 手写 `swap`（指针版 vs 引用版）
3. 数组反转（指针运算版）
4. `const` 位置组合表与编译错误演示

**习题（4 道）：**
1. 用指针实现 `strlen`、`strcpy`、`strcmp`
2. 用 `new` 创建动态二维数组，正确释放
3. 找出代码中的内存泄漏并修复
4. 写一个函数同时返回多个值（用指针/引用输出参数）

---

### Week 3：RAII 与智能指针

**核心内容：**
- 构造函数与析构函数的调用时机
- RAII（Resource Acquisition Is Initialization）模式
- `std::unique_ptr`：独占所有权、`std::make_unique`、`std::move` 转移
- `std::shared_ptr`：共享所有权、引用计数、`std::make_shared`
- `std::weak_ptr`：打破循环引用
- 三五法则：析构、拷贝构造、拷贝赋值、移动构造、移动赋值
- `= default` / `= delete`

**Java 差异重点：**
- `try-with-resources` vs RAII（RAII 更通用，不限于 I/O）
- `finalize()` vs 析构函数（析构是确定性的）
- GC vs 确定性资源释放
- `Cloneable.clone()` vs 拷贝构造函数

**示例（4 个）：**
1. 文件 RAII 封装：`FileHandle` 类
2. 智能指针工厂模式：根据条件返回不同子类型
3. 循环引用与 `weak_ptr` 的破解
4. 三五法则完整演示：带资源管理的类

**习题（4 道）：**
1. 实现一个 RAII 数据库连接类
2. 用 `unique_ptr` 实现链表并正确释放
3. 找出 `shared_ptr` 循环引用并修复
4. 为一个类正确实现拷贝和移动语义

---

## 阶段二：核心（4 周）

### Week 4：类与面向对象

**核心内容：**
- 构造函数种类：默认、带参、拷贝、移动、委托
- 成员初始化列表（必须用的场景）
- `virtual`、`override`、`final`
- 纯虚函数与抽象类
- 访问控制：`public`/`protected`/`private` 继承
- 值语义 vs 引用语义（对象切片问题）
- `this` 指针
- `static` 成员变量/方法
- `friend` 类/函数

**Java 差异重点：**
- 没有 `Object` 根类
- 没有 `interface` 关键字（用纯虚类模拟）
- 对象可存在于栈上（值语义），不必总是 `new`
- 拷贝控制是 C++ 独有且核心的概念
- 访问控制在类级别而非文件级别
- 多重继承（含菱形问题）→ `virtual` 继承

**示例（4 个）：**
1. 策略模式：虚函数实现 vs Java interface 版本对比
2. 对象切片：值传递导致的多态丢失
3. 成员初始化列表必要性演示（const 成员、引用成员）
4. 菱形继承与虚继承

**习题（4 道）：**
1. 实现一个 Shape 类层次：`Shape` → `Circle`、`Rectangle`，计算面积
2. 设计一个不可拷贝的类（`= delete`）
3. 找出并修复对象切片 bug
4. 用组合替代多重继承重构设计

---

### Week 5：运算符重载与移动语义

**核心内容：**
- 运算符重载规则（哪些可重载、哪些不可）
- 成员函数 vs 友元函数重载
- `operator<<`（流输出）、`operator==`、`operator[]`、`operator()` 等
- 拷贝构造 vs 拷贝赋值
- 移动构造 vs 移动赋值
- 右值引用（`T&&`）
- `std::move`（语义上的转移，不是物理移动）
- 完美转发（`std::forward`）简介
- `= default` / `= delete` 在拷贝/移动控制中的使用

**Java 差异重点：**
- Java 无此概念——这是 C++ 零开销抽象的关键
- Java `equals()` → C++ `operator==`
- Java `toString()` → C++ `operator<<`
- 移动语义是 C++11 最重要的新增特性

**示例（4 个）：**
1. 完整的 `String` 类：实现所有运算符 + 拷贝/移动
2. 性能对比：`vector` push_back 拷贝 vs 移动的开销
3. 函数对象（`operator()`）：自定义比较器
4. 完美转发工厂函数

**习题（4 道）：**
1. 为 `Complex` 类重载 `+`、`-`、`*`、`==`、`<<`
2. 实现移动构造/赋值并验证效率提升
3. 写一个 `Matrix` 类支持 `operator()` 访问元素
4. 设计一个只能移动不能拷贝的类

---

### Week 6：模板基础

**核心内容：**
- 函数模板：类型推导、显式实例化
- 类模板：成员函数定义位置
- 模板特化：全特化、偏特化
- 非类型模板参数（`template<int N>`）
- `typename` vs `class` 关键字
- 模板代码组织（`.hpp` 中全部定义或显式实例化）
- 概念（Concepts，C++20 简介，可选）

**Java 差异重点：**
- Java 泛型 = 类型擦除 + 运行时检查；C++ 模板 = 编译期代码生成
- 基本类型（`int`、`double`）可做模板参数（Java 必须装箱）
- 模板可做编译期计算（模板元编程）
- 错误信息差异：C++ 模板错误通常极其冗长

**示例（4 个）：**
1. 泛型 `max`/`min` 函数模板
2. 手写 `Vector<T>` 类模板（动态数组）
3. 编译期斐波那契（非类型参数 + 特化）
4. 类型萃取：`is_same`、`enable_if` 入门

**习题（4 道）：**
1. 实现一个泛型 `Stack<T>` 类模板
2. 写一个模板函数打印任意容器的所有元素
3. 用非类型模板参数实现编译期数组
4. 模板特化：为 `const char*` 特化比较逻辑

---

### Week 7：STL 容器与算法

**核心内容：**
- 序列容器：`vector`、`deque`、`list`、`array`
- 关联容器：`map`、`set`、`unordered_map`、`unordered_set`
- 容器适配器：`stack`、`queue`、`priority_queue`
- 迭代器：`begin()`/`end()`、`cbegin()`/`cend()`、范围 for
- `<algorithm>`：`sort`、`find`、`copy`、`transform`、`accumulate`、`remove_if`
- Lambda 表达式：`[capture](params) -> ret { body }`
- `std::string`、`std::string_view`

**Java 差异重点：**
- `ArrayList<T>` → `std::vector<T>`
- `HashMap<K,V>` → `std::unordered_map<K,V>`
- `TreeMap<K,V>` → `std::map<K,V>`
- Stream API → `<algorithm>` + lambda
- `String` → `std::string`（可变，无 intern pool）
- 迭代器比 Java Iterator 更灵活（随机访问迭代器等）

**示例（4 个）：**
1. 容器选择决策树 + 性能对比
2. Java Stream 风格链式处理 → C++ `<algorithm>` 等价写法
3. 自定义排序：lambda 比较器、函数对象
4. `string_view` 避免拷贝的性能优势

**习题（4 道）：**
1. 用 STL 实现词频统计（替代 Java HashMap 方案）
2. 用 `<algorithm>` 实现 `map`/`filter`/`reduce`
3. 实现一个 LRU 缓存（`list` + `unordered_map`）
4. 对比 `vector`/`list`/`deque` 在不同操作下的性能

---

## 阶段三：进阶（4 周）

### Week 8：模板元编程入门

**核心内容：**
- `constexpr`：编译期函数、编译期变量
- 类型萃取：`<type_traits>` 常用工具
- SFINAE 与 `std::enable_if`
- 变参模板（Variadic Templates）
- 折叠表达式（C++17）
- 编译期类型列表与递归展开

**Java 差异重点：**
- Java 无编译期计算（注解处理器勉强算，但完全不同）
- 这是算子开发的核心技巧——编译期多态零开销
- 常用于：根据数据类型选择最优算法路径

**示例（4 个）：**
1. 编译期阶乘/斐波那契（`constexpr` 函数）
2. 类型列表：`TypeList<int, double, float>` 与长度计算
3. `enable_if` 实现函数重载选择（整数版 vs 浮点版算法）
4. 变参 `print` 函数（递归展开 + 折叠表达式对比）

**习题（4 道）：**
1. 用 `constexpr` 实现编译期 `sqrt`（牛顿法）
2. 用类型萃取实现：整数类型用位运算、浮点用数学库
3. 变参模板实现 `make_vector` 工厂函数
4. 用 SFINAE 检测一个类型是否有 `size()` 方法

---

### Week 9：并发编程

**核心内容：**
- `std::thread`：创建、join、detach
- 互斥量：`std::mutex`、`std::lock_guard`、`std::unique_lock`
- 条件变量：`std::condition_variable`
- 原子操作：`std::atomic<T>`、内存序概念（入门）
- `std::future`、`std::promise`、`std::async`
- 线程安全的单例模式
- 线程池模式简介

**Java 差异重点：**
- `synchronized` → `std::mutex` + `std::lock_guard`
- `volatile` 在 C++ 中与并发无关（仅防止编译器优化）
- 无内置线程池（需自己实现或用第三方库）
- `wait()/notify()` → `condition_variable::wait()/notify_one()`
- `Thread.start()` → `std::thread` 构造即启动

**示例（4 个）：**
1. 多线程累加：无锁 → mutex → atomic 三种版本对比
2. 生产者-消费者：`condition_variable` 实现
3. `std::async` 并行计算任务
4. 线程安全单例（C++11 静态局部变量保证）

**习题（4 道）：**
1. 多线程矩阵乘法并验证正确性
2. 实现一个简单的线程安全队列
3. 用原子操作实现无锁计数器
4. 用 `future`/`promise` 实现并行归约

---

### Week 10：异常安全与错误处理

**核心内容：**
- 异常安全三个等级：基本保证、强保证、不抛出
- `noexcept` 规范
- RAII 如何保证异常安全
- `std::optional<T>`：可能不存在的值
- `std::variant<T...>`：多类型联合
- `std::expected<T,E>`（C++23，或自定义实现）
- 错误码模式（`std::error_code`）
- 构造函数中的错误处理

**Java 差异重点：**
- checked exception 不存在
- `finally` → RAII（在析构函数中清理）
- `Optional<T>` → `std::optional<T>`
- 异常规范相反：Java `throws` 声明可能抛出，C++ `noexcept` 声明不抛出

**示例（4 个）：**
1. 异常安全的栈 vs 不安全的栈
2. `std::optional` 替代返回 null 的模式
3. `std::variant` 实现类型安全的 union
4. 构造函数失败处理（抛异常 vs 工厂方法 + optional）

**习题（4 道）：**
1. 实现异常安全的 `Vector::push_back`
2. 用 `optional` 实现 `find` 函数
3. 为 `divide` 函数设计错误处理（抛异常 vs `optional` vs 错误码）
4. 分析一段代码的异常安全等级并修复

---

### Week 11：性能优化基础

**核心内容：**
- 缓存层次结构与缓存行（Cache Line）
- AoS（Array of Structs）vs SoA（Struct of Arrays）
- 数据对齐（`alignas`、`alignof`）
- 内联（`inline`）与编译器优化（`-O2`、`-O3`）
- Benchmark 基础：`std::chrono`、Google Benchmark
- 性能分析工具：`perf`、`valgrind`（Linux）/ Instruments（macOS）
- SIMD 概念入门（不深入手写 intrinsic）
- 分支预测友好代码

**Java 差异重点：**
- JIT 编译优化 vs AOT 编译优化
- Java 对象头开销 vs C++ 精确控制内存布局
- GC 停顿 vs 确定性内存管理
- 可精确控制内存对齐和布局

**示例（4 个）：**
1. AoS vs SoA 遍历性能对比（缓存命中率）
2. 循环展开与编译器自动向量化
3. Google Benchmark 使用：对比不同实现
4. `alignas` 对齐对 SIMD 的性能影响

**习题（4 道）：**
1. 将 AoS 结构改为 SoA 并测量性能差异
2. 用 `std::chrono` 实现一个简单 benchmark 宏
3. 优化矩阵转置（考虑缓存友好）
4. 对比 `-O0`/`-O2`/`-O3` 下同一代码的性能

---

## 阶段四：算子实战（3 周）

### Week 12：张量的内存模型

**核心内容：**
- 多维数组的线性存储：行优先（row-major）vs 列优先（col-major）
- Stride：非连续视图的步长表示
- 切片（slice）与广播（broadcast）的内存语义
- 自定义内存分配器（`std::allocator`）
- 内存对齐与 `aligned_alloc`
- 内存池概念

**示例（3 个）：**
1. 手写 2D/3D 张量的索引计算（行优先）
2. 零拷贝切片：共享数据 + 独立 stride/offset
3. 自定义对齐分配器

**习题（3 道）：**
1. 实现 `Tensor2D` 类：创建、索引、打印
2. 实现 stride-based 的转置（不拷贝数据）
3. 实现一个简单的内存池分配器

---

### Week 13：高性能算子核函数

**核心内容：**
- 逐元素操作（element-wise）：模板化 `add`、`mul`、`relu`
- 归约操作（reduce）：`sum`、`max`、`mean`
- 矩阵乘法（gemm）：朴素实现 → 循环重排 → 分块
- 广播（broadcast）语义实现
- `softmax` 的数值稳定实现
- 编译器优化提示与 pragma

**示例（3 个）：**
1. 逐元素操作的模板化实现（支持任意类型组合）
2. 矩阵乘法：三级优化对比（朴素 → 循环交换 → 分块）
3. 数值稳定的 softmax

**习题（3 道）：**
1. 实现模板化的 `element_wise_add(A, B, C)`
2. 实现多维 `reduce_sum`（沿指定轴）
3. 优化矩阵乘法：尝试不同分块大小并 benchmark

---

### Week 14：集成与项目收尾

**核心内容：**
- 整合前 13 周成果：构建微型张量计算库
- CMake 打包：`install`、`find_package`
- 单元测试：Google Test 集成
- 文档与使用示例
- Python 绑定概念（pybind11 简介，可选）
- 回顾与进阶方向指引

**结业项目：微型张量计算库**

功能清单：
- `Tensor<T, Rank>` 模板类
  - 创建（从数据、零初始化、随机）
  - 索引访问（`operator()`）
  - 切片（返回视图，零拷贝）
  - 形状变换（reshape）
- 基本算子
  - 逐元素：`add`、`sub`、`mul`、`div`、`relu`、`sigmoid`
  - 归约：`sum`、`mean`、`max`
  - 矩阵乘法：`matmul`
  - 激活函数：`softmax`（数值稳定版）
- 自动求导骨架
  - 计算图节点
  - 前向传播记录
  - 反向传播（手动实现梯度传播）
- 测试
  - 每个算子的正确性测试
  - 与 NumPy 结果对比（手动验证）

---

## 技术要求

- **编译器**：Clang 15+ 或 GCC 12+（支持 C++17，部分示例需 C++20）
- **构建系统**：CMake 3.20+
- **测试框架**：Google Test（阶段四引入）
- **代码风格**：Google C++ Style Guide（基础规范，不强制全部）
- **平台**：macOS/Linux 均可

## 使用方式

每个模块可独立学习。建议按顺序推进，但如果有特定 Java 背景需要快速查阅差异，可直接跳转对应模块的 `diff.md`。

## 后续扩展

完成此路径后，可进一步深入：
- CUDA 算子开发（GPU 编程）
- MLIR/TVM 编译器算子
- 深度学习框架源码阅读（PyTorch ATen、TensorFlow 自定义算子）
- 高性能计算专题（SIMD 深入、多线程调度）
