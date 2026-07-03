// Exercise 03: Lock-Free Counter — 完整解法

#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>

class AtomicCounter {
public:
    explicit AtomicCounter(int initial = 0)
        : value_(initial) {}

    int increment() {
        return value_.fetch_add(1, std::memory_order_relaxed) + 1;
    }

    int decrement() {
        return value_.fetch_sub(1, std::memory_order_relaxed) - 1;
    }

    int get() const {
        return value_.load(std::memory_order_relaxed);
    }

    void reset(int value = 0) {
        value_.store(value, std::memory_order_relaxed);
    }

private:
    std::atomic<int> value_;
};

class BoundedLockFreeCounter {
public:
    explicit BoundedLockFreeCounter(int maxValue)
        : maxValue_(maxValue), value_(0) {}

    bool tryIncrement() {
        int old = value_.load(std::memory_order_relaxed);
        while (true) {
            if (old >= maxValue_)
                return false;
            if (value_.compare_exchange_weak(old, old + 1,
                    std::memory_order_relaxed, std::memory_order_relaxed))
                return true;
        }
    }

    int get() const {
        return value_.load(std::memory_order_relaxed);
    }

    void reset() {
        value_.store(0, std::memory_order_relaxed);
    }

private:
    std::atomic<int> value_;
    int maxValue_;
};

int main() {
    std::cout << "=== 无锁计数器 ===\n";
    std::cout << "Hardware concurrency: "
              << std::thread::hardware_concurrency() << "\n\n";

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
        int result = counter.get();
        std::cout << "并发递增 " << numThreads << "×" << increments
                  << " = " << result
                  << " (期望: " << expected << ") "
                  << (result == expected ? "PASS" : "FAIL") << '\n';

        counter.reset();
        std::cout << "重置后: " << counter.get() << '\n';

        counter.increment();
        counter.decrement();
        std::cout << "递增后递减: " << counter.get() << " (期望: 0)\n";
    }

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

        int result = bounded.get();
        std::cout << numThreads << "×" << attempts
                  << " 次 tryIncrement(上限 100) = "
                  << result << " (期望: 100) "
                  << (result == 100 ? "PASS" : "FAIL") << '\n';
    }

    return 0;
}
