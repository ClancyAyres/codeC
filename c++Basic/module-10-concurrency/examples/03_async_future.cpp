// 模块 9 示例: async + future
// 类似 Java CompletableFuture

#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <vector>
#include <numeric>
#include <algorithm>

// ✅ 异步任务: 类似 CompletableFuture.supplyAsync
int heavyComputation(int input) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return input * input;
}

int main() {
    // === std::async: 启动异步任务 ===
    std::future<int> result1 = std::async(std::launch::async, heavyComputation, 42);

    std::cout << "Doing other work while computation runs...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // 获取结果: 阻塞直到计算完成
    std::cout << "Result: " << result1.get() << '\n';

    // === 并行求和 ===
    auto parallelSum = [](const std::vector<int>& data) -> long long {
        size_t mid = data.size() / 2;

        // 两个异步任务分别计算前后半段
        auto future1 = std::async(std::launch::async, [&data, mid]() {
            return std::accumulate(data.begin(), data.begin() + mid, 0LL);
        });
        auto future2 = std::async(std::launch::async, [&data, mid]() {
            return std::accumulate(data.begin() + mid, data.end(), 0LL);
        });

        return future1.get() + future2.get();
    };

    std::vector<int> numbers(1000000);
    std::iota(numbers.begin(), numbers.end(), 1);

    auto sum = parallelSum(numbers);
    std::cout << "Sum of 1..1,000,000 = " << sum << '\n';

    return 0;
}
