# 习题 01：复数类运算符重载

## 目标

实现一个完整的 `Complex` 复数类，重载算术运算符和比较运算符。

## 要求

实现以下成员函数和运算符：

```cpp
class Complex {
    double real_, imag_;
public:
    Complex(double r = 0.0, double i = 0.0);

    // 算术运算符
    Complex operator+(const Complex& other) const;
    Complex operator-(const Complex& other) const;
    Complex operator*(const Complex& other) const;

    // 复合赋值
    Complex& operator+=(const Complex& other);
    Complex& operator-=(const Complex& other);
    Complex& operator*=(const Complex& other);

    // 比较
    bool operator==(const Complex& other) const;
    bool operator!=(const Complex& other) const;

    // 流输出
    friend std::ostream& operator<<(std::ostream& os, const Complex& c);

    // 一元负号
    Complex operator-() const;
};
```

## 公式

- 加法：`(a + bi) + (c + di) = (a + c) + (b + d)i`
- 减法：`(a + bi) - (c + di) = (a - c) + (b - d)i`
- 乘法：`(a + bi) × (c + di) = (ac - bd) + (ad + bc)i`

## 提示

- 用 `operator+` 委托给 `operator+=` 实现
- 友元函数可访问私有成员
- `operator-()`（一元负号）返回新对象，不修改自身

## 预期输出

```
=== Arithmetic ===
c1 = 3 + 4i
c2 = 1 + 2i
c1 + c2 = 4 + 6i
c1 - c2 = 2 + 2i
c1 * c2 = -5 + 10i

=== Compound Assignment ===
c1 += c2 = 4 + 6i
c1 -= c2 = 3 + 4i
c1 *= c2 = -5 + 10i

=== Comparison ===
c1 == c2: false
c1 != c2: true
c1 == Complex(3, 4): true

=== Unary Minus ===
-c1 = -3 + -4i
```
