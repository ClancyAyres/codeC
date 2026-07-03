# 习题 02：用 unique_ptr 实现单向链表

## 目标

用 `std::unique_ptr` 实现一个单向链表，理解独占所有权在数据结构中的应用。

## 要求

实现以下结构和方法：

```cpp
struct Node {
    int value;
    std::unique_ptr<Node> next;

    explicit Node(int val) : value(val), next(nullptr) {}
};

class IntList {
    std::unique_ptr<Node> head_;
    Node* tail_;       // 非拥有型裸指针，用于 O(1) 尾插

public:
    IntList();                     // 空链表
    ~IntList();                    // 不需要显式实现（unique_ptr 自动递归释放）

    void push_back(int value);     // 在尾部插入
    void push_front(int value);    // 在头部插入
    int  front() const;            // 返回头部值
    bool empty() const;            // 链表是否为空
    void print() const;            // 打印所有元素
};
```

## 行为要求

- `push_back`：创建新节点，追加到尾部，更新 `tail_`
- `push_front`：创建新节点，插入头部（注意维护 `tail_`）
- `front()`：返回头部值；如果链表为空则抛出 `std::runtime_error`
- 析构函数不需要显式实现 — `unique_ptr` 的递归析构会自动释放所有节点

## 提示

- 头插时如果链表原来为空，需要同时更新 `tail_`
- 尾插时如果链表原来为空，需要同时更新 `head_`
- 用 `std::make_unique<Node>(value)` 创建节点
- 用 `std::move` 转移 `unique_ptr` 所有权

## 预期输出

```
List after push_back: 10 20 30
List after push_front: 5 10 20 30
List after mixed ops: 1 5 10 20 30 99
front: 1 (expect 1)
empty: 0 (expect 0)
```
