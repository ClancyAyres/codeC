// Exercise 03: Lock-Free Counter
// TODO: 实现一个线程安全的计数器（原子操作 + CAS）

#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <iomanip>

// ── 任务1: 基本原子计数器 ─────────────────────────────────────────

// TODO: 实现线程安全的递增/递减/获取/重置操作
class AtomicCounter {
public:
    AtomicCounter(int initial = 0);

    // TODO: 原子递增
    int increment();

    // TODO: 原子递减
    int decrement();

    // TODO: 获取当前值
    int get() const;

    // TODO: 重置为指定值
    void reset(int value = 0);

private:
    // TODO: std::atomic<int> value_;
};

// ── 任务2: 无锁上限计数器 ─────────────────────────────────────────

// TODO: 实现一个不超过 maxValue 的计数器
// - increment() 用 CAS 循环实现，达到上限后保持不变
class BoundedLockFreeCounter {
public:
    BoundedLockFreeCounter(int maxValue);

    // TODO: 尝试递增（CAS 循环），若已达上限则返回 false
    bool tryIncrement();

    // TODO: 获取当前值
    int get() const;

    // TODO: 重置
    void reset();

private:
    // TODO: std::atomic<int> value_ 和 int maxValue_
};

// ── 测试代码 ──────────────────────────────────────────────────────

int main() {
    std::cout << "=== 无锁计数器 ===\n";
    std::cout << "Hardware concurrency: "
              << std::thread::hardware_concurrency() << "\n\n";

    // 测试 AtomicCounter
    {
        std::cout << "--- AtomicCounter ---\n";
        AtomicCounter counter;
        constexpr int numThreads = 4;
        constexpr int increments = 100'000;
        std::vector<std::thread> threads;

        for (int i = 0; i < numThreads; ++i)
            threads.emplace_back([&counter] {
                for (int j = 0; j < increments; ++j)
                    counter.increment();
            });

        for (auto& t : threads) t.join();

        int expected = numThreads * increments;
        std::cout << "并发递增 " << numThreads << "×" << increments
                  << " = " << counter.get()
                  << " (期望: " << expected << ")\n";

        // TODO: 测试递减
        // TODO: 测试重置
    }

    // 测试 BoundedLockFreeCounter
    {
        std::cout << "\n--- BoundedLockFreeCounter ---\n";
        BoundedLockFreeCounter bounded(100);
        constexpr int numThreads = 8;
        constexpr int attempts = 50;
        std::vector<std::thread> threads;

        for (int i = 0; i < numThreads; ++i)
            threads.emplace_back([&bounded] {
                for (int j = 0; j < attempts; ++j)
                    bounded.tryIncrement();
            });

        for (auto& t : threads) t.join();

        std::cout << "8×50 次 tryIncrement(上限 100) = "
                  << bounded.get() << " (期望: 100)\n";
    }

    return 0;
}
