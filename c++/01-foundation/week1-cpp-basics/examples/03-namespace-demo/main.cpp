#include <iostream>
#include <cstring>

// 数学命名空间：定义数值版本的 max
namespace math {
    int max(int a, int b) {
        std::cout << "  [调用 math::max(int, int)]\n";
        return a > b ? a : b;
    }

    double max(double a, double b) {
        std::cout << "  [调用 math::max(double, double)]\n";
        return a > b ? a : b;
    }
}

// 文本命名空间：定义字符串版本的 max（按长度比较）
namespace text {
    const char* max(const char* a, const char* b) {
        std::cout << "  [调用 text::max(const char*, const char*)]\n";
        return std::strlen(a) > std::strlen(b) ? a : b;
    }
}

// 全局命名空间的 max（返回两数之和，故意不同语义以演示区分）
int max(int a, int b) {
    std::cout << "  [调用 ::max(int, int) — 全局]\n";
    return a + b;  // 故意实现为加法，演示不同语义
}

int main() {
    std::cout << "========== 命名空间演示 ==========\n\n";

    // 1. 完全限定名调用
    std::cout << "1. 完全限定名:\n";
    std::cout << "   math::max(3, 5) = " << math::max(3, 5) << '\n';
    std::cout << "   math::max(3.14, 2.71) = " << math::max(3.14, 2.71) << '\n';
    std::cout << "   text::max(\"hello\", \"world\") = "
              << text::max("hello", "world") << '\n';
    std::cout << "   ::max(3, 5) = " << ::max(3, 5) << '\n';

    // 2. using 声明：只引入 math::max
    std::cout << "\n2. using 声明:\n";
    using math::max;
    std::cout << "   max(10, 20) = " << max(10, 20) << '\n';
    std::cout << "   max(1.5, 2.5) = " << max(1.5, 2.5) << '\n';
    // ::max 被隐藏，需要显式调用
    std::cout << "   ::max(10, 20) = " << ::max(10, 20) << '\n';

    // 3. using 指令演示（仅在局部作用域使用）
    std::cout << "\n3. using 指令（块作用域内演示）:\n";
    {
        using namespace std;  // 仅在当前块作用域生效
        cout << "   在块作用域内 using namespace std 后可直接用 cout\n";
    }

    return 0;
}
