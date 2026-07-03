// Example 04: Thread-Safe Singleton — Meyer's Singleton + call_once
// 演示三种线程安全初始化方式

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>

// ── 方案1: Meyer's Singleton (C++11) — 推荐 ──────────────────────

class MeyersSingleton {
public:
    static MeyersSingleton& getInstance() {
        static MeyersSingleton instance;  // C++11 保证线程安全初始化
        return instance;
    }

    void increment() { ++counter_; }
    int value() const { return counter_; }

    MeyersSingleton(const MeyersSingleton&) = delete;
    MeyersSingleton& operator=(const MeyersSingleton&) = delete;
    MeyersSingleton(MeyersSingleton&&) = delete;
    MeyersSingleton& operator=(MeyersSingleton&&) = delete;

private:
    MeyersSingleton() = default;
    int counter_ = 0;
};

// ── 方案2: std::call_once — 显式一次性初始化 ─────────────────────

class CallOnceSingleton {
public:
    static CallOnceSingleton& getInstance() {
        std::call_once(init_flag_, [] {
            instance_.reset(new CallOnceSingleton());
        });
        return *instance_;
    }

    void increment() { ++counter_; }
    int value() const { return counter_; }

    CallOnceSingleton(const CallOnceSingleton&) = delete;
    CallOnceSingleton& operator=(const CallOnceSingleton&) = delete;

private:
    CallOnceSingleton() = default;
    int counter_ = 0;

    static std::once_flag init_flag_;
    static std::unique_ptr<CallOnceSingleton> instance_;
};

std::once_flag CallOnceSingleton::init_flag_;
std::unique_ptr<CallOnceSingleton> CallOnceSingleton::instance_;

// ── 方案3: 双重检查锁定 (DCLP) — 复杂易错，不推荐 ───────────────

class DCLPSingleton {
public:
    static DCLPSingleton& getInstance() {
        DCLPSingleton* tmp = instance_.load(std::memory_order_acquire);
        if (!tmp) {
            std::lock_guard<std::mutex> lock(mutex_);
            tmp = instance_.load(std::memory_order_relaxed);
            if (!tmp) {
                tmp = new DCLPSingleton();
                instance_.store(tmp, std::memory_order_release);
            }
        }
        return *tmp;
    }

    void increment() { ++counter_; }
    int value() const { return counter_; }

    DCLPSingleton(const DCLPSingleton&) = delete;
    DCLPSingleton& operator=(const DCLPSingleton&) = delete;

private:
    DCLPSingleton() = default;
    int counter_ = 0;

    static std::atomic<DCLPSingleton*> instance_;
    static std::mutex mutex_;
};

std::atomic<DCLPSingleton*> DCLPSingleton::instance_{nullptr};
std::mutex DCLPSingleton::mutex_;

// ── 测试：多线程同时访问单例 ─────────────────────────────────────

template<typename Singleton>
void testSingleton(const char* name) {
    std::cout << "\n── " << name << " ──\n";

    constexpr int numThreads = 10;
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([i] {
            auto& inst = Singleton::getInstance();
            inst.increment();
            std::cout << "  线程 " << i << " 获得实例地址: " << &inst << '\n';
        });
    }

    for (auto& t : threads) t.join();

    std::cout << "  最终计数: " << Singleton::getInstance().value()
              << " (期望: " << numThreads << ")\n";
}

// ── main ──────────────────────────────────────────────────────────

int main() {
    std::cout << "=== 线程安全单例 ===\n";
    std::cout << "Hardware concurrency: "
              << std::thread::hardware_concurrency() << '\n';

    testSingleton<MeyersSingleton>("Meyer's Singleton (推荐)");
    testSingleton<CallOnceSingleton>("std::call_once");
    testSingleton<DCLPSingleton>("双重检查锁定 (不推荐)");

    std::cout << "\n所有地址应相同，计数应为 10\n";
    return 0;
}
