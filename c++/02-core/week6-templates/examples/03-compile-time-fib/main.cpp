// Example 03: Compile-Time Fibonacci — 非类型模板参数 + 特化
// 演示：template<int N>、全特化、编译时计算

#include <iostream>

// 通用递归模板：编译时计算斐波那契数
template<int N>
struct Fibonacci {
    static constexpr int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};

// 全特化：终止条件 N=0
template<>
struct Fibonacci<0> {
    static constexpr int value = 0;
};

// 全特化：终止条件 N=1
template<>
struct Fibonacci<1> {
    static constexpr int value = 1;
};

// 编译时阶乘（另一个非类型参数示例）
template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

int main() {
    std::cout << "=== Compile-Time Fibonacci ===\n";
    std::cout << "Fibonacci<0>  = " << Fibonacci<0>::value << '\n';
    std::cout << "Fibonacci<1>  = " << Fibonacci<1>::value << '\n';
    std::cout << "Fibonacci<2>  = " << Fibonacci<2>::value << '\n';
    std::cout << "Fibonacci<3>  = " << Fibonacci<3>::value << '\n';
    std::cout << "Fibonacci<4>  = " << Fibonacci<4>::value << '\n';
    std::cout << "Fibonacci<5>  = " << Fibonacci<5>::value << '\n';
    std::cout << "Fibonacci<10> = " << Fibonacci<10>::value << '\n';
    std::cout << "Fibonacci<15> = " << Fibonacci<15>::value << '\n';
    std::cout << "Fibonacci<20> = " << Fibonacci<20>::value << '\n';

    std::cout << "\n=== Compile-Time Factorial ===\n";
    std::cout << "Factorial<0> = " << Factorial<0>::value << '\n';
    std::cout << "Factorial<1> = " << Factorial<1>::value << '\n';
    std::cout << "Factorial<2> = " << Factorial<2>::value << '\n';
    std::cout << "Factorial<3> = " << Factorial<3>::value << '\n';
    std::cout << "Factorial<4> = " << Factorial<4>::value << '\n';
    std::cout << "Factorial<5> = " << Factorial<5>::value << '\n';
    std::cout << "Factorial<10> = " << Factorial<10>::value << '\n';

    std::cout << "\n=== Non-Type Array Size ===\n";
    // 使用非类型参数创建不同大小的数组
    std::cout << "sizeof(int[" << Fibonacci<5>::value << "]) = "
              << sizeof(int[Fibonacci<5>::value]) << " bytes\n";
    std::cout << "sizeof(int[" << Fibonacci<10>::value << "]) = "
              << sizeof(int[Fibonacci<10>::value]) << " bytes\n";

    return 0;
}
