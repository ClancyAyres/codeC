# 习题 02：打印任意容器 — 模板函数

## 目标

实现一个模板函数 `printContainer`，能够打印任意容器的元素。

## 要求

```cpp
// 打印容器所有元素，用分隔符分隔
template<typename Container>
void printContainer(const Container& c, const char* separator = " ");

// 示例用法：
std::vector<int> v = {1, 2, 3};
printContainer(v);         // 输出: 1 2 3
printContainer(v, ", ");   // 输出: 1, 2, 3
```

## 提示

- 使用 `std::begin(c)` 和 `std::end(c)` 迭代（支持 C 数组和 STL 容器）
- 第一个元素前不加分隔符
- 末尾不输出多余分隔符

## 预期输出

```
=== vector<int> ===
1 2 3 4 5
1, 2, 3, 4, 5

=== vector<string> ===
hello world C++ templates

=== list<double> ===
3.14 2.72 1.41

=== Custom Array ===
10 20 30 40
```
