// 模块 1 示例: 类型转换
// 相关规范: Core Guidelines ES.48, ES.49; Google Style: Casting

#include <iostream>
#include <memory>

class Animal {
public:
    virtual ~Animal() = default;
    virtual std::string sound() const = 0;
};

class Dog : public Animal {
public:
    std::string sound() const override { return "Woof!"; }
    void fetch() { std::cout << "Dog fetches ball!\n"; }
};

class Cat : public Animal {
public:
    std::string sound() const override { return "Meow!"; }
};

int main() {
    std::cout << "=== static_cast ===\n";
    // 相关类型转换，编译期检查
    double pi = 3.14159;
    int ipi = static_cast<int>(pi);  // ✅ 明确的窄化转换
    std::cout << "pi as int: " << ipi << '\n';

    // 基类 → 派生类（不检查！你确定的话用 static_cast）
    Animal* animal = new Dog();
    Dog* dog1 = static_cast<Dog*>(animal);  // ⚠️ 不进行运行时检查
    dog1->fetch();

    std::cout << "\n=== dynamic_cast ===\n";
    // 多态类型下行，运行时安全检查
    Animal* unknown = new Cat();

    Dog* dog2 = dynamic_cast<Dog*>(unknown);
    if (dog2) {
        dog2->fetch();
    } else {
        std::cout << "Not a Dog! dynamic_cast returned nullptr\n";
    }

    // ✅ dynamic_cast 引用版本: 失败时抛 std::bad_cast
    try {
        Cat& catRef = dynamic_cast<Cat&>(*unknown);
        std::cout << "It's a Cat: " << catRef.sound() << '\n';
    } catch (const std::bad_cast&) {
        std::cout << "Not a Cat!\n";
    }

    delete animal;
    delete unknown;

    std::cout << "\n=== const_cast (谨慎使用) ===\n";
    // ⚠️ 仅用于兼容不接受 const 的旧 API
    const int safe = 100;
    // int* bad = &safe;  // ❌ 编译错误
    // int* hack = const_cast<int*>(&safe);  // ⚠️ 能编译但修改 const 对象是 UB

    std::cout << "const_cast should rarely be used\n";

    return 0;
}
