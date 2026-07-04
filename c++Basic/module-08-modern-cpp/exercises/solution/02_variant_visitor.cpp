#include <iostream>
#include <variant>
#include <string>
#include <sstream>

using ConfigValue = std::variant<int, double, std::string, bool>;

ConfigValue parse(const std::string& input) {
    if (input == "true") return true;
    if (input == "false") return false;

    try {
        size_t pos = 0;
        int i = std::stoi(input, &pos);
        if (pos == input.size()) return i;
    } catch (...) {}

    try {
        size_t pos = 0;
        double d = std::stod(input, &pos);
        if (pos == input.size()) return d;
    } catch (...) {}

    return input;
}

void print(const ConfigValue& value) {
    std::visit([](const auto& v) {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, std::string>) {
            std::cout << "string: \"" << v << "\"\n";
        } else if constexpr (std::is_same_v<T, int>) {
            std::cout << "int: " << v << '\n';
        } else if constexpr (std::is_same_v<T, double>) {
            std::cout << "double: " << v << '\n';
        } else if constexpr (std::is_same_v<T, bool>) {
            std::cout << "bool: " << (v ? "true" : "false") << '\n';
        }
    }, value);
}

int main() {
    print(parse("42"));        // int
    print(parse("3.14"));      // double
    print(parse("true"));      // bool
    print(parse("false"));     // bool
    print(parse("hello"));     // string

    return 0;
}
