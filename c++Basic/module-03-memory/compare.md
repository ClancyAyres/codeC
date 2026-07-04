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
