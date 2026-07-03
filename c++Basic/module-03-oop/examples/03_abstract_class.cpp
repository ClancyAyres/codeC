// 模块 3 示例: 抽象类与接口模式
// 相关规范: Core Guidelines C.121, C.129

#include <iostream>
#include <memory>
#include <vector>
#include <string>

class IShape {
public:
    virtual ~IShape() = default;
    virtual double area() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public IShape {
public:
    explicit Circle(double r) : radius_(r) {}
    double area() const override { return 3.14159 * radius_ * radius_; }
    std::string name() const override { return "Circle"; }
private:
    double radius_;
};

class Rectangle : public IShape {
public:
    Rectangle(double w, double h) : width_(w), height_(h) {}
    double area() const override { return width_ * height_; }
    std::string name() const override { return "Rectangle"; }
private:
    double width_, height_;
};

// NVI 模式
class ShapeWithLogging : public IShape {
public:
    double area() const final {
        double a = doArea();
        std::cout << "  [log] " << name() << " area = " << a << '\n';
        return a;
    }
private:
    virtual double doArea() const = 0;
};

class LoggedCircle : public ShapeWithLogging {
public:
    explicit LoggedCircle(double r) : radius_(r) {}
    std::string name() const override { return "LoggedCircle"; }
private:
    double doArea() const override { return 3.14159 * radius_ * radius_; }
    double radius_;
};

int main() {
    std::vector<std::unique_ptr<IShape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(3.0, 4.0));
    shapes.push_back(std::make_unique<LoggedCircle>(2.0));

    for (const auto& s : shapes) {
        std::cout << s->name() << " area: " << s->area() << '\n';
    }

    return 0;
}
