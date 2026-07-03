#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Animal {
protected:
    std::string name_;
public:
    Animal(const std::string& name) : name_(name) {}
    virtual ~Animal() = default;
    virtual std::string sound() const = 0;

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

void addAnimal(std::vector<std::unique_ptr<Animal>>& zoo,
               std::unique_ptr<Animal> animal) {
    zoo.push_back(std::move(animal));
}

int main() {
    std::vector<std::unique_ptr<Animal>> zoo;

    addAnimal(zoo, std::make_unique<Dog>("Buddy"));
    addAnimal(zoo, std::make_unique<Cat>("Whiskers"));

    std::cout << "=== Zoo ===\n";
    for (const auto& animal : zoo) {
        std::cout << animal->describe() << '\n';
    }

    return 0;
}
