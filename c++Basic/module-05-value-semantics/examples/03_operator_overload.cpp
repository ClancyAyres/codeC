// 模块 4 示例: 运算符重载
// 相关规范: Core Guidelines C.160, C.161

#include <iostream>

class Vec2 {
public:
    double x, y;

    Vec2(double x_, double y_) : x(x_), y(y_) {}

    Vec2& operator+=(const Vec2& rhs) { x += rhs.x; y += rhs.y; return *this; }
    Vec2& operator*=(double scalar) { x *= scalar; y *= scalar; return *this; }
    Vec2 operator-() const { return Vec2(-x, -y); }

    bool operator==(const Vec2& rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator!=(const Vec2& rhs) const { return !(*this == rhs); }
};

Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
    Vec2 result = lhs;
    result += rhs;
    return result;
}

Vec2 operator*(const Vec2& lhs, double rhs) {
    Vec2 result = lhs;
    result *= rhs;
    return result;
}

Vec2 operator*(double lhs, const Vec2& rhs) { return rhs * lhs; }

std::ostream& operator<<(std::ostream& os, const Vec2& v) {
    return os << "(" << v.x << ", " << v.y << ")";
}

int main() {
    Vec2 a(1.0, 2.0);
    Vec2 b(3.0, 4.0);

    std::cout << "a = " << a << '\n';
    std::cout << "b = " << b << '\n';
    std::cout << "a + b = " << (a + b) << '\n';
    std::cout << "a * 2 = " << (a * 2) << '\n';
    std::cout << "3 * b = " << (3 * b) << '\n';
    std::cout << "-a = " << (-a) << '\n';

    a += b;
    std::cout << "a += b → " << a << '\n';

    std::cout << "a == b: " << (a == b) << '\n';

    return 0;
}
