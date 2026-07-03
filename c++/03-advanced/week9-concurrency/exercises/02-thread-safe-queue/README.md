# 习题 02：线程安全阻塞队列

## 目标

实现 `BoundedBlockingQueue<T>` —— 一个有限容量的线程安全阻塞队列。

## 要求

```cpp
template<typename T>
class BoundedBlockingQueue {
public:
    BoundedBlockingQueue(size_t capacity);
    void push(T item);        // 阻塞入队（满时等待）
    T pop();                  // 阻塞出队（空时等待）
    bool tryPop(T& item);     // 非阻塞出队（空时返回 false）
    size_t size();            // 线程安全获取大小
    bool isEmpty();           // 是否为空
    bool isFull();            // 是否已满
};
```

- 使用 `std::mutex` + 两个 `std::condition_variable`（`notEmpty_`, `notFull_`）
- `push()` 满时阻塞等待 `notFull_`
- `pop()` 空时阻塞等待 `notEmpty_`
- `tryPop()` 空时立即返回 `false`
- 所有方法线程安全

## 提示

- 内部使用 `std::queue<T>` 和 `std::mutex`
- `wait` 始终用 predicate 版本
- `push` 通知 `notEmpty_`，`pop` 通知 `notFull_`

## 预期输出

```
[生产] 1 (队列大小: 1)
[生产] 2 (队列大小: 2)
[生产] 3 (队列大小: 3)
  [消费] 1 (队列大小: 2)
[生产] 4 (队列大小: 3)
...
[生产] 完成
  [消费] 完成
最终队列大小: 0 (期望: 0)
非阻塞出队正确返回 false
```
