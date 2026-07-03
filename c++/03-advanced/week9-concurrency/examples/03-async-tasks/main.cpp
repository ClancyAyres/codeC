// Example 03: Async Tasks — future / promise / async / packaged_task
// 演示四种异步任务模式

#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <vector>
#include <numeric>

// ── 1. std::async — 最简单的异步 ──────────────────────────────────

int slow_square(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    return x * x;
}

void demo_async() {
    std::cout << "── 1. std::async ──\n";
    std::cout << "  main thread: launching async task...\n";

    std::future<int> fut = std::async(std::launch::async, slow_square, 7);
    // 注意：std::launch::async 强制在新线程执行

    std::cout << "  main thread: doing other work while async runs...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // wait_for 检查状态
    auto status = fut.wait_for(std::chrono::milliseconds(100));
    if (status == std::future_status::ready)
        std::cout << "  结果已就绪\n";
    else
        std::cout << "  仍在计算中...\n";

    // get() 阻塞等待最终结果（只能调用一次）
    int result = fut.get();
    std::cout << "  7^2 = " << result << "\n\n";
}

// ── 2. std::promise — 手动设置 future ─────────────────────────────

void compute_with_promise(std::promise<int> prom, int a, int b) {
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    prom.set_value(a + b);  // 在任意线程中设置结果
}

void demo_promise() {
    std::cout << "── 2. std::promise ──\n";

    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread t(compute_with_promise, std::move(prom), 10, 20);

    std::cout << "  等待 promise 结果...\n";
    int result = fut.get();
    std::cout << "  10 + 20 = " << result << '\n';

    t.join();
    std::cout << '\n';
}

// ── 3. std::packaged_task — 包装可调用对象 ────────────────────────

void demo_packaged_task() {
    std::cout << "── 3. std::packaged_task ──\n";

    std::packaged_task<int(int)> task([](int n) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        int sum = 0;
        for (int i = 1; i <= n; ++i) sum += i;
        return sum;
    });

    std::future<int> fut = task.get_future();

    // packaged_task 可移动，不可拷贝
    std::thread t(std::move(task), 100);

    std::cout << "  等待 packaged_task 结果...\n";
    int result = fut.get();
    std::cout << "  1+2+...+100 = " << result << '\n';

    t.join();
    std::cout << '\n';
}

// ── 4. 多个 async 并发执行 ─────────────────────────────────────

void demo_parallel_async() {
    std::cout << "── 4. 并发 async 任务 ──\n";

    auto start = std::chrono::steady_clock::now();

    // 同时启动多个异步任务
    std::vector<std::future<int>> futures;
    for (int i = 1; i <= 5; ++i) {
        futures.push_back(
            std::async(std::launch::async, slow_square, i)
        );
    }

    std::cout << "  5 个任务已在后台运行...\n";

    // 收集结果
    for (int i = 0; i < 5; ++i) {
        int result = futures[i].get();
        std::cout << "  " << (i + 1) << "^2 = " << result << '\n';
    }

    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "  总耗时: " << elapsed << " ms (若串行需 ~1000 ms)\n";

    if (!futures[0].valid())
        std::cout << "  future 已被 get() 消耗\n\n";
}

// ── main ──────────────────────────────────────────────────────────

int main() {
    std::cout << "=== 异步任务模式 ===\n";
    std::cout << "Hardware concurrency: "
              << std::thread::hardware_concurrency() << "\n\n";

    demo_async();
    demo_promise();
    demo_packaged_task();
    demo_parallel_async();

    return 0;
}
