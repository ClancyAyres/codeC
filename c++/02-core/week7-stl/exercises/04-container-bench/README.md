# 习题 04：容器性能基准测试 — vector vs list vs deque

## 目标

编写基准测试比较 `std::vector`、`std::list`、`std::deque` 在不同操作下的性能差异，验证理论复杂度。

## 要求

实现以下基准测试函数：

```cpp
// 测试尾部追加 N 个元素
void benchPushBack(size_t N);

// 测试头部插入 N 个元素
void benchPushFront(size_t N);

// 测试随机访问 N 次
void benchRandomAccess(size_t N);

// 测试遍历所有元素
void benchIteration(size_t N);

// 测试在中间位置插入 N 个元素
void benchInsertMiddle(size_t N);
```

## 提示

- 使用 `std::chrono::high_resolution_clock` 计时
- 每个测试输出 vector / list / deque 各自的耗时
- 对 `std::list` 的随机访问用 `std::advance` 模拟（体现 O(n)）
- 使用足够大的 N 以观察明显差异（建议 N = 100,000）

## 预期输出格式

```
=== push_back (N=100000) ===
vector:  X ms
list:    X ms
deque:   X ms

=== push_front (N=100000) ===
vector:  X ms  (O(N) — slow!)
list:    X ms
deque:   X ms  (O(1) — fast)

=== random_access (N=100000) ===
vector:  X ms  (O(1) — fast)
list:    X ms  (O(N) — very slow!)
deque:   X ms
...
```
