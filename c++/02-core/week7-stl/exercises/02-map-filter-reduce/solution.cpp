#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>

template<typename Container, typename Predicate>
Container filter(const Container& c, Predicate pred) {
    Container result;
    std::copy_if(c.begin(), c.end(), std::back_inserter(result), pred);
    return result;
}

template<typename Container, typename UnaryOp>
auto map(const Container& c, UnaryOp op) -> std::vector<decltype(op(*c.begin()))> {
    std::vector<decltype(op(*c.begin()))> result;
    result.reserve(c.size());
    std::transform(c.begin(), c.end(), std::back_inserter(result), op);
    return result;
}

template<typename Container, typename T, typename BinaryOp>
T reduce(const Container& c, T init, BinaryOp op) {
    return std::accumulate(c.begin(), c.end(), init, op);
}

template<typename T>
void print(const std::string& label, const std::vector<T>& v) {
    std::cout << label << ": ";
    for (const auto& x : v) std::cout << x << ' ';
    std::cout << '\n';
}

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<std::string> words = {"hello", "a", "world", "ab", "algorithm"};

    std::cout << "=== Filter ===\n";
    auto evens = filter(numbers, [](int n) { return n % 2 == 0; });
    print("Evens", evens);

    auto longWords = filter(words, [](const std::string& s) { return s.length() > 3; });
    print("Words with len > 3", longWords);

    std::cout << "\n=== Map ===\n";
    auto squares = map(evens, [](int n) { return n * n; });
    print("Squares", squares);

    auto upper = map(longWords, [](std::string s) {
        for (auto& c : s) c = static_cast<char>(std::toupper(c));
        return s;
    });
    print("Uppercase", upper);

    std::cout << "\n=== Reduce ===\n";
    auto sum = reduce(squares, 0, std::plus<int>());
    std::cout << "Sum of squares: " << sum << '\n';

    auto product = reduce(std::vector<int>{1, 2, 3, 4, 5}, 1, std::multiplies<int>());
    std::cout << "Product: " << product << '\n';

    std::cout << "\n=== Chained ===\n";
    auto result = reduce(
        map(
            filter(numbers, [](int n) { return n > 5; }),
            [](int n) { return n * 10; }
        ),
        0,
        std::plus<int>()
    );
    std::cout << "Filter(>5) → Map(*10) → Sum = " << result << '\n';

    return 0;
}
