# 习题 01：Stack<T> 类模板

## 目标

实现一个泛型 `Stack<T>` 类模板，支持 push、pop、top、empty 操作。

## 要求

```cpp
template<typename T>
class Stack {
    // 内部用动态数组存储元素
    // 初始容量为 4，push 满时扩容为 2 倍
public:
    Stack();                          // 默认构造
    ~Stack();                         // 析构
    void push(const T& value);        // 压栈
    void pop();                       // 出栈（不返回值）
    T& top();                         // 返回栈顶引用
    const T& top() const;             // const 版本
    bool empty() const;               // 是否为空
    size_t size() const;              // 当前元素数
};
```

## 预期行为

```
Stack<int> s;
s.push(10);
s.push(20);
s.push(30);
s.top() == 30
s.pop()
s.top() == 20
s.size() == 2
s.empty() == false
```

## 提示

- 内部维护 `T* data_`, `size_t size_`, `size_t capacity_`
- pop 只需 --size_（不释放内存）
- 扩容时分配新数组、移动旧元素、释放旧数组
- 不需要拷贝/移动构造（默认行为可能出错，但习题简化）

## 预期输出

```
=== Push ===
Pushed 10, 20, 30
Top: 30
Size: 3
Empty: false

=== Pop ===
Popped. New top: 20
Size: 2

=== Pop All ===
Popped: 20
Popped: 10
Stack empty: true
Size: 0

=== Re-push ===
Pushed 100, 200
Top: 200
```
