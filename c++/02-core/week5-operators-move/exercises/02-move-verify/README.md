# 习题 02：移动语义性能验证

## 目标

实现一个带移动语义的资源管理类，并验证移动语义的性能优势。

## 要求

实现 `Buffer` 类：

```cpp
class Buffer {
    int* data_;
    size_t size_;
public:
    explicit Buffer(size_t n);           // 分配 n 个 int 的内存
    ~Buffer();

    Buffer(const Buffer& other);         // 拷贝构造 — 深拷贝
    Buffer(Buffer&& other) noexcept;     // 移动构造 — 转移所有权

    Buffer& operator=(const Buffer& other);    // 拷贝赋值
    Buffer& operator=(Buffer&& other) noexcept; // 移动赋值

    size_t size() const;
};
```

## 验证步骤

1. 在拷贝/移动构造和赋值中打印消息，证明移动语义被调用
2. 使用 `std::vector<Buffer>` 对比拷贝和移动的性能
3. 观察 `vector` 扩容时的行为

## 提示

- 使用 `std::exchange` 简化移动操作
- 移动操作标记 `noexcept`
- 拷贝操作需要 `if (this != &other)` 自赋值保护

## 预期行为

```
=== Copy Test ===
  (copy constructor called — expensive!)
  (copy constructor called — expensive!)
  vector size: 3

=== Move Test ===
  (move constructor called — cheap!)
  (move constructor called — cheap!)
  vector size: 3

=== Move Assignment ===
  (move assignment called — cheap!)
  after move: buf1 is empty
```
