// Example 01: constexpr Factorial — 编译时 vs 运行时 + if constexpr
// 演示：constexpr 函数、constexpr 变量、if constexpr、static_assert

#include <iostream>
#include <array>

constexpr int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i) result *= i;
    return result;
}

template<int N>
struct Fib {
    static constexpr int value = Fib<N - 1>::value + Fib<N - 2>::value;
};
template<> struct Fib<0> { static constexpr int value = 0; };
template<> struct Fib<1> { static constexpr int value = 1; };

template<typename T>
void describe(T val) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "integral: " << val << '\n';
    } else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "floating: " << val << '\n';
    } else {
        std::cout << "other\n";
    }
}

constexpr bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; ++i)
        if (n % i == 0) return false;
    return true;
}

int main() {
    std::cout << "=== constexpr Factorial ===\n";
    constexpr int f0 = factorial(0);
    constexpr int f5 = factorial(5);
    constexpr int f10 = factorial(10);
    std::cout << "factorial(0)  = " << f0 << '\n';
    std::cout << "factorial(5)  = " << f5 << '\n';
    std::cout << "factorial(10) = " << f10 << '\n';

    static_assert(factorial(5) == 120, "factorial(5) should be 120");
    static_assert(factorial(0) == 1, "factorial(0) should be 1");

    std::cout << "\n=== Compile-Time Array Size ===\n";
    constexpr int size = factorial(4); // 24
    int arr[size];
    std::cout << "int arr[factorial(4)] → sizeof = " << sizeof(arr) << " bytes\n";

    std::cout << "\n=== Template vs constexpr Fibonacci ===\n";
    std::cout << "Fib<10>::value     = " << Fib<10>::value << " (template metaprogramming)\n";
    constexpr int cf10 = Fib<10>::value;
    std::cout << "runtimeFib(10)    = " << cf10 << " (constexpr via template)\n";

    std::cout << "\n=== if constexpr ===\n";
    describe(42);
    describe(3.14);
    describe("hello");

    std::cout << "\n=== constexpr Prime Check ===\n";
    static_assert(isPrime(2));
    static_assert(isPrime(17));
    static_assert(!isPrime(1));
    static_assert(!isPrime(9));
    constexpr int primes[] = {2, 3, 5, 7, 11, 13, 17, 19};
    std::cout << "Primes: ";
    for (auto p : primes) std::cout << p << ' ';
    std::cout << "\nall verified at compile time\n";

    return 0;
}
