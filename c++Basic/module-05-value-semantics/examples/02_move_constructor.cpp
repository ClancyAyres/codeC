// 模块 4 示例: 值语义 vs 引用语义 — Java 开发者的思维转换
//
// Java: 变量=引用，赋值=引用赋值，两个变量指向同一个对象
// C++:  变量=值，赋值=拷贝，每个变量拥有独立的对象

#include <iostream>
#include <string>
#include <vector>

struct Person {
    std::string name;
    int age;

    Person(std::string n, int a) : name(std::move(n)), age(a) {}
};

int main() {
    std::cout << "=== Java 思维 vs C++ 思维 ===\n\n";

    std::cout << "--- Java 风格: 指针 ---\n";
    auto* p1 = new Person("Alice", 30);
    auto* p2 = p1;
    p2->age = 31;
    std::cout << "p1->age = " << p1->age << '\n';
    std::cout << "p2->age = " << p2->age << '\n';
    delete p1;

    std::cout << "\n--- C++ 风格: 值语义 ---\n";
    Person person1("Bob", 25);
    Person person2 = person1;
    person2.age = 26;
    std::cout << "person1.age = " << person1.age << '\n';
    std::cout << "person2.age = " << person2.age << '\n';

    auto birthday = [](Person p) { p.age += 1; };
    Person person3("Carol", 20);
    birthday(person3);
    std::cout << "After birthday: " << person3.age << '\n';

    auto birthdayRef = [](Person& p) { p.age += 1; };
    birthdayRef(person3);
    std::cout << "After birthdayRef: " << person3.age << '\n';

    // 值语义的威力: vector 拥有元素的所有权
    std::cout << "\n--- vector 值语义 ---\n";
    std::vector<Person> people;
    people.push_back(Person("Dave", 40));
    people.emplace_back("Eve", 35);

    for (const auto& person : people) {
        std::cout << person.name << ": " << person.age << '\n';
    }

    return 0;
}
