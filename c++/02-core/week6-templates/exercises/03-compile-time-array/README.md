# 习题 03：编译时数组 — 非类型模板参数

## 目标

实现一个 `CompileArray<T, N>` 类模板，数组大小在编译时确定。

## 要求

```cpp
template<typename T, size_t N>
class CompileArray {
    T data_[N];   // 栈上分配，编译时确定大小
public:
    // 默认构造：所有元素用 T{} 初始化
    CompileArray();

    // 从初始化列表构造
    CompileArray(std::initializer_list<T> init);

    // 下标访问
    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    // 获取大小
    constexpr size_t size() const;

    // begin/end 迭代器支持
    T* begin();
    T* end();
    const T* begin() const;
    const T* end() const;

    // 填充
    void fill(const T& value);
};
```

## 提示

- `size()` 返回 `N`（编译时常量，用 `constexpr`）
- `operator[]` 不做边界检查（与 C 数组行为一致）
- `begin()` 返回 `data_`，`end()` 返回 `data_ + N`
- `fill()` 用循环赋值

## 预期输出

```
=== Default Construction ===
arr1 size: 5
arr1 elements: 0 0 0 0 0

=== Initializer List ===
arr2 size: 3
arr2 elements: 10 20 30

=== Fill ===
After fill(7): 7 7 7 7 7

=== Element Access ===
arr2[0] = 10
arr2[2] = 30

=== Range-based For ===
10 20 30

=== Compile-time Size ===
CompileArray<int, 5> and CompileArray<int, 10> are different types!
```
