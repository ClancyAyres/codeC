// Example 01: Thread Sum — 无锁→mutex→atomic 三版本求和对比
// 目地：展示数据竞争问题及两种正确修复方式

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <chrono>

constexpr int NUM_THREADS = 4;
constexpr int INCREMENTS_PER_THREAD = 100'000;
constexpr int EXPECTED = NUM_THREADS * INCREMENTS_PER_THREAD;

// ── 版本1: 无锁 — 存在 data race（UB），结果错误 ──────────────
int unsafe_counter = 0;

void unsafe_increment() {
    for (int i = 0; i < INCREMENTS_PER_THREAD; ++i) {
        ++unsafe_counter;  // data race — 未定义行为
    }
}

int run_unsafe() {
    unsafe_counter = 0;
    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; ++i)
        threads.emplace_back(unsafe_increment);
    for (auto& t : threads) t.join();
    return unsafe_counter;
}

// ── 版本2: mutex + lock_guard — 正确但慢 ─────────────────────────
int mutex_counter = 0;
std::mutex mtx;

void mutex_increment() {
    for (int i = 0; i < INCREMENTS_PER_THREAD; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        ++mutex_counter;
    }
}

int run_mutex() {
    mutex_counter = 0;
    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; ++i)
        threads.emplace_back(mutex_increment);
    for (auto& t : threads) t.join();
    return mutex_counter;
}

// ── 版本3: atomic — 正确且快 ─────────────────────────────────────
std::atomic<int> atomic_counter{0};

void atomic_increment() {
    for (int i = 0; i < INCREMENTS_PER_THREAD; ++i) {
        atomic_counter.fetch_add(1, std::memory_order_relaxed);
    }
}

int run_atomic() {
    atomic_counter = 0;
    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; ++i)
        threads.emplace_back(atomic_increment);
    for (auto& t : threads) t.join();
    return atomic_counter.load();
}

// ── main ──────────────────────────────────────────────────────────

int main() {
    std::cout << "=== 多线程求和: " << NUM_THREADS << " 线程 × "
              << INCREMENTS_PER_THREAD << " 次 = 期望 " << EXPECTED << "\n\n";

    std::cout << "Hardware concurrency: "
              << std::thread::hardware_concurrency() << "\n\n";

    // 版本1: 无锁（错误演示）
    std::cout << "── 版本1: 无锁（data race） ──\n";
    auto t1 = std::chrono::steady_clock::now();
    int unsafe_result = run_unsafe();
    auto t2 = std::chrono::steady_clock::now();
    std::cout << "  结果: " << unsafe_result
              << "  (缺失 " << (EXPECTED - unsafe_result) << ")\n";
    std::cout << "  耗时: "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
              << " us\n";
    std::cout << "  状态: " << (unsafe_result == EXPECTED ? "正确" : "错误（data race）") << "\n\n";

    // 版本2: mutex
    std::cout << "── 版本2: mutex + lock_guard ──\n";
    t1 = std::chrono::steady_clock::now();
    int mutex_result = run_mutex();
    t2 = std::chrono::steady_clock::now();
    std::cout << "  结果: " << mutex_result
              << "  (" << (mutex_result == EXPECTED ? "" : "不") << "正确)\n";
    std::cout << "  耗时: "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
              << " us\n\n";

    // 版本3: atomic
    std::cout << "── 版本3: atomic + relaxed ──\n";
    t1 = std::chrono::steady_clock::now();
    int atomic_result = run_atomic();
    t2 = std::chrono::steady_clock::now();
    std::cout << "  结果: " << atomic_result
              << "  (" << (atomic_result == EXPECTED ? "" : "不") << "正确)\n";
    std::cout << "  耗时: "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()
              << " us\n";

    return 0;
}
