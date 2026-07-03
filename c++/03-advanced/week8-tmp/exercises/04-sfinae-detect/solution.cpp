// Exercise 04 Solution: SFINAE Detect — 检测成员函数是否存在

#include <iostream>
#include <type_traits>
#include <string>
#include <vector>
#include <list>

template<typename, typename = void>
struct is_iterable : std::false_type {};

template<typename T>
struct is_iterable<T, std::void_t<
    decltype(std::declval<T&>().begin()),
    decltype(std::declval<T&>().end())
>> : std::true_type {};

template<typename T>
inline constexpr bool is_iterable_v = is_iterable<T>::value;

template<typename, typename = void>
struct has_size : std::false_type {};

template<typename T>
struct has_size<T, std::void_t<decltype(std::declval<T&>().size())>> : std::true_type {};

template<typename T>
inline constexpr bool has_size_v = has_size<T>::value;

template<typename, typename = void>
struct has_reserve : std::false_type {};

template<typename T>
struct has_reserve<T, std::void_t<decltype(std::declval<T&>().reserve(std::declval<size_t>()))>> : std::true_type {};

template<typename T>
inline constexpr bool has_reserve_v = has_reserve<T>::value;

template<typename T>
void inspect(const T& t) {
    std::cout << "[?] ";
    if constexpr (is_iterable_v<T>) {
        std::cout << "iterable: [";
        bool first = true;
        for (const auto& e : t) {
            if (!first) std::cout << ", ";
            std::cout << e;
            first = false;
        }
        std::cout << "] ";
    } else {
        std::cout << "not iterable ";
    }

    if constexpr (has_size_v<T>)
        std::cout << " size: " << t.size();
    else
        std::cout << " size: no";

    if constexpr (has_reserve_v<T>)
        std::cout << " reserve: yes";
    else
        std::cout << " reserve: no";

    std::cout << '\n';
}

int main() {
    std::cout << "=== SFINAE Detect ===\n";

    std::vector<int> v{1, 2, 3};
    std::cout << "\nvector<int>::\n";
    std::cout << "  is_iterable: " << is_iterable_v<std::vector<int>> << '\n';
    inspect(v);

    std::string s = "hi";
    std::cout << "\nstd::string:\n";
    std::cout << "  is_iterable: " << is_iterable_v<std::string> << '\n';
    inspect(s);

    int x = 42;
    std::cout << "\nint:\n";
    std::cout << "  is_iterable: " << is_iterable_v<int> << '\n';
    inspect(x);

    std::list<int> l{10, 20};
    std::cout << "\nstd::list<int>:\n";
    inspect(l);

    return 0;
}
