# 习题 04：只允许移动的类

## 目标

设计一个"只允许移动、禁止拷贝"的类，实践 `=default` 和 `=delete` 的使用。

## 要求

实现 `UniqueResource` 类：

```cpp
class UniqueResource {
    int* data_;
    size_t size_;
public:
    explicit UniqueResource(size_t n);
    ~UniqueResource();

    // 禁止拷贝
    UniqueResource(const UniqueResource&) = delete;
    UniqueResource& operator=(const UniqueResource&) = delete;

    // 允许移动
    UniqueResource(UniqueResource&& other) noexcept;
    UniqueResource& operator=(UniqueResource&& other) noexcept;

    // 访问器
    size_t size() const;
    int& operator[](size_t index);
    const int& operator[](size_t index) const;

    // 交换
    void swap(UniqueResource& other) noexcept;

    // 释放资源（返回指针，调用者负责释放）
    int* release() noexcept;
};
```

## 要求验证

1. 尝试拷贝构造/赋值应产生编译错误（注释掉相关代码）
2. 移动构造/赋值正常工作
3. `operator[]` 提供元素访问
4. `swap` 高效交换两个对象
5. `release` 放弃所有权，返回原始指针

## 提示

- `=delete` 的拷贝操作在编译时阻止误用
- `operator[]` 需要 const 和非 const 两个版本
- `release` 后对象应处于空状态
- `std::swap` 需要 `#include <utility>`

## 预期输出

```
=== Create ===
Created with size 5

=== Access via operator[] ===
res[0] = 10
res[4] = 50

=== Move Constructor ===
Moved res1 to res2
res1 is empty: true
res2 size: 5

=== Move Assignment ===
res3 = std::move(res2)
res2 is empty: true
res3 size: 5

=== Swap ===
Before: res3 size=5, res4 size=3
After:  res3 size=3, res4 size=5

=== Release ===
Released from res3, res3 is empty: true
```
