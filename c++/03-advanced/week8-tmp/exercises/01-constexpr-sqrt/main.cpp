// Exercise 01: constexpr sqrt — 编译时整数平方根
// TODO: 实现 constexpr int sqrt(int n)，用二分查找返回 ⌊√n⌋

#include <iostream>

// TODO: 实现 constexpr sqrt
constexpr int sqrt(int n) {
    // TODO: if n <= 1, return n
    // TODO: binary search: low=1, high=n, ans=0
    // TODO: while low <= high, mid = (low+high)/2, if mid*mid <= n: ans=mid, low=mid+1 else high=mid-1
    return 0;  // placeholder
    // TODO: return ans
}

int main() {
    std::cout << "=== constexpr sqrt ===\n";

    // TODO: uncomment and test
    // constexpr int s0  = sqrt(0);
    // constexpr int s1  = sqrt(1);
    // constexpr int s4  = sqrt(4);
    // constexpr int s9  = sqrt(9);
    // constexpr int s16 = sqrt(16);
    // constexpr int s17 = sqrt(17);
    // constexpr int s25 = sqrt(25);
    // constexpr int s100 = sqrt(100);
    // constexpr int s1000 = sqrt(1000);

    // std::cout << "sqrt(0) = " << s0 << '\n';
    // std::cout << "sqrt(1) = " << s1 << '\n';
    // std::cout << "sqrt(4) = " << s4 << '\n';
    // std::cout << "sqrt(9) = " << s9 << '\n';
    // std::cout << "sqrt(16) = " << s16 << '\n';
    // std::cout << "sqrt(17) = " << s17 << '\n';
    // std::cout << "sqrt(25) = " << s25 << '\n';
    // std::cout << "sqrt(100) = " << s100 << '\n';
    // std::cout << "sqrt(1000) = " << s1000 << '\n';

    // TODO: static_assert tests
    // static_assert(sqrt(0) == 0);
    // static_assert(sqrt(1) == 1);
    // static_assert(sqrt(4) == 2);
    // static_assert(sqrt(16) == 4);
    // static_assert(sqrt(17) == 4);
    // static_assert(sqrt(100) == 10);

    // std::cout << "\nAll static_asserts passed!\n";

    return 0;
}
