// 模块 6 示例: 变参模板
// C++ 支持变参模板，Java 泛型不支持

#include <iostream>
#include <string>

// ✅ 递归终止: 无参数版本
template<typename T>
T sum(T v) {
    return v;
}

// ✅ 变参模板: 递归展开
template<typename T, typename... Args>
T sum(T first, Args... rest) {
    return first + sum(rest...);
}

// ✅ 变参 + 完美转发: 类似 Java 的 Builder 模式
template<typename... Args>
void log(const std::string& format, Args... args) {
    std::cout << "[LOG] ";
    // C++17 fold expression — Java 完全没有的语法
    ((std::cout << args << ' '), ...);
    std::cout << '\n';
}

// ✅ C++17 fold expression (折叠表达式)
template<typename... Args>
auto sumFold(Args... args) {
    return (... + args);  // 一元右折叠
}

int main() {
    std::cout << sum(1, 2, 3, 4, 5) << '\n';
    std::cout << sumFold(1, 2, 3, 4, 5) << '\n';
    // 等效于: 1 + (2 + (3 + (4 + 5)))

    log("test", 42, 3.14, std::string("hello"));

    return 0;
}
