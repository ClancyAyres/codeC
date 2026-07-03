# Week 8：模板元编程 — Java vs C++ 差异对照

## 一、编译期计算：C++ 独有

| 特性 | Java | C++ |
|------|------|-----|
| 编译期计算 | 不支持（注解处理器可生成代码，但非语言能力） | `constexpr` / 模板元编程 |
| 编译时 if | 不支持 | C++17 `if constexpr` |
| 非类型模板参数 | 不支持 | `template<int N>`, `template<auto V>` (C++17) |
| 变参模板 | 不支持 | `template<typename... Args>` (C++11) |
| 折叠表达式 | 不支持 | C++17 `(Args + ...)` |
| SFINAE | 不支持 | 模板替换失败非错误（核心机制） |
| type_traits | 有限（`instanceof`, `Class<?>`） | 完整的 `<type_traits>` 库 |

## 二、代码对照

### 2.1 编译时阶乘

```java
// Java: 只能用运行时循环/递归
public static int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}
// 无法在编译时确定值，不能用于数组大小或模板参数
```

```cpp
// C++: constexpr — 编译时计算
constexpr int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i) result *= i;
    return result;
}
int arr[factorial(5)];  // int arr[120] — 合法！
```

### 2.2 类型分发

```java
// Java: instanceof + 类型转换（运行时）
public static String describe(Object obj) {
    if (obj instanceof Integer i) return "int: " + i;
    if (obj instanceof Double d) return "double: " + d;
    return "unknown";
}
```

```cpp
// C++: if constexpr — 编译时分发，零开销
template<typename T>
std::string describe(T val) {
    if constexpr (std::is_integral_v<T>)    return "int: " + std::to_string(val);
    if constexpr (std::is_floating_point_v<T>) return "float: " + std::to_string(val);
    return "unknown";
}
```

### 2.3 变参函数

```java
// Java: 变参在运行时（本质是数组语法糖）
public static void printAll(String... args) {
    for (String s : args) System.out.print(s + " ");
}
// 类型擦除后为 String[]；不能跨不同类型
```

```cpp
// C++: 变参模板 — 编译时展开，类型安全
template<typename... Args>
void printAll(Args... args) {
    ((std::cout << args << ' '), ...);  // C++17 折叠表达式
}
// 每个参数独立类型，零开销展开
```

### 2.4 SFINAE（替换失败非错误）

```cpp
// C++ 独有：基于特征的条件编译
template<typename T>
auto length(const T& t) -> decltype(t.size(), size_t{}) {  // 有 .size() 的版本
    return t.size();
}

template<typename T>
size_t length(const T&) {  // 回退版本
    return 0;
}

// 编译器自动选择匹配的版本，不匹配的静默丢弃
```

## 三、快速参考卡

| 场景 | Java 做法 | C++ 做法 |
|------|----------|---------|
| 编译时计算 | 不支持 | `constexpr` / 模板元编程 |
| 类型信息 | `instanceof`, `getClass()` | `std::is_*<T>` type_traits |
| 条件编译 | `if (debug) ...` (JIT 可消除) | `if constexpr(...)` 保证丢弃 |
| 变参 | `T... args` (同类型，运行时) | `Args... args` (异类型，编译时) |
| 函数重载选择 | 运行时重载决议 | SFINAE + `enable_if` + `if constexpr` |
| 代码生成 | 注解处理器 | 模板实例化（类型安全的代码生成） |
| 报错质量 | 清晰 | 冗长（C++20 Concepts 大幅改善） |
