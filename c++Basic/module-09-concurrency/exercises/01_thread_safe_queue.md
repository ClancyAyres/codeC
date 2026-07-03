# 任务: 线程安全队列

## 需求背景

生产者-消费者模式需要一个线程安全的队列。

## 需求描述

实现 `ThreadSafeQueue<T>`:
- `push(T value)` 入队
- `bool tryPop(T& out)` 尝试出队（非阻塞）
- `T pop()` 阻塞出队（队列空时等待）
- `bool empty() const`
- `size_t size() const`

使用 `std::mutex` + `std::condition_variable`。

## 验收标准
- [ ] 多生产者多消费者测试通过
- [ ] 所有元素都被正确处理
