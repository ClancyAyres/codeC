// Example 04: Type Traits Intro — is_same, enable_if, 类型分发
// 演示：编译时类型判断和基于类型的函数分发

#include <iostream>
#include <type_traits>
#include <string>

// ============================================================
// 1. is_same — 编译时判断类型是否相同
// ============================================================

template<typename T>
void checkType() {
    if constexpr (std::is_same_v<T, int>) {
        std::cout << "T is int\n";
    } else if constexpr (std::is_same_v<T, double>) {
        std::cout << "T is double\n";
    } else if constexpr (std::is_same_v<T, std::string>) {
        std::cout << "T is std::string\n";
    } else {
        std::cout << "T is something else\n";
    }
}

// ============================================================
// 2. enable_if — 条件性启用函数重载
// ============================================================

// 整数版本：仅当 T 是整数类型时启用
template<typename T>
typename std::enable_if<std::is_integral_v<T>, void>::type
process(T value) {
    std::cout << "Integral processing: " << value << " * 2 = " << (value * 2) << '\n';
}

// 浮点版本：仅当 T 是浮点类型时启用
template<typename T>
typename std::enable_if<std::is_floating_point_v<T>, void>::type
process(T value) {
    std::cout << "Floating-point processing: " << value << " * 3.14 = " << (value * 3.14) << '\n';
}

// ============================================================
// 3. if constexpr — C++17 更简洁的类型分发
// ============================================================

template<typename T>
void describe(T value) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "[int]   " << value << " (integral)\n";
    } else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "[float] " << value << " (floating point)\n";
    } else {
        std::cout << "[other] " << value << " (unknown type)\n";
    }
}

// ============================================================
// 4. 自定义 type trait：判断是否有 size() 方法
// ============================================================

template<typename, typename = void>
struct has_size : std::false_type {};

template<typename T>
struct has_size<T, std::void_t<decltype(std::declval<T>().size())>> : std::true_type {};

template<typename T>
inline constexpr bool has_size_v = has_size<T>::value;

template<typename T>
void printSize(const T& container) {
    if constexpr (has_size_v<T>) {
        std::cout << "Container size: " << container.size() << '\n';
    } else {
        std::cout << "(no size() method)\n";
    }
}

int main() {
    std::cout << "=== is_same ===\n";
    checkType<int>();
    checkType<double>();
    checkType<std::string>();
    checkType<char>();

    std::cout << "\n=== enable_if (function dispatch) ===\n";
    process(42);              // 整数版本
    process(3.14);            // 浮点版本
    process(100L);            // long 是整数，整数版本

    std::cout << "\n=== if constexpr ===\n";
    describe(42);
    describe(3.14f);
    describe(std::string("hello"));

    std::cout << "\n=== Type Traits at Compile Time ===\n";
    static_assert(std::is_integral_v<int>);
    static_assert(std::is_floating_point_v<double>);
    static_assert(!std::is_integral_v<double>);
    static_assert(std::is_same_v<int, signed int>);
    std::cout << "All static_asserts passed!\n";

    std::cout << "\n=== Custom Type Trait ===\n";
    std::string s = "hello";
    printSize(s);       // 有 size()
    printSize(42);      // 无 size()

    return 0;
}
