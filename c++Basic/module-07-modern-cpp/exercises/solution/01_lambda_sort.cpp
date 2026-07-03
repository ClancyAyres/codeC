#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

struct Employee {
    std::string name;
    int age;
    double salary;
};

std::ostream& operator<<(std::ostream& os, const Employee& e) {
    return os << e.name << " (age=" << e.age << ", salary=" << e.salary << ")";
}

int main() {
    std::vector<Employee> staff = {
        {"Alice", 30, 50000},
        {"Bob", 25, 60000},
        {"Charlie", 35, 50000},
        {"Diana", 25, 55000},
    };

    // 1. 按 salary 降序
    {
        auto sorted = staff;
        std::sort(sorted.begin(), sorted.end(),
                  [](const Employee& a, const Employee& b) {
                      return a.salary > b.salary;
                  });
        std::cout << "By salary desc:\n";
        for (const auto& e : sorted) std::cout << "  " << e << '\n';
    }

    // 2. 按 age 升序，同 age 按 name 字母序
    {
        auto sorted = staff;
        std::sort(sorted.begin(), sorted.end(),
                  [](const Employee& a, const Employee& b) {
                      if (a.age != b.age) return a.age < b.age;
                      return a.name < b.name;
                  });
        std::cout << "\nBy age asc, then name:\n";
        for (const auto& e : sorted) std::cout << "  " << e << '\n';
    }

    // 3. 按 name 长度升序
    {
        auto sorted = staff;
        std::sort(sorted.begin(), sorted.end(),
                  [](const Employee& a, const Employee& b) {
                      return a.name.length() < b.name.length();
                  });
        std::cout << "\nBy name length:\n";
        for (const auto& e : sorted) std::cout << "  " << e << '\n';
    }

    return 0;
}
