# 模块 6: 模板与泛型 — Java vs C++

## 核心差异

| 维度 | Java 泛型 | C++ 模板 |
|------|-----------|----------|
| 实现方式 | 类型擦除（运行时一个版本） | 编译期代码生成（每个类型独立编译） |
| 基本类型 | 不能用于泛型（需要装箱） | 可用于模板（`vector<int>` 直接支持） |
| 约束 | `<T extends Comparable<T>>` | C++17: SFINAE/static_assert; C++20: `concepts` |
| 运行时开销 | 装箱/拆箱开销 | 零开销抽象（编译后无痕迹） |
| 编译速度 | 快 | 慢（每个实例化都是独立编译单元） |
| 特化 | 不支持 | 模板特化（全特化/偏特化） |
| 变参 | 不支持 | `template<typename... Args>` |

## 知识点: 函数模板

| 场景 | Java | C++ |
|------|------|-----|
| 泛型方法 | `<T> T max(T a, T b)` | `template<typename T> T max(T a, T b)` |

## 知识点: 类模板

| 场景 | Java | C++ |
|------|------|-----|
| 泛型类 | `class Box<T>` | `template<typename T> class Box` |

## 相关编码规范

| 规范来源 | 编号 | 规则 |
|----------|------|------|
| C++ Core Guidelines | T.1 | 用模板提高代码抽象水平 |
| C++ Core Guidelines | T.3 | 用模板表达对多个参数类型都适用的算法 |
| C++ Core Guidelines | T.10 | 为所有模板参数指定概念 (concepts) |
| Google Style Guide | - | 只在少量文件使用复杂模板（模板元编程需审慎） |

## 常见 Bug

- [ ] 忘记 `typename` 关键字 — 依赖类型需要 `typename` 前缀
- [ ] 模板定义写在 .cpp 中 — 必须在头文件（或在编译单元可见处）
- [ ] 模板特化未声明在原始模板之后
- [ ] 变参模板递归缺少终止条件
