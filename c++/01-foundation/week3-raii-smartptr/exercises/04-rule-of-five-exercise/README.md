# 习题 04：实现拷贝和移动语义

## 目标

为 `StringArray` 类实现完整的规则五（Rule of Five）操作。

## 背景

`StringArray` 是一个管理动态分配字符串数组的类。骨架代码已经提供了构造函数和析构函数，你需要实现拷贝和移动操作。

## 要求

实现以下特殊成员函数：

```cpp
class StringArray {
    size_t size_;
    std::string* data_;

public:
    explicit StringArray(size_t size);           // 已提供
    ~StringArray();                               // 已提供

    // TODO: 实现以下五个函数
    StringArray(const StringArray& other);       // 拷贝构造 — 深拷贝
    StringArray& operator=(const StringArray& other); // 拷贝赋值 — 深拷贝，处理自赋值
    StringArray(StringArray&& other) noexcept;   // 移动构造 — 接管资源
    StringArray& operator=(StringArray&& other) noexcept; // 移动赋值 — 释放旧资源，接管新资源

    // 访问器（已提供）
    size_t size() const;
    std::string& operator[](size_t i);
    const std::string& operator[](size_t i) const;
};
```

## 行为要求

- **拷贝构造**：分配新数组，逐元素拷贝
- **拷贝赋值**：处理自赋值，分配新数组，释放旧数组，逐元素拷贝
- **移动构造**：接管源对象的 `data_` 和 `size_`，源对象 `data_` 置 `nullptr`，`size_` 置 0
- **移动赋值**：检查自赋值，释放旧 `data_`，接管源对象资源，源对象重置
- 析构函数输出 `[dtor] StringArray(size=N) destroyed`

## 提示

- 用 `std::copy` 进行元素拷贝
- 用 `std::exchange` 实现移动操作
- 拷贝赋值中先分配新数组再释放旧数组，确保异常安全
- 移动操作标记 `noexcept`

## 预期输出

```
[ctor] StringArray(size=3) constructed
[copy-ctor] StringArray(size=3) copied
[copy-assign] StringArray(size=4) <- StringArray(size=3)
[move-ctor] StringArray(size=3) moved
[move-assign] StringArray(size=3) <- StringArray(size=3)
Self-assignment handled correctly
[dtor] StringArray(size=3) destroyed
[dtor] StringArray(size=0) destroyed
[dtor] StringArray(size=3) destroyed
[dtor] StringArray(size=0) destroyed
[dtor] StringArray(size=3) destroyed
```
