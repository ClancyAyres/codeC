#include <iostream>
#include <string>
#include <vector>

class Animal {
protected:
    std::string name_;
public:
    Animal(const std::string& name) : name_(name) {}
    virtual ~Animal() = default;
    virtual std::string sound() const { return "???"; }

    std::string describe() const {
        return name_ + " says " + sound();
    }
};

class Dog : public Animal {
public:
    Dog(const std::string& name) : Animal(name) {}
    std::string sound() const override { return "Woof"; }
};

class Cat : public Animal {
public:
    Cat(const std::string& name) : Animal(name) {}
    std::string sound() const override { return "Meow"; }
};

// TODO: BUG — 按值传递导致对象切片
// 修复：改为 unique_ptr<Animal>
void addAnimal(std::vector<Animal>& zoo, Animal animal) {
    zoo.push_back(animal);  // BUG: 切片！
}

int main() {
    // TODO: BUG — vector<Animal> 存储值，派生类数据会丢失
    // 修复：改为 vector<std::unique_ptr<Animal>>
    std::vector<Animal> zoo;

    Dog dog("Buddy");
    Cat cat("Whiskers");

    addAnimal(zoo, dog);  // BUG: 切片！丢失 Dog 的 sound()
    addAnimal(zoo, cat);  // BUG: 切片！丢失 Cat 的 sound()

    std::cout << "=== Zoo ===\n";
    for (const auto& animal : zoo) {
        std::cout << animal.describe() << '\n';
    }

    return 0;
}
