#include <iostream>
#include <cstring>

template<typename T>
int compare(const T& a, const T& b) {
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

template<>
int compare<const char*>(const char* const& a, const char* const& b) {
    int result = std::strcmp(a, b);
    if (result < 0) return -1;
    if (result > 0) return 1;
    return 0;
}

int main() {
    std::cout << "=== Generic (int) ===\n";
    std::cout << "compare(3, 5) = " << compare(3, 5) << '\n';
    std::cout << "compare(5, 3) = " << compare(5, 3) << '\n';
    std::cout << "compare(3, 3) = " << compare(3, 3) << '\n';

    std::cout << "\n=== Generic (double) ===\n";
    std::cout << "compare(3.14, 2.72) = " << compare(3.14, 2.72) << '\n';
    std::cout << "compare(1.0, 2.0) = " << compare(1.0, 2.0) << '\n';

    std::cout << "\n=== Specialized (const char*) ===\n";
    const char* s1 = "apple";
    const char* s2 = "banana";
    const char* s3 = "hello";
    const char* s4 = "zebra";
    std::cout << "compare(\"apple\", \"banana\") = "
              << compare(s1, s2)
              << "  (content: \"apple\" < \"banana\")\n";
    std::cout << "compare(\"hello\", \"hello\") = "
              << compare(s3, s3)
              << "  (content: equal)\n";
    std::cout << "compare(\"zebra\", \"apple\") = "
              << compare(s4, s1)
              << "  (content: \"zebra\" > \"apple\")\n";

    return 0;
}
