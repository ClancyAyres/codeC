// Example 01: Generic Max — 函数模板与类型推导
// 演示：函数模板定义、类型推导、显式实例化

#include <iostream>
#include <string>

template<typename T>
T max(T a, T b) {
    return a > b ? a : b;
}

template<typename T>
T min(T a, T b) {
    return a < b ? a : b;
}

int main() {
    std::cout << "=== Type Deduction ===\n";
    std::cout << "max(3, 5) = " << max(3, 5) << "       (T = int)\n";
    std::cout << "max(3.14, 2.72) = " << max(3.14, 2.72) << "   (T = double)\n";
    std::cout << "min(10, 20) = " << min(10, 20) << "       (T = int)\n";
    std::cout << "min(5.5, 2.2) = " << min(5.5, 2.2) << "     (T = double)\n";

    std::cout << "\n=== Explicit Instantiation ===\n";
    std::cout << "max<int>(3, 5) = " << max<int>(3, 5) << '\n';
    std::cout << "max<double>(3, 3.14) = " << max<double>(3, 3.14) << "  (int 3 -> double)\n";
    std::cout << "max<long>(10L, 20L) = " << max<long>(10L, 20L) << '\n';

    std::cout << "\n=== String Comparison ===\n";
    std::string s1 = "apple";
    std::string s2 = "banana";
    std::cout << "max(\"" << s1 << "\", \"" << s2 << "\") = \"" << ::max(s1, s2) << "\" (lexicographical)\n";

    std::cout << "\n=== Mixed Types (explicit) ===\n";
    std::cout << "max<float>(3, 4.5f) = " << max<float>(3, 4.5f) << '\n';

    return 0;
}
