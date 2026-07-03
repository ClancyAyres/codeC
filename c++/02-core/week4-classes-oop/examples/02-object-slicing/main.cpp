// Example 02: Object Slicing — 派生类按值传递导致数据丢失
// Java 中不存在此问题（所有对象变量都是引用）

#include <iostream>
#include <memory>
#include <vector>
#include <string>

class Animal {
protected:
    std::string name_;
public:
    Animal(const std::string& name) : name_(name) {}
    virtual ~Animal() = default;

    virtual void speak() const {
        std::cout << "Animal " << name_ << " makes a sound\n";
    }

    virtual void info() const {
        std::cout << "Animal(name=" << name_ << ")\n";
    }
};

class Dog : public Animal {
    std::string breed_;
    int age_;
public:
    Dog(const std::string& name, const std::string& breed, int age)
        : Animal(name), breed_(breed), age_(age) {
    }

    void speak() const override {
        std::cout << "Dog " << name_ << " (" << breed_
                  << ", " << age_ << "y) barks: Woof!\n";
    }

    void info() const override {
        std::cout << "Dog(name=" << name_
                  << ", breed=" << breed_
                  << ", age=" << age_ << ")\n";
    }
};

class Cat : public Animal {
    std::string color_;
public:
    Cat(const std::string& name, const std::string& color)
        : Animal(name), color_(color) {
    }

    void speak() const override {
        std::cout << "Cat " << name_ << " (" << color_
                  << ") meows: Meow!\n";
    }

    void info() const override {
        std::cout << "Cat(name=" << name_
                  << ", color=" << color_ << ")\n";
    }
};

// BUG: 按值传递 — 导致对象切片
void processByValue(Animal a) {
    std::cout << "  [processByValue] calling speak(): ";
    a.speak();   // 始终调用 Animal::speak() — 切片！
    std::cout << "  [processByValue] calling info(): ";
    a.info();    // 始终调用 Animal::info() — 派生类数据丢失！
}

// FIX: 按 const 引用传递 — 保留多态
void processByRef(const Animal& a) {
    std::cout << "  [processByRef]   calling speak(): ";
    a.speak();   // 虚函数调用 — 正确多态
    std::cout << "  [processByRef]   calling info(): ";
    a.info();    // 虚函数调用 — 正确多态
}

// FIX: 按指针传递
void processByPtr(const Animal* a) {
    if (!a) return;
    std::cout << "  [processByPtr]   calling speak(): ";
    a->speak();
    std::cout << "  [processByPtr]   calling info(): ";
    a->info();
}

void slicingInVector() {
    std::cout << "\n=== Slicing in vector<Animal> ===\n";
    std::vector<Animal> animals;
    animals.push_back(Dog("Rex", "German Shepherd", 3));  // 切片！
    animals.push_back(Cat("Whiskers", "Orange"));          // 切片！

    for (const auto& a : animals) {
        a.speak();  // 全部调用 Animal::speak()
    }
}

void noSlicingWithPointers() {
    std::cout << "\n=== No slicing with vector<unique_ptr<Animal>> ===\n";
    std::vector<std::unique_ptr<Animal>> animals;
    animals.push_back(std::make_unique<Dog>("Rex", "German Shepherd", 3));
    animals.push_back(std::make_unique<Cat>("Whiskers", "Orange"));

    for (const auto& a : animals) {
        a->speak();  // 正确的多态调用
    }
}

int main() {
    Dog dog("Buddy", "Golden Retriever", 5);
    Cat cat("Luna", "Black");

    std::cout << "=== Direct calls (baseline) ===\n";
    std::cout << "  Direct dog.speak(): ";
    dog.speak();
    std::cout << "  Direct cat.speak(): ";
    cat.speak();

    std::cout << "\n=== Object Slicing: pass by value ===\n";
    std::cout << "Dog passed by value:\n";
    processByValue(dog);  // SLICING! breed_ 和 age_ 丢失
    std::cout << "Cat passed by value:\n";
    processByValue(cat);  // SLICING! color_ 丢失

    std::cout << "\n=== Correct: pass by const reference ===\n";
    std::cout << "Dog passed by reference:\n";
    processByRef(dog);
    std::cout << "Cat passed by reference:\n";
    processByRef(cat);

    std::cout << "\n=== Correct: pass by pointer ===\n";
    processByPtr(&dog);
    processByPtr(&cat);

    slicingInVector();
    noSlicingWithPointers();

    return 0;
}
