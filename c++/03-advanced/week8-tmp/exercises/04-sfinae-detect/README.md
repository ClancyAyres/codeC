# 习题 04：SFINAE Detect — 检测成员函数是否存在

## 目标

使用 SFINAE + `void_t` 检测类型是否有特定成员函数（`begin()`, `end()`, `size()`, `reserve(size_t)`）。

## 要求

```cpp
// 检测是否有 begin() 和 end()（即为可迭代类型）
template<typename, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<
    decltype(std::declval<T&>().begin()),
    decltype(std::declval<T&>().end())
>> : std::true_type {};

// TODO: 实现 has_size   — 检测 T 是否有 size() 方法
// TODO: 实现 has_reserve — 检测 T 是否有 reserve(size_t) 方法
```

然后实现重载函数 `inspect(T)`：
- 如果可迭代 → 打印元素用 `[` `]` 包裹
- 如果有 size() → 打印 size
- 如果有 reserve() → 标注 "has reserve"

## 预期行为

```
std::vector<int> v{1,2,3};
inspect(v);
  → [vector] iterable: [1, 2, 3]  size: 3  reserve: yes

std::string s = "hi";
inspect(s);
  → [string] iterable: [h, i]  size: 2  reserve: yes

int x = 42;
inspect(x);
  → [int] not iterable  size: no  reserve: no
```

## 提示

- 参考 `example/03-enable-if-overload/main.cpp` 中的 `is_iterable`
- `has_reserve` 需要 `decltype(std::declval<T&>().reserve(std::declval<size_t>()))`
- 用 `if constexpr` 组合所有检测结果
