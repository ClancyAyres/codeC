// Exercise 03 Solution: makeVector — 变参模板创建 vector<string>

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <type_traits>

template<typename T>
std::string toString(const T& val) {
    if constexpr (std::is_same_v<T, std::string>) {
        return val;
    } else if constexpr (std::is_same_v<T, const char*> || std::is_same_v<T, char*>) {
        return std::string(val);
    } else if constexpr (std::is_arithmetic_v<T>) {
        return std::to_string(val);
    } else {
        return "<?>";
    }
}

template<typename... Args>
std::vector<std::string> makeVector(Args... args) {
    std::vector<std::string> result;
    (result.push_back(toString(args)), ...);
    return result;
}

template<typename... Args>
std::set<std::string> makeSet(Args... args) {
    std::set<std::string> result;
    (result.insert(toString(args)), ...);
    return result;
}

void print(const std::vector<std::string>& v) {
    std::cout << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << v[i];
    }
    std::cout << "]\n";
}

void print(const std::set<std::string>& s) {
    std::cout << "{";
    bool first = true;
    for (const auto& e : s) {
        if (!first) std::cout << ", ";
        std::cout << e;
        first = false;
    }
    std::cout << "}\n";
}

int main() {
    std::cout << "=== makeVector ===\n";
    auto v = makeVector(1, 2.5, "hello", std::string("world"), true);
    print(v);

    std::cout << "\n=== makeSet ===\n";
    auto s = makeSet(3, 1, 2, 3, 1);
    print(s);

    return 0;
}
