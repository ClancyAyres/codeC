# 模块 7: 现代 C++ 特性 — C++11/14/17/20

## Lambda 表达式

| 场景 | Java | C++ | 踩坑提醒 |
|------|------|-----|----------|
| 基本语法 | `(x) -> x * 2` | `[](int x) { return x * 2; }` | ⚠️ `[]` 是捕获列表，Java 没有这个概念 |
| 捕获变量 | 只能捕获 effectively final | 灵活的捕获列表 `[=]` `[&]` `[this]` `[&x]` | ⚠️ 要理解值捕获 vs 引用捕获 |
| mutable | 不适用 | `[x]() mutable { x++; }` | ⚠️ mutable 让值捕获可修改 |

### 相关编码规范

| 规范来源 | 编号 | 规则 |
|----------|------|------|
| C++ Core Guidelines | ES.28 | 用 lambda 进行复杂的初始化 |
| Google Style Guide | - | 如果 lambda 超过 5 行，考虑改为命名函数 |

## constexpr

| 场景 | Java | C++ |
|------|------|-----|
| 编译期常量 | `static final` | `constexpr` — 可在编译期求值 |
| 编译期函数 | 不支持 | `constexpr int factorial(int n)` |
| 编译期 if | 不支持 | `if constexpr` (C++17) |

## 结构化绑定 (C++17)

```cpp
auto [x, y, z] = std::make_tuple(1, 2.0, "hello");
for (const auto& [key, value] : map) { ... }
```

## std::optional (C++17)

| 场景 | Java | C++ |
|------|------|-----|
| 可空值 | `Optional<T>` | `std::optional<T>` |
| 检查有无值 | `opt.isPresent()` | `opt.has_value()` |
| 获取值 | `opt.get()` | `*opt` 或 `opt.value()` |
| 默认值 | `opt.orElse(default)` | `opt.value_or(default)` |

## std::variant (C++17)

类型安全的 union，Java 没有直接对应物。使用 `std::visit` 和 lambda 进行模式匹配。

## 常见 Bug

- [ ] Lambda 引用捕获 `[&]` 导致悬垂引用
- [ ] `optional` 解引用前未检查 `has_value()`
- [ ] `constexpr` 函数中有 iostream 操作（编译期不可用）
