#include <iostream>
#include <memory>
#include <string>

struct Person {
    std::string name;
    std::weak_ptr<Person> spouse;

    Person(const std::string& n) : name(n) {
        std::cout << "Person(" << name << ") created\n";
    }
    ~Person() {
        std::cout << "Person(" << name << ") destroyed\n";
    }

    std::shared_ptr<Person> getSpouse() const {
        return spouse.lock();
    }
};

int main() {
    std::cout << "=== Test 1: Create couple ===\n";
    auto alice = std::make_shared<Person>("Alice");
    auto bob = std::make_shared<Person>("Bob");

    alice->spouse = bob;
    bob->spouse = alice;

    std::cout << "  alice.use_count() = " << alice.use_count() << " (expect 1)\n";
    std::cout << "  bob.use_count()   = " << bob.use_count() << " (expect 1)\n";

    auto aliceSpouse = alice->getSpouse();
    if (aliceSpouse) {
        std::cout << "  Alice's spouse: " << aliceSpouse->name << " (expect Bob)\n";
    }

    auto bobSpouse = bob->getSpouse();
    if (bobSpouse) {
        std::cout << "  Bob's spouse: " << bobSpouse->name << " (expect Alice)\n";
    }

    std::cout << "\n=== Test 2: Reset alice, check bob's spouse ===\n";
    alice.reset();
    std::cout << "  After alice.reset():\n";

    auto bobSpouse2 = bob->getSpouse();
    if (bobSpouse2) {
        std::cout << "  Bob's spouse: " << bobSpouse2->name << '\n';
    } else {
        std::cout << "  Bob's spouse: (none)\n";
    }

    std::cout << "  bob.use_count() = " << bob.use_count() << '\n';

    std::cout << "\n=== Test 3: Cleanup ===\n";
    bob.reset();

    std::cout << "\nAll done. Both Person objects destroyed — no leak!\n";
    return 0;
}
