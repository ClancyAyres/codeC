#include <iostream>
#include <cstring>

namespace math {
    int max(int a, int b) {
        return a > b ? a : b;
    }
}

namespace text {
    const char* max(const char* a, const char* b) {
        return std::strlen(a) > std::strlen(b) ? a : b;
    }
}

int max(int a, int b) {
    return a + b;  // 故意不同语义，演示命名空间隔离
}

int main() {
    // 完全限定名调用
    std::cout << "math::max(3, 5) = " << math::max(3, 5) << '\n';
    std::cout << "text::max(\"hello\", \"hi\") = "
              << text::max("hello", "hi") << '\n';
    std::cout << "::max(3, 5) = " << ::max(3, 5) << '\n';

    // using 声明
    using math::max;
    std::cout << "using math::max 后, max(10, 20) = " << max(10, 20) << '\n';
    // 全局 ::max 被隐藏，必须显式调用
    std::cout << "using math::max 后, ::max(10, 20) = " << ::max(10, 20) << '\n';

    return 0;
}
