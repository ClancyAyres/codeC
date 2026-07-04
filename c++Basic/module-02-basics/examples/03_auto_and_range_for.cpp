// 模块 1 示例: auto 与 range-for
// 相关规范: Core Guidelines ES.11; Google Style: Type Deduction

#include <iostream>
#include <string>
#include <vector>
#include <map>

int main() {
    auto i = 42;
    auto d = 3.14;
    auto s = std::string("hello");

    std::vector<std::string> names = {"Alice", "Bob", "Charlie"};

    for (auto name : names) {
        name += "!";
    }

    for (const auto& name : names) {
        std::cout << name << ' ';
    }
    std::cout << '\n';

    for (auto& name : names) {
        name += "!";
    }

    std::map<std::string, int> scores = {{"Alice", 90}, {"Bob", 85}};
    for (const auto& [name, score] : scores) {
        std::cout << name << ": " << score << '\n';
    }

    return 0;
}
