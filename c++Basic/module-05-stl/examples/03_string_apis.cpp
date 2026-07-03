// 模块 5 示例: string 与 string_view
// 相关规范: Core Guidelines SL.str

#include <iostream>
#include <string>
#include <string_view>

void printInfo(std::string_view sv) {
    std::cout << "length: " << sv.length() << ", first char: "
              << (sv.empty() ? '?' : sv[0]) << '\n';
}

std::string upper(std::string_view sv) {
    std::string result;
    result.reserve(sv.size());
    for (char c : sv) {
        result += static_cast<char>(std::toupper(c));
    }
    return result;
}

int main() {
    std::string s = "Hello, C++!";

    std::cout << "substr(0,5): " << s.substr(0, 5) << '\n';
    std::cout << "find('C'): " << s.find('C') << '\n';

    std::string greeting = s + " Welcome.";
    std::cout << greeting << '\n';

    std::string numStr = std::to_string(42);
    std::cout << "to_string(42): " << numStr << '\n';

    int value = std::stoi("42");
    std::cout << "stoi(\"42\"): " << value << '\n';

    printInfo(s);
    printInfo("literal");
    printInfo(std::string_view(s).substr(0, 5));

    std::cout << upper(s) << '\n';

    return 0;
}
