// Exercise 04: Parallel Reduce
// TODO: 实现并行归约（求和/最大值）使用多线程

#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <limits>
#include <numeric>
#include <functional>

// ── 任务: 实现 parallelReduce ─────────────────────────────────────

// TODO: 实现并行归约函数模板
// - 将数据均匀分片给多个线程
// - 每个线程计算局部结果
// - 主线程合并所有局部结果
template<typename T, typename BinaryOp>
T parallelReduce(const std::vector<T>& data,
                 T init,
                 BinaryOp op,
                 int numThreads = 0) {
    // TODO: 如果 numThreads <= 0，使用硬件并发数
    // TODO: 计算每个线程的数据范围 (start, end)
    // TODO: 每个线程计算自己的局部结果
    // TODO: join 所有线程
    // TODO: 合并局部结果
    return init;
}

// TODO: 实现并行求和（基于 parallelReduce）
template<typename T>
T parallelSum(const std::vector<T>& data, int numThreads = 0) {
    // TODO: 调用 parallelReduce，init=T{0}, op=std::plus<T>()
    return T{};
}

// TODO: 实现并行查找最大值（基于 parallelReduce）
template<typename T>
T parallelMax(const std::vector<T>& data, int numThreads = 0) {
    // TODO: 调用 parallelReduce
    // TODO: init=std::numeric_limits<T>::lowest(), op 返回较大值
    return T{};
}

// ── 测试代码 ──────────────────────────────────────────────────────

int main() {
    std::cout << "=== 并行归约 ===\n";
    std::cout << "Hardware concurrency: "
              << std::thread::hardware_concurrency() << "\n\n";

    // 生成测试数据
    std::vector<int> data(1'000'000);
    std::iota(data.begin(), data.end(), 1);  // 1, 2, 3, ..., 1000000

    // 串行计算参考值
    long long expected_sum = static_cast<long long>(data.size())
                           * (data.size() + 1) / 2;  // n*(n+1)/2
    int expected_max = static_cast<int>(data.size());

    std::cout << "数据大小: " << data.size() << '\n';
    std::cout << "期望求和: " << expected_sum << '\n';
    std::cout << "期望最大值: " << expected_max << "\n\n";

    // TODO: 测试并行求和
    // auto sum = parallelSum(data, 4);
    // std::cout << "并行求和(4线程): " << sum
    //           << " (" << (sum == expected_sum ? "PASS" : "FAIL") << ")\n";

    // TODO: 测试并行最大值
    // auto maxVal = parallelMax(data, 4);
    // std::cout << "并行最大值(4线程): " << maxVal
    //           << " (" << (maxVal == expected_max ? "PASS" : "FAIL") << ")\n";

    // TODO: 使用 parallelReduce 实现并行求积（可选）

    std::cout << "\nTODO: 实现 parallelReduce / parallelSum / parallelMax\n";

    return 0;
}
