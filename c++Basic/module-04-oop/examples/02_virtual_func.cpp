// 模块 3 示例: 虚函数与多态
// 相关规范: Core Guidelines C.128, C.35

#include <iostream>
#include <memory>
#include <vector>
#include <string>

class Animal {
public:
    explicit Animal(std::string n) : name_(std::move(n)) {}
    virtual ~Animal() = default;

    virtual std::string sound() const = 0;
    virtual std::string describe() const {
        return name_ + " makes sound: " + sound();
    }

    const std::string& name() const { return name_; }
private:
    std::string name_;
};

class Dog : public Animal {
public:
    explicit Dog(std::string n) : Animal(std::move(n)) {}
    std::string sound() const override { return "Woof!"; }
};

class Cat : public Animal {
public:
    explicit Cat(std::string n) : Animal(std::move(n)) {}
    std::string sound() const override { return "Meow!"; }
    std::string describe() const override {
        return Animal::describe() + " (a cat)";
    }
};

class SilentDog : public Dog {
public:
    explicit SilentDog(std::string n) : Dog(std::move(n)) {}
    std::string sound() const { return "..."; }
};

int main() {
    std::vector<std::unique_ptr<Animal>> animals;
    animals.push_back(std::make_unique<Dog>("Buddy"));
    animals.push_back(std::make_unique<Cat>("Mittens"));

    for (const auto& a : animals) {
        std::cout << a->describe() << '\n';
    }

    Dog* silent = new SilentDog("Ghost");
    std::cout << "SilentDog via Dog*: " << silent->sound() << '\n';
    delete silent;

    return 0;
}
