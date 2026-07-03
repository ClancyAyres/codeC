#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

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
    explicit Circle(double radius) : radius_(radius) {}

    double area() const override {
        return 3.14159 * radius_ * radius_;
    }

    double perimeter() const override {
        return 2 * 3.14159 * radius_;
    }

    const char* name() const override {
        return "Circle";
    }
};

class Rectangle : public Shape {
    double width_, height_;

public:
    Rectangle(double width, double height) : width_(width), height_(height) {}

    double area() const override {
        return width_ * height_;
    }

    double perimeter() const override {
        return 2 * (width_ + height_);
    }

    const char* name() const override {
        return "Rectangle";
    }
};

int main() {
    std::cout << "=== Circle ===\n";
    Circle c(5.0);
    std::cout << "  area: " << c.area() << " (expect 78.53975)\n";
    std::cout << "  perimeter: " << c.perimeter() << " (expect 31.4159)\n";

    std::cout << "=== Rectangle ===\n";
    Rectangle r(3.0, 4.0);
    std::cout << "  area: " << r.area() << " (expect 12)\n";
    std::cout << "  perimeter: " << r.perimeter() << " (expect 14)\n";

    std::cout << "=== Polymorphism ===\n";
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(3.0, 4.0));

    for (const auto& s : shapes) {
        std::cout << "  " << s->name() << ": area=" << s->area() << '\n';
    }

    return 0;
}
