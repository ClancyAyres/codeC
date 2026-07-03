#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> topK(const std::vector<int>& data, size_t k) {
    if (data.empty()) return {};
    if (k > data.size()) k = data.size();

    std::vector<int> result = data;
    std::partial_sort(result.begin(), result.begin() + k, result.end(),
                      std::greater<int>{});
    result.resize(k);
    return result;
}

int main() {
    std::vector<int> data = {5, 1, 9, 3, 7, 2, 8, 6, 4};

    auto top = topK(data, 3);
    std::cout << "Top 3: ";
    for (int x : top) std::cout << x << ' ';
    std::cout << '\n';

    auto all = topK(data, 100);
    std::cout << "All (k > size): ";
    for (int x : all) std::cout << x << ' ';
    std::cout << '\n';

    return 0;
}
