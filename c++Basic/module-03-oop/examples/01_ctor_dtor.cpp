// 模块 3 示例: 构造与析构
// 相关规范: Core Guidelines C.41, C.47, C.48

#include <iostream>
#include <string>
#include <memory>

class Base {
public:
    Base() { std::cout << "  Base()\n"; }
    virtual ~Base() { std::cout << "  ~Base()\n"; }
};

class Member {
public:
    Member(const std::string& n) : name(n) {
        std::cout << "  Member(" << name << ")\n";
    }
    ~Member() { std::cout << "  ~Member(" << name << ")\n"; }
    std::string name;
};

class Derived : public Base {
public:
    Derived(const std::string& n)
        : Base(),
          m1_(n + "_m1"),
          m2_(n + "_m2")
    {
        std::cout << "  Derived() body\n";
    }

    ~Derived() override {
        std::cout << "  ~Derived() body\n";
    }

private:
    Member m1_;
    Member m2_;
};

int main() {
    std::cout << "=== 栈上对象 ===\n";
    {
        Derived d("stack");
    }

    std::cout << "\n=== 堆上对象 (delete 基类指针) ===\n";
    {
        Base* b = new Derived("heap");
        delete b;
    }

    std::cout << "\n=== unique_ptr 版本 ===\n";
    {
        auto b = std::make_unique<Derived>("unique");
    }

    return 0;
}
