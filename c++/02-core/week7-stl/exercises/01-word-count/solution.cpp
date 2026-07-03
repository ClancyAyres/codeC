#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

std::unordered_map<std::string, int> wordCount(const std::string& text) {
    std::unordered_map<std::string, int> counts;
    std::istringstream stream(text);
    std::string word;

    while (stream >> word) {
        std::string cleaned;
        for (char c : word) {
            if (std::isalpha(static_cast<unsigned char>(c))) {
                cleaned += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            }
        }
        if (!cleaned.empty()) {
            ++counts[cleaned];
        }
    }

    return counts;
}

int main() {
    std::string text = R"(
        C++ is a powerful language.
        Cpp is powerful, and C++ is fast!
        The C++ standard library provides many containers.
        The library is well-designed and the library is efficient.
    )";

    auto counts = wordCount(text);

    std::vector<std::pair<std::string, int>> sorted(
        counts.begin(), counts.end());
    std::sort(sorted.begin(), sorted.end(),
        [](const auto& a, const auto& b) {
            if (a.second != b.second) return a.second > b.second;
            return a.first < b.first;
        });

    std::cout << "Word frequency (descending):\n";
    for (const auto& [word, count] : sorted) {
        std::cout << word << ": " << count << '\n';
    }

    return 0;
}
