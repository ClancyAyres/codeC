# Java 转 C++ 面向工作学习课程设计

## 目标

面向实际 C++ 开发工作场景，帮助 Java 开发者快速达到能独立承担日常开发任务的水平。学完之后，你应该能：

- 读懂并修改现有 C++ 代码库
- 独立完成中小型功能的开发
- 代码审查中不犯低级错误
- 排查常见的内存问题和编译链接错误
- 写出符合业界一流编码规范的 C++ 代码

## 设计原则

- **以工作场景驱动**：不是"学特性"，而是"完成工作任务"
- **踩坑优先**：每个模块重点标注 Java 开发者最容易踩的坑
- **代码审查视角**：每个模块包含"审阅时会被打回的代码"
- **编码规范贯穿**：C++ Core Guidelines（原则层）+ Google C++ Style Guide（规则层）融入每个知识点
- **难度递进**：前期模块面向"能干活"，后期模块面向"干好活"

## 编码规范体系

以 **C++ Core Guidelines**（Stroustrup & Sutter）为主干提供设计原则，以 **Google C++ Style Guide** 为补充提供可执行细则。不在学习材料外单独开一章讲规范，而是把规范规则散布到每个知识点的讲解中：

| 层面 | 来源 | 作用 |
|------|------|------|
| 设计原则 | C++ Core Guidelines | 回答"为什么这么写"（如 C.21: 要么不定义，要么 =default 全部） |
| 执行细则 | Google C++ Style Guide | 回答"具体怎么写"（如命名规则、include 顺序、注释格式） |

每个模块的 `compare.md` 会把相关的规范规则编号直接列在知识点旁边，示例代码和习题解答严格遵循这两套规范。

## 整体架构

11 个主题模块（按工作依赖排序） + 1 个综合工作场景。

## 模块列表

| 序号 | 模块 | 工作场景 | 核心内容 |
|------|------|----------|----------|
| 1 | 前置基础 | 理解 C++ 独有的基础概念 | 编译模型(.h/.cpp)、预处理器、基本类型大小、初始化语法、static 三种含义、enum class、类型转换、C 数组 vs std::array、main 签名 |
| 2 | 基础语法差异 | 第一天上班看代码 | 引用/指针、const、auto、namespace、默认参数/函数重载、range-for |
| 3 | 内存管理 | 实现一个功能不出内存问题 | 栈 vs 堆、new/delete、unique_ptr/shared_ptr/weak_ptr、RAII、移动语义、悬垂指针 |
| 4 | OOP 对比 | 设计/修改一个类 | 构造/析构、继承/虚函数、抽象类、访问控制、多重继承 vs 接口、final/override |
| 5 | 值语义与运算符 | 写一个可复用的工具类 | 运算符重载、拷贝/移动构造、三五法则、值语义 vs 引用语义 |
| 6 | STL 容器与算法 | 处理数据集合 | vector/map/set/unordered_map、迭代器、算法(sort/find/transform)、string/string_view |
| 7 | 模板与泛型 | 写一个通用工具 | 函数模板、类模板、模板特化、变参模板、concepts(C++20) |
| 8 | 现代 C++ 特性 | 提升代码质量和可读性 | lambda、constexpr、结构化绑定、optional/variant/any、fold expression |
| 9 | 错误处理 | 让代码在异常情况下健壮 | 异常 vs 错误码、noexcept、RAII 与异常安全 |
| 10 | 并发编程 | 处理多线程任务 | thread/mutex/lock_guard、atomic、async/future |
| 11 | 综合工作场景 | 独立完成一个完整需求 | 一个模拟真实任务的 CLI 应用 |

## 模块内部结构

每个模块统一使用以下目录结构：

```
module-01-basics/
├── compare.md          # Java vs C++ 对比 + 踩坑记录 + 编码规范对照
├── examples/           # 符合编码规范的示例代码
│   ├── 01_xxx.cpp
│   └── ...
├── exercises/          # 工作任务式习题
│   ├── 01_xxx.md       # 需求描述（含规范检查清单）
│   ├── 01_xxx.cpp      # 骨架代码，留空让学生实现
│   └── solution/
│       ├── 01_xxx.cpp  # 参考解答（含规范注释标注）
│       └── ...
└── CMakeLists.txt
```

## 内容编写规范

### compare.md 格式

每个知识点四元组（对比 + 坑 + 规范）：

```markdown
### 知识点：引用 vs 指针

| 场景 | Java | C++ | 踩坑提醒 |
|------|------|-----|----------|
| xxx  | xxx  | xxx | ⚠️ xxx  |

### 相关编码规范

| 规范来源 | 编号 | 规则 | 说明 |
|----------|------|------|------|
| C++ Core Guidelines | F.15 | 优先用传值/传引用，少用裸指针 | 可读性更好，所有权更明确 |
| C++ Core Guidelines | F.22 | 用 T* 或 owner<T*> 表示单个对象 | 区分单个对象和数组 |
| Google Style Guide | 引用参数 | 所有引用参数必须加 const，可变的用指针 | const T& 输入，T* 输出 |

### 常见 Bug（Code Review 回退清单）
- [ ] 返回了局部变量的引用 —— 悬垂引用，必崩
- [ ] 函数参数忘记加 const& —— 不必要的拷贝
```

### 编码规范集成规则

- **每个示例文件开头注释**：标注本文件涉及的 Core Guidelines 编号
- **习题需求文档**：附带"编码规范检查清单"，完成代码后自检
- **答案文件注释**：关键行标注"// Core Guideline C.21"，让学生理解为什么这么写
- **违反规范的代码**：部分示例有意展示反例并标注"❌ Code Review 会直接打回"

### exercises 格式

```markdown
# 任务：实现一个线程安全的计数器

## 需求背景
模块需要统计请求数量，多线程环境下需要确保计数准确。

## 需求描述
实现 `ThreadSafeCounter` 类：
- `increment()` 方法，线程安全地 +1
- `get()` 方法，返回当前值
- 支持移动语义（计数器不可拷贝）

## 编码规范检查清单
- [ ] 类名大驼峰，方法名大驼峰（Google Style）
- [ ] 成员变量名以 _ 结尾？（如有团队规范）
- [ ] 是否违反 Core Guideline C.21（五法则）？
- [ ] 析构函数是否需要 virtual？

## 验收标准
- [ ] 多线程 increment 后 get 结果正确
- [ ] 不可拷贝（编译期阻止）
- [ ] 通过规范检查清单
```

### examples 格式

```cpp
// 模块 2 示例：unique_ptr 所有权转移
// 涉及规范：Core Guidelines R.20, R.30; Google Style: Ownership and Smart Pointers
//
// ✅ 正确写法
```

## 构建系统

- 根目录一个 `CMakeLists.txt`，`add_subdirectory` 管理所有模块
- C++ 标准：C++17
- 编译命令：`cmake -B build && cmake --build build`
- 环境搭建不在学习范围内

## 文件命名规范

- 示例文件：`01_关键词.cpp`
- 习题骨架：`01_关键词.cpp`
- 答案文件：`solution/01_关键词.cpp`
- 需求文档：`01_关键词.md`

## 技术选型

- **C++ 标准**：C++17（基线），C++20 亮点标注
- **构建**：CMake 3.16+
- **编译器**：GCC 9+ / Clang 10+ / MSVC 2019+
- **代码风格**：Google C++ Style Guide
- **设计规范**：C++ Core Guidelines

## 不在范围内

- 开发环境安装（IDE、编译器安装）
- 操作系统特定内容
- 第三方库（Boost、Qt 等）
- 编译原理、链接细节
- 性能优化专题
