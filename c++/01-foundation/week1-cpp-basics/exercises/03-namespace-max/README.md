# 习题 03：命名空间 max

## 目标

用命名空间实现两个版本的 `max` 函数，理解命名空间如何隔离同名符号。

## 要求

1. 在 `math` 命名空间中定义 `max(int, int)`，返回较大值
2. 在 `text` 命名空间中定义 `max(const char*, const char*)`，返回较长的字符串
3. 在全局命名空间中定义一个不同的 `max(int, int)`（例如返回两者之和）
4. 在 `main` 中演示三种调用方式：
   - 完全限定名（`math::max()`, `text::max()`, `::max()`）
   - `using` 声明（`using math::max;`）
   - 观察当使用 `using` 声明后，全局 `::max` 如何被隐藏

## 提示

- 需要 `<cstring>` 来使用 `std::strlen`
- 需要 `<iostream>` 来输出结果
- 使用 `::max()` 访问全局命名空间的函数

## 期望输出参考

```
math::max(3, 5) = 5
text::max("hello", "hi") = hello
::max(3, 5) = 8    (或你定义的语义)
```
