# 习题 03：无锁计数器

## 目标

使用 `std::atomic` 实现两个线程安全计数器，理解原子操作与 CAS（compare-and-swap）。

## 要求

### 1. AtomicCounter

```cpp
class AtomicCounter {
public:
    AtomicCounter(int initial = 0);
    int increment();             // 返回递增后的值
    int decrement();             // 返回递减后的值
    int get() const;             // 获取当前值
    void reset(int value = 0);   // 重置
};
```

- 使用 `std::atomic<int>` 实现
- `increment()` / `decrement()` 使用 `fetch_add(1)` / `fetch_sub(1)`
- 建议用 `memory_order_relaxed`（纯计数无需强同步）

### 2. BoundedLockFreeCounter

```cpp
class BoundedLockFreeCounter {
public:
    BoundedLockFreeCounter(int maxValue);
    bool tryIncrement();   // CAS 循环递增，达上限返回 false
    int get() const;
    void reset();
};
```

- 使用 CAS (`compare_exchange_weak`) 循环实现 `tryIncrement()`
- 如果当前值 < maxValue，则尝试递增；否则返回 false
- **CAS 循环模式**：

```cpp
int old = value_.load();
do {
    if (old >= maxValue_) return false;
} while (!value_.compare_exchange_weak(old, old + 1));
```

## 提示

- `compare_exchange_weak` 可能有伪失败，必须在循环中使用
- `fetch_add(1)` 返回的是旧值，返回值+1 才是递增后
- `atomic` 不可拷贝，不可移动

## 预期输出

```
并发递增 4×100000 = 400000 (期望: 400000)
8×50 次 tryIncrement(上限 100) = 100 (期望: 100)
```
