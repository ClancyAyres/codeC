// 模块 3 示例: 多重继承
// 相关规范: Core Guidelines C.135, C.136, C.137

#include <iostream>
#include <memory>
#include <string>

class IDrawable {
public:
    virtual ~IDrawable() = default;
    virtual void draw() const = 0;
};

class ISerializable {
public:
    virtual ~ISerializable() = default;
    virtual std::string serialize() const = 0;
};

class Circle : public IDrawable, public ISerializable {
public:
    explicit Circle(double r) : radius_(r) {}

    void draw() const override {
        std::cout << "Circle(r=" << radius_ << ")\n";
    }

    std::string serialize() const override {
        return "Circle:" + std::to_string(radius_);
    }

private:
    double radius_;
};

class A {
public:
    int value = 10;
};

class B : virtual public A {};
class C : virtual public A {};
class D : public B, public C {};

int main() {
    auto c = std::make_unique<Circle>(3.0);
    c->draw();

    IDrawable* d = c.get();
    d->draw();

    ISerializable* s = c.get();
    std::cout << s->serialize() << '\n';

    D dObj;
    std::cout << "D::value = " << dObj.value << '\n';

    return 0;
}
