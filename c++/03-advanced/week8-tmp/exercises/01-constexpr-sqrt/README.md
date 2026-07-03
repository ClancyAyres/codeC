# 习题 01：constexpr sqrt — 编译时平方根

## 目标

实现编译时整数平方根：`constexpr int sqrt(int n)` 返回 ⌊√n⌋（向下取整）。

## 要求

```cpp
constexpr int sqrt(int n) {
    // TODO: 使用二分查找或牛顿迭代（编译时可用的循环/递归）
    // 0 和 1 直接返回自身
    // 返回最大的整数 r 满足 r*r <= n
}
```

## 预期行为

```
sqrt(0)  → 0
sqrt(1)  → 1
sqrt(4)  → 2
sqrt(9)  → 3
sqrt(16) → 4
sqrt(17) → 4
sqrt(25) → 5
sqrt(100)→ 10
sqrt(1000) → 31
```

## 提示

- 用 `while (true)` 或 `for` 循环，C++17 constexpr 支持循环
- 二分查找：`low = 0, high = n`，`mid = (low + high) / 2`
- 记得用 `static_assert` 验证编译时行为

## 进阶

在 `main.cpp` 内用 `static_assert` 确认一些值，包括把 sqrt 结果用作数组大小：

```cpp
constexpr int s = sqrt(100);
int arr[s]; // arr[10]
```
