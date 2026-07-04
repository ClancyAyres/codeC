// 模块 1 示例: enum vs enum class
// 相关规范: Core Guidelines Enum.3, Enum.7; Google Style: Enumerations

#include <iostream>

// ❌ C 风格 enum — 枚举值泄漏到全局命名空间
enum OldColor { RED, GREEN, BLUE };

// ✅ enum class (C++11) — 类型安全，枚举值在作用域内
enum class Color { Red, Green, Blue };
enum class Direction { North, South, East, West };

int main() {
    // === C 风格 enum 的问题 ===
    int x = RED;  // ❌ 隐式转换为 int，x = 0
    std::cout << "RED as int: " << x << '\n';

    // int y = GREEN;  // GREEN 是全局的，可能和其他枚举冲突

    // === enum class: 类型安全 ===
    Color c = Color::Red;  // ✅ 必须带作用域
    // int z = c;  // ❌ 编译错误: 不能隐式转换

    // ✅ 显式转换
    int colorIndex = static_cast<int>(c);
    std::cout << "Color::Red as int: " << colorIndex << '\n';

    // ✅ 比较: 只和同类型比较
    if (c == Color::Red) {
        std::cout << "It's Red!\n";
    }
    // if (c == Direction::North) {}  // ❌ 不同类型不能比较

    // ✅ 可以指定底层类型
    enum class Flags : unsigned char {
        Read = 1 << 0,
        Write = 1 << 1,
        Execute = 1 << 2
    };

    Flags permission = Flags::Read;
    // 位运算需要显式转换
    unsigned char raw = static_cast<unsigned char>(Flags::Read)
                      | static_cast<unsigned char>(Flags::Write);
    std::cout << "Read|Write raw: " << static_cast<int>(raw) << '\n';

    return 0;
}
