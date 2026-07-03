#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <algorithm>
#include <functional>

long long parallelSum(const std::vector<int>& data, size_t numThreads) {
    if (numThreads == 0) numThreads = 1;
    if (numThreads > data.size()) numThreads = data.size();

    size_t chunkSize = data.size() / numThreads;
    std::vector<std::thread> threads;
    std::vector<long long> partialSums(numThreads, 0);

    for (size_t i = 0; i < numThreads; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == numThreads - 1) ? data.size() : start + chunkSize;

        threads.emplace_back([start, end, &data, &partialSums, i]() {
            long long sum = 0;
            for (size_t j = start; j < end; ++j) {
                sum += data[j];
            }
            partialSums[i] = sum;
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    return std::accumulate(partialSums.begin(), partialSums.end(), 0LL);
}

int main() {
    std::vector<int> data(1000000);
    std::iota(data.begin(), data.end(), 1);

    long long singleSum = std::accumulate(data.begin(), data.end(), 0LL);
    long long multiSum = parallelSum(data, 4);

    std::cout << "Single-thread sum: " << singleSum << '\n';
    std::cout << "Multi-thread sum:  " << multiSum << '\n';
    std::cout << "Match: " << (singleSum == multiSum ? "YES" : "NO") << '\n';

    return 0;
}
