#include <iostream>
#include <memory>
#include <string>

struct Person {
    std::string name;
    std::shared_ptr<Person> spouse;    // TODO: 这里导致循环引用！应该用什么类型？

    Person(const std::string& n) : name(n) {
        std::cout << "Person(" << name << ") created\n";
    }
    ~Person() {
        std::cout << "Person(" << name << ") destroyed\n";
    }

    // TODO: 实现 getSpouse() 方法，安全地获取配偶
    // std::shared_ptr<Person> getSpouse() const {
    //     // TODO: 如果 spouse 是 weak_ptr，调用 lock()
    //     // 如果 spouse 是 shared_ptr，直接返回
    // }
};

int main() {
    std::cout << "=== Test 1: Create couple ===\n";
    auto alice = std::make_shared<Person>("Alice");
    auto bob = std::make_shared<Person>("Bob");

    alice->spouse = bob;
    bob->spouse = alice;   // BUG: 循环引用在这里形成

    std::cout << "  alice.use_count() = " << alice.use_count() << " (expect 2)\n";
    std::cout << "  bob.use_count()   = " << bob.use_count() << " (expect 2)\n";

    // TODO: 使用 getSpouse() 检查配偶
    // auto aliceSpouse = alice->getSpouse();
    // if (aliceSpouse) {
    //     std::cout << "  Alice's spouse: " << aliceSpouse->name << '\n';
    // }

    std::cout << "\n=== Test 2: Reset alice, check bob's spouse ===\n";
    alice.reset();
    std::cout << "  After alice.reset():\n";

    // TODO: 检查 bob 的配偶是否还存在
    // auto bobSpouse = bob->getSpouse();
    // if (bobSpouse) {
    //     std::cout << "  Bob's spouse: " << bobSpouse->name << '\n';
    // } else {
    //     std::cout << "  Bob's spouse: (none)\n";
    // }

    std::cout << "  bob.use_count() = " << bob.use_count() << '\n';

    std::cout << "\n=== Test 3: Cleanup ===\n";
    bob.reset();

    std::cout << "\n=== Note ===\n";
    std::cout << "The BUG: both spouse members are shared_ptr, creating a cycle.\n";
    std::cout << "After alice.reset(), alice's refcount goes from 2 to 1 (bob->spouse still holds it).\n";
    std::cout << "After bob.reset(), bob's refcount goes from 2 to 1 (alice->spouse still holds it).\n";
    std::cout << "Neither destructor is called — memory leak!\n";
    std::cout << "\nFix: change spouse to std::weak_ptr<Person>.\n";

    return 0;
}
