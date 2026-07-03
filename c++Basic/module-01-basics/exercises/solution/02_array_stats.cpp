// 模块 1 习题答案: 数组统计
// 规范: Core Guidelines F.15, F.22, I.13; Google Style: Output Parameters

#include <iostream>
#include <cstddef>

void computeStats(const int* data, size_t size,
                  int* outMin, int* outMax, double* outAvg) {
    if (data == nullptr || size == 0) {
        return;
    }

    int min = data[0];
    int max = data[0];
    int sum = data[0];

    for (size_t i = 1; i < size; ++i) {
        if (data[i] < min) min = data[i];
        if (data[i] > max) max = data[i];
        sum += data[i];
    }

    if (outMin) *outMin = min;
    if (outMax) *outMax = max;
    if (outAvg) *outAvg = static_cast<double>(sum) / size;
}

int main() {
    int temperatures[] = {23, 19, 31, 25, 28, 22, 30};
    size_t n = sizeof(temperatures) / sizeof(temperatures[0]);

    int min, max;
    double avg;
    computeStats(temperatures, n, &min, &max, &avg);
    std::cout << "Min: " << min << ", Max: " << max << ", Avg: " << avg << '\n';

    int onlyMin;
    computeStats(temperatures, n, &onlyMin, nullptr, nullptr);
    std::cout << "Only Min: " << onlyMin << '\n';

    computeStats(nullptr, 0, &min, &max, &avg);
    std::cout << "Empty array handled safely.\n";

    return 0;
}
