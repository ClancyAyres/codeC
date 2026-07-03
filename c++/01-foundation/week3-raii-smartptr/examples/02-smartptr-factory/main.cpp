#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Animal {
public:
    virtual ~Animal() = default;
    virtual void speak() const = 0;
    virtual std::string type() const = 0;
};

class Dog : public Animal {
    std::string name_;
public:
    explicit Dog(const std::string& name) : name_(name) {}
    void speak() const override {
        std::cout << "Dog(" << name_ << "): Woof!\n";
    }
    std::string type() const override { return "Dog"; }
};

class Cat : public Animal {
    std::string name_;
public:
    explicit Cat(const std::string& name) : name_(name) {}
    void speak() const override {
        std::cout << "Cat(" << name_ << "): Meow!\n";
    }
    std::string type() const override { return "Cat"; }
};

class Duck : public Animal {
    std::string name_;
public:
    explicit Duck(const std::string& name) : name_(name) {}
    void speak() const override {
        std::cout << "Duck(" << name_ << "): Quack!\n";
    }
    std::string type() const override { return "Duck"; }
};

std::unique_ptr<Animal> createAnimal(const std::string& type, const std::string& name) {
    if (type == "dog")  return std::make_unique<Dog>(name);
    if (type == "cat")  return std::make_unique<Cat>(name);
    if (type == "duck") return std::make_unique<Duck>(name);
    return nullptr;
}

void useAnimal(const Animal& animal) {
    std::cout << "  Using (const&): ";
    animal.speak();
}

void takeOwnership(std::unique_ptr<Animal> animal) {
    std::cout << "  Taking ownership of " << animal->type() << ": ";
    animal->speak();
    // animal 离开作用域，对象被 delete
}

int main() {
    std::cout << "=== Factory: create different animals ===\n";
    auto dog = createAnimal("dog", "Buddy");
    auto cat = createAnimal("cat", "Whiskers");
    auto duck = createAnimal("duck", "Donald");

    dog->speak();
    cat->speak();
    duck->speak();

    std::cout << "\n=== Using via const reference (no ownership transfer) ===\n";
    useAnimal(*dog);
    useAnimal(*cat);
    // dog 和 cat 仍然有效

    std::cout << "\n=== Transferring ownership ===\n";
    takeOwnership(std::move(dog));
    // dog 现在是 nullptr
    if (!dog) {
        std::cout << "  dog is now nullptr after move\n";
    }

    std::cout << "\n=== Storing in vector ===\n";
    std::vector<std::unique_ptr<Animal>> zoo;
    zoo.push_back(std::move(cat));
    zoo.push_back(std::move(duck));
    zoo.push_back(createAnimal("dog", "Rex"));

    for (const auto& animal : zoo) {
        animal->speak();
    }

    std::cout << "\n=== All animals will be auto-deleted ===\n";
    // zoo 析构 → 每个 unique_ptr 析构 → 每个 Animal 对象 delete
    return 0;
}
