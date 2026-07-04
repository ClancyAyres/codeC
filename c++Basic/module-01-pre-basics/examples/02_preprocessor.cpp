// 模块 1 示例: 预处理器
// 相关规范: Core Guidelines SF.8; Google Style: Preprocessor Macros
//
// C++ 预处理器在编译前运行。Java 完全没有这个概念。

#include <iostream>
#include <string>

// ✅ #define 常量 — 但优先用 const/constexpr 替代
#define MAX_SIZE 100

// ✅ 条件编译 — 不同平台不同代码
#if defined(__APPLE__)
    #define PLATFORM "macOS"
#elif defined(__linux__)
    #define PLATFORM "Linux"
#else
    #define PLATFORM "Other"
#endif

// ⚠️ 宏函数 — 易出错，优先用 inline 函数
// ❌ Code Review 会打回: 带副作用的宏函数
// #define SQUARE(x) x * x   // SQUARE(1+2) = 1+2*1+2 = 5, 不是你想要的!

int main() {
    std::cout << "MAX_SIZE = " << MAX_SIZE << '\n';
    std::cout << "Running on: " << PLATFORM << '\n';

    // ✅ #ifdef 条件编译 — 调试代码只在 debug 模式下编译
    #ifdef DEBUG_MODE
        std::cout << "Debug: this line only appears with -DDEBUG_MODE\n";
    #endif

    // #error 编译期报错
    // #error "This code requires C++17 or later"

    return 0;
}
