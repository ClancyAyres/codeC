// 模块 4 习题答案: Vec2d
// 规范: Core Guidelines C.160, C.161

#include <iostream>
#include <cmath>
#include <sstream>

class Vec2d {
public:
    double x, y;

    Vec2d(double x_, double y_) : x(x_), y(y_) {}

    Vec2d& operator+=(const Vec2d& rhs) { x += rhs.x; y += rhs.y; return *this; }
    Vec2d& operator-=(const Vec2d& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
    Vec2d& operator*=(double s) { x *= s; y *= s; return *this; }
    Vec2d& operator/=(double s) { x /= s; y /= s; return *this; }

    double dot(const Vec2d& rhs) const { return x * rhs.x + y * rhs.y; }
    double lengthSq() const { return dot(*this); }
    double length() const { return std::sqrt(lengthSq()); }

    Vec2d normalized() const {
        double len = length();
        if (len < 1e-12) return *this;
        Vec2d result = *this;
        result /= len;
        return result;
    }

    bool operator==(const Vec2d& rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator!=(const Vec2d& rhs) const { return !(*this == rhs); }
};

Vec2d operator+(const Vec2d& lhs, const Vec2d& rhs) { Vec2d r = lhs; r += rhs; return r; }
Vec2d operator-(const Vec2d& lhs, const Vec2d& rhs) { Vec2d r = lhs; r -= rhs; return r; }
Vec2d operator*(const Vec2d& lhs, double rhs) { Vec2d r = lhs; r *= rhs; return r; }
Vec2d operator*(double lhs, const Vec2d& rhs) { return rhs * lhs; }
Vec2d operator/(const Vec2d& lhs, double rhs) { Vec2d r = lhs; r /= rhs; return r; }

std::ostream& operator<<(std::ostream& os, const Vec2d& v) {
    return os << "(" << v.x << ", " << v.y << ")";
}

int main() {
    Vec2d a(3.0, 4.0);
    Vec2d b(1.0, 2.0);

    std::cout << "a = " << a << '\n';
    std::cout << "b = " << b << '\n';
    std::cout << "a + b = " << (a + b) << '\n';
    std::cout << "a - b = " << (a - b) << '\n';
    std::cout << "a * 2 = " << (a * 2) << '\n';
    std::cout << "3 * b = " << (3 * b) << '\n';
    std::cout << "a / 2 = " << (a / 2) << '\n';
    std::cout << "|a| = " << a.length() << '\n';
    std::cout << "a · b = " << a.dot(b) << '\n';
    std::cout << "normalized a = " << a.normalized() << '\n';
    std::cout << "a == b: " << (a == b) << '\n';

    return 0;
}
