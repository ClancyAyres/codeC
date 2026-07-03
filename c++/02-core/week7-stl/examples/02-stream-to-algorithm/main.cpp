// Example 02: Stream → Algorithm — Java Stream API vs C++ <algorithm> + lambda
// 演示：常见 Stream 操作在 C++ 中的等价实现

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <iterator>

void print(const std::string& label, const std::vector<int>& v) {
    std::cout << label << ": ";
    for (auto x : v) std::cout << x << ' ';
    std::cout << '\n';
}

void print(const std::string& label, const std::vector<std::string>& v) {
    std::cout << label << ": ";
    for (const auto& s : v) std::cout << s << ' ';
    std::cout << '\n';
}

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    print("Original", numbers);

    // ── filter ────────────────────────────────────────────
    // Java:  numbers.stream().filter(n -> n % 2 == 0).collect(toList())
    {
        std::vector<int> evens;
        std::copy_if(numbers.begin(), numbers.end(), std::back_inserter(evens),
            [](int n) { return n % 2 == 0; });
        print("\nfilter(even)", evens);
    }

    // ── map ───────────────────────────────────────────────
    // Java:  numbers.stream().map(n -> n * n).collect(toList())
    {
        std::vector<int> squares;
        std::transform(numbers.begin(), numbers.end(), std::back_inserter(squares),
            [](int n) { return n * n; });
        print("\nmap(square)", squares);
    }

    // ── filter + map ──────────────────────────────────────
    // Java:  numbers.stream().filter(n->n>5).map(n->n*10).collect(toList())
    {
        std::vector<int> result;
        std::copy_if(numbers.begin(), numbers.end(), std::back_inserter(result),
            [](int n) { return n > 5; });
        std::transform(result.begin(), result.end(), result.begin(),
            [](int n) { return n * 10; });
        print("\nfilter(>5)+map(*10)", result);
    }

    // ── reduce ────────────────────────────────────────────
    // Java:  numbers.stream().reduce(0, Integer::sum)
    {
        int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
        std::cout << "\nreduce(sum): " << sum << '\n';

        int product = std::accumulate(numbers.begin(), numbers.end(), 1,
            std::multiplies<int>());
        std::cout << "reduce(product): " << product << '\n';
    }

    // ── sorted ────────────────────────────────────────────
    // Java:  numbers.stream().sorted(Comparator.reverseOrder()).collect(toList())
    {
        std::vector<int> sorted = {3, 1, 4, 1, 5, 9, 2, 6};
        print("\nBefore sort", sorted);
        std::sort(sorted.begin(), sorted.end(), std::greater<int>());
        print("sort(descending)", sorted);
    }

    // ── distinct ──────────────────────────────────────────
    // Java:  list.stream().distinct().collect(toList())
    {
        std::vector<int> withDupes = {1, 2, 2, 3, 3, 3, 4};
        print("\nBefore distinct", withDupes);
        std::sort(withDupes.begin(), withDupes.end());
        auto last = std::unique(withDupes.begin(), withDupes.end());
        withDupes.erase(last, withDupes.end());
        print("distinct", withDupes);
    }

    // ── limit ─────────────────────────────────────────────
    // Java:  numbers.stream().limit(3).collect(toList())
    {
        std::vector<int> limited(numbers.begin(), numbers.begin() + 3);
        print("\nlimit(3)", limited);
    }

    // ── skip ──────────────────────────────────────────────
    // Java:  numbers.stream().skip(7).collect(toList())
    {
        std::vector<int> skipped(numbers.begin() + 7, numbers.end());
        print("skip(7)", skipped);
    }

    // ── anyMatch / allMatch / noneMatch ───────────────────
    // Java:  numbers.stream().anyMatch(n -> n > 5)
    {
        bool anyGt5 = std::any_of(numbers.begin(), numbers.end(),
            [](int n) { return n > 5; });
        bool allGt0 = std::all_of(numbers.begin(), numbers.end(),
            [](int n) { return n > 0; });
        bool noneNeg = std::none_of(numbers.begin(), numbers.end(),
            [](int n) { return n < 0; });

        std::cout << "\nanyMatch(n>5): " << (anyGt5 ? "true" : "false") << '\n';
        std::cout << "allMatch(n>0): " << (allGt0 ? "true" : "false") << '\n';
        std::cout << "noneMatch(n<0): " << (noneNeg ? "true" : "false") << '\n';
    }

    // ── count ─────────────────────────────────────────────
    // Java:  numbers.stream().filter(n -> n % 2 == 0).count()
    {
        auto evenCount = std::count_if(numbers.begin(), numbers.end(),
            [](int n) { return n % 2 == 0; });
        std::cout << "\ncount(even): " << evenCount << '\n';
    }

    // ── findFirst ─────────────────────────────────────────
    // Java:  numbers.stream().filter(n -> n > 5).findFirst()
    {
        auto it = std::find_if(numbers.begin(), numbers.end(),
            [](int n) { return n > 5; });
        std::cout << "\nfindFirst(n>5): "
                  << (it != numbers.end() ? std::to_string(*it) : "not found") << '\n';
    }

    // ── flatMap (simplified) ──────────────────────────────
    // Java:  lists.stream().flatMap(List::stream).collect(toList())
    {
        std::vector<std::vector<int>> nested = {{1, 2}, {3, 4, 5}, {6}};
        std::vector<int> flat;
        for (const auto& inner : nested) {
            flat.insert(flat.end(), inner.begin(), inner.end());
        }
        print("\nflatMap", flat);
    }

    // ── String operations ─────────────────────────────────
    {
        std::vector<std::string> words = {"hello", "world", "c++", "stl", "algorithm"};

        // Java: words.stream().map(String::toUpperCase).collect(toList())
        std::vector<std::string> upper;
        std::transform(words.begin(), words.end(), std::back_inserter(upper),
            [](std::string s) {
                for (auto& c : s) c = static_cast<char>(std::toupper(c));
                return s;
            });
        print("\nStrings map(toUpper)", upper);

        // Java: words.stream().filter(s -> s.length() > 3).collect(toList())
        std::vector<std::string> longWords;
        std::copy_if(words.begin(), words.end(), std::back_inserter(longWords),
            [](const std::string& s) { return s.length() > 3; });
        print("Strings filter(len>3)", longWords);
    }

    // ── Performance note ──────────────────────────────────
    std::cout << "\n=== Performance Note ===\n";
    std::cout << "C++ <algorithm> + lambda advantages:\n";
    std::cout << "  - Zero-cost abstraction: lambdas are inlined\n";
    std::cout << "  - No virtual dispatch overhead (unlike Java Stream)\n";
    std::cout << "  - No boxing of primitives\n";
    std::cout << "  - Iterator-based: works on arrays, raw pointers, any range\n";
    std::cout << "C++20 <ranges> adds pipe syntax: v | filter(...) | transform(...)\n";

    return 0;
}
