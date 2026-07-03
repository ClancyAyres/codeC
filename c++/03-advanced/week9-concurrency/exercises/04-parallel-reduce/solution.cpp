// Exercise 04: Parallel Reduce — 完整解法

#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <limits>
#include <numeric>
#include <functional>

template<typename T, typename BinaryOp>
T parallelReduce(const std::vector<T>& data,
                 T init,
                 BinaryOp op,
                 int numThreads = 0) {
    if (data.empty())
        return init;

    if (numThreads <= 0)
        numThreads = std::thread::hardware_concurrency();

    size_t n = data.size();
    numThreads = std::min(numThreads, static_cast<int>(n));

    std::vector<T> partialResults(numThreads, init);
    std::vector<std::thread> threads;

    size_t chunk = n / numThreads;
    size_t remainder = n % numThreads;
    size_t start = 0;

    for (int t = 0; t < numThreads; ++t) {
        size_t count = chunk + (t < static_cast<int>(remainder) ? 1 : 0);
        size_t end = start + count;

        threads.emplace_back([&partialResults, &data, t, start, end, init, &op] {
            T local = init;
            for (size_t i = start; i < end; ++i)
                local = op(local, data[i]);
            partialResults[t] = local;
        });

        start = end;
    }

    for (auto& t : threads)
        t.join();

    T result = init;
    for (const auto& partial : partialResults)
        result = op(result, partial);

    return result;
}

template<typename T>
T parallelSum(const std::vector<T>& data, int numThreads = 0) {
    return parallelReduce(data, T{0}, std::plus<T>(), numThreads);
}

template<typename T>
T parallelMax(const std::vector<T>& data, int numThreads = 0) {
    return parallelReduce(
        data,
        std::numeric_limits<T>::lowest(),
        [](T a, T b) { return a > b ? a : b; },
        numThreads
    );
}

int main() {
    std::cout << "=== 并行归约 ===\n";
    std::cout << "Hardware concurrency: "
              << std::thread::hardware_concurrency() << "\n\n";

    std::vector<int> data(1'000'000);
    std::iota(data.begin(), data.end(), 1);

    long long expected_sum = static_cast<long long>(data.size())
                           * (data.size() + 1) / 2;
    int expected_max = static_cast<int>(data.size());

    std::cout << "数据大小: " << data.size() << '\n';
    std::cout << "期望求和: " << expected_sum << '\n';
    std::cout << "期望最大值: " << expected_max << "\n\n";

    auto sum = parallelSum(data, 4);
    std::cout << "并行求和(4线程): " << sum
              << " (" << (sum == expected_sum ? "PASS" : "FAIL") << ")\n";

    auto maxVal = parallelMax(data, 4);
    std::cout << "并行最大值(4线程): " << maxVal
              << " (" << (maxVal == expected_max ? "PASS" : "FAIL") << ")\n";

    return 0;
}
