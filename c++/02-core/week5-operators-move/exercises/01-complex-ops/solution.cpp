#include <iostream>

class Complex {
    double real_, imag_;

public:
    Complex(double r = 0.0, double i = 0.0) : real_(r), imag_(i) {}

    Complex operator+(const Complex& other) const {
        return Complex(real_ + other.real_, imag_ + other.imag_);
    }

    Complex operator-(const Complex& other) const {
        return Complex(real_ - other.real_, imag_ - other.imag_);
    }

    Complex operator*(const Complex& other) const {
        return Complex(
            real_ * other.real_ - imag_ * other.imag_,
            real_ * other.imag_ + imag_ * other.real_
        );
    }

    Complex& operator+=(const Complex& other) {
        real_ += other.real_;
        imag_ += other.imag_;
        return *this;
    }

    Complex& operator-=(const Complex& other) {
        real_ -= other.real_;
        imag_ -= other.imag_;
        return *this;
    }

    Complex& operator*=(const Complex& other) {
        double newReal = real_ * other.real_ - imag_ * other.imag_;
        double newImag = real_ * other.imag_ + imag_ * other.real_;
        real_ = newReal;
        imag_ = newImag;
        return *this;
    }

    bool operator==(const Complex& other) const {
        return real_ == other.real_ && imag_ == other.imag_;
    }

    bool operator!=(const Complex& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Complex& c);

    Complex operator-() const {
        return Complex(-real_, -imag_);
    }
};

std::ostream& operator<<(std::ostream& os, const Complex& c) {
    os << c.real_;
    if (c.imag_ >= 0) {
        os << " + " << c.imag_ << "i";
    } else {
        os << " - " << -c.imag_ << "i";
    }
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
