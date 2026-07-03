// Exercise 01 Solution: constexpr sqrt — 编译时整数平方根

#include <iostream>

constexpr int sqrt(int n) {
    if (n <= 1) return n;
    int low = 1, high = n, ans = 0;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (mid <= n / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

int main() {
    std::cout << "=== constexpr sqrt ===\n";

    constexpr int s0  = sqrt(0);
    constexpr int s1  = sqrt(1);
    constexpr int s4  = sqrt(4);
    constexpr int s9  = sqrt(9);
    constexpr int s16 = sqrt(16);
    constexpr int s17 = sqrt(17);
    constexpr int s25 = sqrt(25);
    constexpr int s100 = sqrt(100);
    constexpr int s1000 = sqrt(1000);

    std::cout << "sqrt(0) = " << s0 << '\n';
    std::cout << "sqrt(1) = " << s1 << '\n';
    std::cout << "sqrt(4) = " << s4 << '\n';
    std::cout << "sqrt(9) = " << s9 << '\n';
    std::cout << "sqrt(16) = " << s16 << '\n';
    std::cout << "sqrt(17) = " << s17 << '\n';
    std::cout << "sqrt(25) = " << s25 << '\n';
    std::cout << "sqrt(100) = " << s100 << '\n';
    std::cout << "sqrt(1000) = " << s1000 << '\n';

    static_assert(sqrt(0) == 0);
    static_assert(sqrt(1) == 1);
    static_assert(sqrt(4) == 2);
    static_assert(sqrt(16) == 4);
    static_assert(sqrt(17) == 4);
    static_assert(sqrt(100) == 10);

    std::cout << "\nAll static_asserts passed!\n";

    constexpr int s = sqrt(100);
    int arr[s];
    std::cout << "sizeof(arr[sqrt(100)]) = " << sizeof(arr) << " bytes\n";

    return 0;
}
