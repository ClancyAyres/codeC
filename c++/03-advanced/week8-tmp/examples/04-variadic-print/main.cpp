// Example 04: Variadic Print — 变参模板 + 折叠表达式
// 演示：递归展开、C++17 折叠表达式、sizeof...、编译时 sum/print

#include <iostream>
#include <string>
#include <sstream>

// ============================================================
// 1. 递归展开（C++11 传统方式）
// ============================================================

void printRecursive() { std::cout << '\n'; }

template<typename T, typename... Args>
void printRecursive(T first, Args... rest) {
    std::cout << first << ' ';
    printRecursive(rest...);
}

// ============================================================
// 2. 折叠表达式 — 打印（C++17 逗号折叠）
// ============================================================

template<typename... Args>
void printFold(Args... args) {
    ((std::cout << args << ' '), ...);
    std::cout << '\n';
}

// 带分隔符的打印
template<typename... Args>
void printJoin(const std::string& sep, Args... args) {
    std::ostringstream oss;
    bool first = true;
    ((oss << (first ? "" : sep) << args, first = false), ...);
    std::cout << oss.str() << '\n';
}

// ============================================================
// 3. 折叠表达式 — 运算
// ============================================================

template<typename... Args>
auto sum(Args... args) { return (args + ...); }

template<typename... Args>
auto product(Args... args) { return (args * ...); }

template<typename... Args>
constexpr bool allTrue(Args... args) { return (true && ... && args); }

template<typename... Args>
constexpr bool anyTrue(Args... args) { return (false || ... || args); }

// ============================================================
// 4. 变参模板 + type_traits — 类型安全打印
// ============================================================

template<typename T>
std::string toPrintable(const T& val) {
    if constexpr (std::is_same_v<T, bool>) {
        return val ? "true" : "false";
    } else if constexpr (std::is_same_v<T, std::string>) {
        return "\"" + val + "\"";
    } else if constexpr (std::is_arithmetic_v<T>) {
        return std::to_string(val);
    } else {
        return "<?>";
    }
}

template<typename... Args>
void typedPrint(Args... args) {
    ((std::cout << toPrintable(args) << ' '), ...);
    std::cout << '\n';
}

// ============================================================
// 5. 使用 sizeof...
// ============================================================

template<typename... Args>
void describe(Args... args) {
    std::cout << sizeof...(Args) << " types, "
              << sizeof...(args) << " arguments\n";
    printFold(args...);
}

int main() {
    std::cout << "=== Recursive Print ===\n";
    printRecursive(1, 2.5, "hello", 'A');

    std::cout << "\n=== Fold Expression Print ===\n";
    printFold(10, 20, 30, "world");

    std::cout << "\n=== Join Print ===\n";
    printJoin(", ", "apple", "banana", "cherry");
    printJoin(" -> ", 1, 2, 3, 4, 5);

    std::cout << "\n=== Fold Arithmetic ===\n";
    std::cout << "sum(1,2,3,4,5)    = " << sum(1, 2, 3, 4, 5) << '\n';
    std::cout << "product(1,2,3,4,5) = " << product(1, 2, 3, 4, 5) << '\n';
    std::cout << "allTrue(true,true,false) = " << allTrue(true, true, false) << '\n';
    std::cout << "anyTrue(false,false,true) = " << anyTrue(false, false, true) << '\n';

    std::cout << "\n=== Typed Print ===\n";
    typedPrint(42, 3.14, std::string("hello"), true);

    std::cout << "\n=== sizeof... ===\n";
    describe(1, "two", 3.0);
    describe(1, 2, 3, 4, 5);

    std::cout << "\n=== Empty pack edge ===\n";
    static_assert(allTrue() == true, "&& over empty pack → true");
    static_assert(anyTrue() == false, "|| over empty pack → false");
    std::cout << "allTrue() = " << allTrue() << " (default: true)\n";
    std::cout << "anyTrue() = " << anyTrue() << " (default: false)\n";

    return 0;
}
