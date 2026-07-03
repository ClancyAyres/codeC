// 模块 5 示例: STL 算法 — 替代 Java Stream
// 相关规范: Core Guidelines SL.alg

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include <iterator>

int main() {
    std::vector<int> data = {5, 2, 8, 1, 9, 3, 7, 4, 6};

    // 排序
    std::vector<int> sorted = data;
    std::sort(sorted.begin(), sorted.end());

    // count_if: 类似 filter + count
    auto count = std::count_if(data.begin(), data.end(),
                               [](int x) { return x > 5; });
    std::cout << "> 5: " << count << '\n';

    // accumulate: 类似 sum
    auto sum = std::accumulate(data.begin(), data.end(), 0);
    std::cout << "sum: " << sum << '\n';

    // max_element: 类似 max
    auto maxIt = std::max_element(data.begin(), data.end());
    if (maxIt != data.end()) {
        std::cout << "max: " << *maxIt << '\n';
    }

    // transform: 类似 map
    std::vector<int> doubled;
    std::transform(data.begin(), data.end(), std::back_inserter(doubled),
                   [](int x) { return x * 2; });

    // any_of: 类似 anyMatch
    bool hasEven = std::any_of(data.begin(), data.end(),
                               [](int x) { return x % 2 == 0; });
    std::cout << "has even: " << (hasEven ? "yes" : "no") << '\n';

    // partial_sort: Top K
    std::vector<int> top3 = data;
    std::partial_sort(top3.begin(), top3.begin() + 3, top3.end(),
                      std::greater<int>{});
    std::cout << "top 3: ";
    for (int i = 0; i < 3; ++i) std::cout << top3[i] << ' ';
    std::cout << '\n';

    // std::remove_if + erase (Erase-Remove 惯用法)
    std::vector<int> filtered = data;
    filtered.erase(
        std::remove_if(filtered.begin(), filtered.end(),
                       [](int x) { return x > 5; }),
        filtered.end());
    std::cout << "after remove_if(>5): ";
    for (int x : filtered) std::cout << x << ' ';
    std::cout << '\n';

    return 0;
}
