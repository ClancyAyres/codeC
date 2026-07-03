#include <iostream>

class Complex {
    double real_, imag_;

public:
    Complex(double r = 0.0, double i = 0.0) : real_(r), imag_(i) {}

    // TODO: 实现 operator+
    Complex operator+(const Complex& other) const {
        // TODO
        return Complex();
    }

    // TODO: 实现 operator-
    Complex operator-(const Complex& other) const {
        // TODO
        return Complex();
    }

    // TODO: 实现 operator* — 公式: (ac-bd) + (ad+bc)i
    Complex operator*(const Complex& other) const {
        // TODO
        return Complex();
    }

    // TODO: 实现 operator+=
    Complex& operator+=(const Complex& other) {
        // TODO
        return *this;
    }

    // TODO: 实现 operator-=
    Complex& operator-=(const Complex& other) {
        // TODO
        return *this;
    }

    // TODO: 实现 operator*=
    Complex& operator*=(const Complex& other) {
        // TODO
        return *this;
    }

    // TODO: 实现 operator==
    bool operator==(const Complex& other) const {
        // TODO
        return false;
    }

    // TODO: 实现 operator!=
    bool operator!=(const Complex& other) const {
        // TODO
        return false;
    }

    // TODO: 实现 operator<<（友元函数，已在类外声明）
    friend std::ostream& operator<<(std::ostream& os, const Complex& c);

    // TODO: 实现一元负号 operator-
    Complex operator-() const {
        // TODO
        return Complex();
    }
};

// TODO: 实现 operator<< — 输出格式: "3 + 4i"（注意 imag_ 为负时的处理）
std::ostream& operator<<(std::ostream& os, const Complex& c) {
    // TODO
    return os;
}

int main() {
    Complex c1(3, 4);
    Complex c2(1, 2);

    std::cout << "=== Arithmetic ===\n";
    std::cout << "c1 = " << c1 << '\n';
    std::cout << "c2 = " << c2 << '\n';
    std::cout << "c1 + c2 = " << (c1 + c2) << '\n';
    std::cout << "c1 - c2 = " << (c1 - c2) << '\n';
    std::cout << "c1 * c2 = " << (c1 * c2) << '\n';

    std::cout << "\n=== Compound Assignment ===\n";
    Complex c3 = c1;
    c3 += c2;
    std::cout << "c1 += c2 = " << c3 << '\n';
    c3 = c1;
    c3 -= c2;
    std::cout << "c1 -= c2 = " << c3 << '\n';
    c3 = c1;
    c3 *= c2;
    std::cout << "c1 *= c2 = " << c3 << '\n';

    std::cout << "\n=== Comparison ===\n";
    std::cout << "c1 == c2: " << (c1 == c2 ? "true" : "false") << '\n';
    std::cout << "c1 != c2: " << (c1 != c2 ? "true" : "false") << '\n';
    std::cout << "c1 == Complex(3, 4): "
              << (c1 == Complex(3, 4) ? "true" : "false") << '\n';

    std::cout << "\n=== Unary Minus ===\n";
    std::cout << "-c1 = " << (-c1) << '\n';

    return 0;
}
