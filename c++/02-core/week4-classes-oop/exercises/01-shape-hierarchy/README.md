# 习题 01：形状继承体系

## 目标

实现一个形状（Shape）继承体系，使用虚函数实现多态。

## 要求

实现以下类：

```cpp
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual const char* name() const = 0;
};

class Circle : public Shape {
    double radius_;
public:
    explicit Circle(double radius);
    double area() const override;
    double perimeter() const override;
    const char* name() const override;
};

class Rectangle : public Shape {
    double width_, height_;
public:
    Rectangle(double width, double height);
    double area() const override;
    double perimeter() const override;
    const char* name() const override;
};
```

## 公式

- Circle: area = π × r², perimeter = 2 × π × r
- Rectangle: area = width × height, perimeter = 2 × (width + height)
- 使用 π ≈ 3.14159

## 提示

- 纯虚函数使 Shape 成为抽象类，不能实例化
- 使用 `override` 关键字确保正确重写
- 使用 `const` 成员函数

## 预期输出

```
=== Circle ===
  area: 78.5397 (expect 78.53975)
  perimeter: 31.4159 (expect 31.4159)
=== Rectangle ===
  area: 12 (expect 12)
  perimeter: 14 (expect 14)
=== Polymorphism ===
  Circle: area=78.5397
  Rectangle: area=12
```
