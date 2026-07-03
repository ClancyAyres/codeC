// Exercise 03: makeVector — 变参模板创建 vector<string>
// TODO: 实现 makeVector 和 makeSet

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <type_traits>

// TODO: 实现 toString 辅助函数
template<typename T>
std::string toString(const T& val) {
    // TODO: if constexpr (std::is_same_v<T, std::string>) return val;
    // TODO: else if constexpr (std::is_same_v<T, const char*> || std::is_same_v<T, char*>) return std::string(val);
    // TODO: else if constexpr (std::is_arithmetic_v<T>) return std::to_string(val);
    // TODO: else return "<?>";
}

// TODO: 实现 makeVector
// template<typename... Args>
// std::vector<std::string> makeVector(Args... args) {
//     std::vector<std::string> result;
//     (result.push_back(toString(args)), ...);
//     return result;
// }

// TODO: 实现 makeSet
// template<typename... Args>
// std::set<std::string> makeSet(Args... args) {
//     std::set<std::string> result;
//     (result.insert(toString(args)), ...);
//     return result;
// }

// TODO: 实现 print 辅助函数
// void print(const std::vector<std::string>& v) { ... }
// void print(const std::set<std::string>& s) { ... }

int main() {
    std::cout << "=== makeVector ===\n";
    // TODO: uncomment
    // auto v = makeVector(1, 2.5, "hello", std::string("world"), true);
    // print(v);

    std::cout << "\n=== makeSet ===\n";
    // TODO: uncomment
    // auto s = makeSet(3, 1, 2, 3, 1);
    // print(s);

    return 0;
}
