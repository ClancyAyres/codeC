# 习题 04：模板特化 — const char* 比较

## 目标

实现一个 `compare` 函数模板，并针对 `const char*` 进行全特化，使其比较字符串内容而非指针地址。

## 要求

```cpp
// 通用版本：使用 < 和 > 比较
template<typename T>
int compare(const T& a, const T& b);

// 特化版本：使用 strcmp 比较字符串内容
template<>
int compare<const char*>(const char* const& a, const char* const& b);
```

- `compare(a, b)` 返回：
  - 负数：a < b
  - 0：a == b
  - 正数：a > b

## 提示

- 通用版本用 `(a < b) ? -1 : (a > b) ? 1 : 0`
- 特化版本用 `std::strcmp(a, b)`
- 需要 `#include <cstring>` 获取 `std::strcmp`

## 预期输出

```
=== Generic (int) ===
compare(3, 5) = -1
compare(5, 3) = 1
compare(3, 3) = 0

=== Generic (double) ===
compare(3.14, 2.72) = 1
compare(1.0, 2.0) = -1

=== Specialized (const char*) ===
compare("apple", "banana") = -1  (content: "apple" < "banana")
compare("hello", "hello") = 0   (content: equal)
compare("zebra", "apple") = 1   (content: "zebra" > "apple")
```
