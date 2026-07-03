// Example 03: enable_if Overload — SFINAE 函数重载 & if constexpr
// 演示：enable_if 返回类型、enable_if 默认模板参数、if constexpr 对比

#include <iostream>
#include <type_traits>
#include <string>
#include <vector>

// ============================================================
// 1. enable_if as return type (traditional SFINAE)
// ============================================================

template<typename T>
typename std::enable_if<std::is_integral_v<T>, std::string>::type
classify(T) { return "integral"; }

template<typename T>
typename std::enable_if<std::is_floating_point_v<T>, std::string>::type
classify(T) { return "floating-point"; }

template<typename T>
typename std::enable_if<std::is_same_v<T, std::string>, std::string>::type
classify(const T&) { return "std::string"; }

// Fallback for everything else
template<typename T>
typename std::enable_if<
    !std::is_integral_v<T> &&
    !std::is_floating_point_v<T> &&
    !std::is_same_v<T, std::string>, std::string>::type
classify(const T&) { return "other"; }

// ============================================================
// 2. enable_if as extra template parameter alternative
// ============================================================

template<typename T, std::enable_if_t<std::is_pointer_v<T>, int> = 0>
void derefPrint(T ptr) {
    std::cout << "pointer → " << *ptr << '\n';
}

template<typename T, std::enable_if_t<!std::is_pointer_v<T>, int> = 0>
void derefPrint(const T& val) {
    std::cout << "value → " << val << '\n';
}

// ============================================================
// 3. if constexpr approach (C++17) — same logic, cleaner syntax
// ============================================================

template<typename T>
void show(const T& val) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "int: " << val << " (×2 = " << (val * 2) << ")\n";
    } else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "float: " << val << " (×3.14 = " << (val * 3.14) << ")\n";
    } else if constexpr (std::is_same_v<T, const char*> || std::is_same_v<T, std::string>) {
        std::cout << "string: \"" << val << "\"\n";
    } else {
        std::cout << "other type\n";
    }
}

// ============================================================
// 4. Custom trait + SFINAE overload
// ============================================================

template<typename, typename = void>
struct is_iterable : std::false_type {};

template<typename T>
struct is_iterable<T, std::void_t<
    decltype(std::begin(std::declval<T&>())),
    decltype(std::end(std::declval<T&>()))
>> : std::true_type {};

template<typename T>
inline constexpr bool is_iterable_v = is_iterable<T>::value;

template<typename T>
std::enable_if_t<is_iterable_v<T>, void>
print(const T& container) {
    std::cout << "[";
    bool first = true;
    for (const auto& e : container) {
        if (!first) std::cout << ", ";
        std::cout << e;
        first = false;
    }
    std::cout << "]\n";
}

template<typename T>
std::enable_if_t<!is_iterable_v<T>, void>
print(const T& val) {
    std::cout << val << '\n';
}

int main() {
    std::cout << "=== enable_if classify ===\n";
    std::cout << "int:    " << classify(42) << '\n';
    std::cout << "double: " << classify(3.14) << '\n';
    std::cout << "string: " << classify(std::string("hello")) << '\n';
    std::cout << "char:   " << classify('x') << '\n';

    std::cout << "\n=== derefPrint (pointer vs value) ===\n";
    int x = 100;
    derefPrint(x);       // value
    derefPrint(&x);      // pointer

    std::cout << "\n=== if constexpr show ===\n";
    show(42);
    show(3.14);
    show(std::string("world"));

    std::cout << "\n=== Custom trait: is_iterable ===\n";
    std::cout << "std::vector<int>: " << is_iterable_v<std::vector<int>> << '\n';
    std::cout << "int:              " << is_iterable_v<int> << '\n';

    std::cout << "\n=== Overloaded print ===\n";
    print(std::vector{1, 2, 3, 4});
    print(42);
    print(std::string("hello"));

    return 0;
}
