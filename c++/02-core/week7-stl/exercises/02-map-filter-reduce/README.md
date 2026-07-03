# 习题 02：Map/Filter/Reduce — 用 `<algorithm>` 实现

## 目标

使用 `<algorithm>` 和 `<numeric>` 实现 map、filter、reduce 三个基本操作，体会 C++ 迭代器风格与 Java Stream 的差异。

## 要求

实现三个函数模板：

```cpp
// filter: 保留满足 predicate 的元素
template<typename Container, typename Predicate>
Container filter(const Container& c, Predicate pred);

// map: 对每个元素应用 transform 函数
template<typename Container, typename UnaryOp>
auto map(const Container& c, UnaryOp op) -> std::vector<decltype(op(*c.begin()))>;

// reduce: 从初始值开始累加
template<typename Container, typename T, typename BinaryOp>
T reduce(const Container& c, T init, BinaryOp op);
```

## 示例用法

```cpp
std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

auto evens = filter(numbers, [](int n) { return n % 2 == 0; });
// evens = {2, 4, 6, 8, 10}

auto squares = map(evens, [](int n) { return n * n; });
// squares = {4, 16, 36, 64, 100}

auto sum = reduce(squares, 0, std::plus<int>());
// sum = 220
```

## 预期输出

```
=== Filter ===
Evens: 2 4 6 8 10
Words with len > 3: hello world algorithm

=== Map ===
Squares: 4 16 36 64 100
Uppercase: HELLO WORLD

=== Reduce ===
Sum of squares: 220
Product: 3840

=== Chained ===
Filter(>5) → Map(*10) → Sum = 400
```
