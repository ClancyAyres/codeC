#include <iostream>
#include <vector>
#include <map>
#include <string>

std::map<std::string, int> wordCount(const std::vector<std::string>& words) {
    std::map<std::string, int> result;
    for (const auto& w : words) {
        if (!w.empty()) {
            ++result[w];
        }
    }
    return result;
}

int main() {
    std::vector<std::string> words = {"apple", "banana", "apple", "cherry", "banana", "apple", ""};
    auto counts = wordCount(words);

    for (const auto& [word, count] : counts) {
        std::cout << word << ": " << count << '\n';
    }

    return 0;
}
