// 模块 3 习题答案: 图形层次结构
// 规范: Core Guidelines C.35, C.121, C.128

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <iomanip>

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string description() const = 0;
};

class Circle : public Shape {
public:
    explicit Circle(double r) : radius_(r) {
        if (r <= 0) throw std::invalid_argument("Radius must be positive");
    }

    double area() const override {
        return M_PI * radius_ * radius_;
    }

    double perimeter() const override {
        return 2 * M_PI * radius_;
    }

    std::string description() const override {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2)
            << "Circle(r=" << radius_
            << ", area=" << area()
            << ", perimeter=" << perimeter() << ")";
        return oss.str();
    }

private:
    double radius_;
};

class Rectangle : public Shape {
public:
    Rectangle(double w, double h) : width_(w), height_(h) {
        if (w <= 0 || h <= 0)
            throw std::invalid_argument("Dimensions must be positive");
    }

    double area() const override { return width_ * height_; }
    double perimeter() const override { return 2 * (width_ + height_); }

    std::string description() const override {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2)
            << "Rectangle(w=" << width_ << ", h=" << height_
            << ", area=" << area()
            << ", perimeter=" << perimeter() << ")";
        return oss.str();
    }

private:
    double width_, height_;
};

std::unique_ptr<Shape> createShape(const std::string& type,
                                    double a, double b = 0) {
    if (type == "circle") {
        return std::make_unique<Circle>(a);
    }
    if (type == "rectangle") {
        return std::make_unique<Rectangle>(a, b);
    }
    throw std::invalid_argument("Unknown shape type: " + type);
}

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(3.0, 4.0));
    shapes.push_back(createShape("circle", 2.0, 0));
    shapes.push_back(createShape("rectangle", 6.0, 2.0));

    double totalArea = 0;
    double totalPerimeter = 0;

    for (const auto& s : shapes) {
        std::cout << s->description() << '\n';
        totalArea += s->area();
        totalPerimeter += s->perimeter();
    }

    std::cout << "\nTotal area: " << totalArea << '\n';
    std::cout << "Total perimeter: " << totalPerimeter << '\n';

    try {
        createShape("triangle", 1, 2);
    } catch (const std::invalid_argument& e) {
        std::cout << "\nCaught expected exception: " << e.what() << '\n';
    }

    return 0;
}
