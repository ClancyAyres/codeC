# Week 6：模板基础 — Java vs C++ 差异对照

## 一、泛型 vs 模板核心差异

| 概念 | Java 泛型 | C++ 模板 |
|------|----------|---------|
| 实现机制 | 类型擦除（Type Erasure） | 代码生成（Code Generation） |
| 运行时信息 | 擦除后 `List<String>` 和 `List<Integer>` 是同一类型 | `vector<int>` 和 `vector<string>` 是完全不同的类型 |
| 基本类型参数 | 不支持（`List<int>` 不合法，需 `List<Integer>` 装箱） | 完全支持（`vector<int>` 合法且高效） |
| 编译产物 | 一份字节码，运行时所有参数化类型共用 | 每个实例化生成独立代码（可能导致代码膨胀） |
| 编译错误 | 相对清晰 | 模板错误消息极其冗长（嵌套模板可产生数百行错误） |
| 特化 | 不支持 | 支持全特化和偏特化 |
| 非类型参数 | 不支持 | 支持（`template<int N>`、`template<size_t N>`） |
| 模板模板参数 | 不支持 | 支持（`template<template<typename> class Container>`） |
| 变参模板 | 不支持 | C++11 起支持（`template<typename... Args>`） |
| 概念/约束 | 有限的边界约束（`<T extends Comparable<T>>`） | C++20 Concepts 提供编译时约束 |

## 二、代码对照

### 2.1 泛型最大值

```java
// Java: 泛型方法，类型擦除 + 边界
public static <T extends Comparable<T>> T max(T a, T b) {
    return a.compareTo(b) > 0 ? a : b;
}
// 调用：MathUtils.<Integer>max(3, 5)
// 运行时通过 compareTo 比较，无法用于基本类型 int
```

```cpp
// C++: 函数模板，编译时生成
template<typename T>
T max(T a, T b) {
    return a > b ? a : b;
}
// 调用：max(3, 5) 或 max<int>(3, 5)
// 编译时为 int 生成具体函数，直接使用 > 运算符
```

### 2.2 泛型容器

```java
// Java: 类型擦除
public class Box<T> {
    private T value;
    public void set(T v) { value = v; }
    public T get() { return value; }
}
// Box<Integer> 和 Box<String> 在运行时是同一个 Box 类
// T 在运行时被替换为 Object
```

```cpp
// C++: 代码生成
template<typename T>
class Box {
    T value;
public:
    void set(T v) { value = v; }
    T get() const { return value; }
};
// Box<int> 和 Box<string> 是两个完全不同的类
// 各自生成独立的机器码
```

### 2.3 编译时计算

```java
// Java: 不支持编译时计算（需运行时循环或查表）
public static int fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}
```

```cpp
// C++: 模板元编程 — 编译时计算
template<int N>
struct Fibonacci {
    static constexpr int value = Fibonacci<N-1>::value + Fibonacci<N-2>::value;
};

template<>
struct Fibonacci<0> { static constexpr int value = 0; };

template<>
struct Fibonacci<1> { static constexpr int value = 1; };

// Fibonacci<10>::value 在编译时计算为 55
```

### 2.4 模板错误消息

```cpp
// 考虑以下错误：
std::vector<std::string> v;
std::sort(v.begin(), v.end());  // 缺少比较器？迭代器类型不匹配？

// C++ 模板错误通常包含多层嵌套的模板实例化信息
// 例如 GCC 的错误消息可能超过 100 行
// 现代编译器（Clang、GCC 新版本）已显著改善，但仍是常见痛点
// C++20 Concepts 可通过约束在实例化前给出清晰错误
```

## 三、快速参考卡

| 场景 | Java 做法 | C++ 做法 |
|------|----------|---------|
| 泛型函数 | `<T extends Bound> T func(T t)` | `template<typename T> T func(T t)` |
| 泛型类 | `class Box<T>` | `template<typename T> class Box` |
| 基本类型容器 | `List<Integer>`（装箱开销） | `std::vector<int>`（零开销） |
| 编译时计算 | 不支持 | 模板元编程 / constexpr |
| 类型特化 | 不支持 | 全特化 / 偏特化 |
| 非类型参数 | 不支持 | `template<int N>` |
| 类型约束 | `<T extends Interface>` | C++20 `requires` / `concept` |
| 错误消息 | 相对简洁 | 冗长（C++20 Concepts 改善） |
| 代码膨胀 | 无（一份字节码） | 有（每个实例化独立代码） |
| 模板代码组织 | 无特殊要求 | 需放在头文件（或显式实例化） |
