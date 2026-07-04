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

## 常见 Bug（Code Review 回退清单）

- [ ] `for (auto item : vec)` 忘记 `&` — 默默拷贝每个元素
- [ ] 函数参数类型是 `T` 而非 `const T&` — 不必要的深拷贝
- [ ] 返回局部变量的引用或指针 — 悬垂引用，运行时崩溃
- [ ] 使用未初始化的指针 — 未定义行为
- [ ] `using namespace std;` 写在头文件 — 污染全局命名空间
