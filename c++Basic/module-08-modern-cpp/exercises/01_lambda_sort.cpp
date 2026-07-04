// 使用 lambda 实现三种排序

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

struct Employee {
    std::string name;
    int age;
    double salary;
};

int main() {
    std::vector<Employee> staff = {
        {"Alice", 30, 50000},
        {"Bob", 25, 60000},
        {"Charlie", 35, 50000},
        {"Diana", 25, 55000},
    };

    // TODO: 1. 按 salary 降序
    // TODO: 2. 按 age 升序，同 age 按 name 字母序
    // TODO: 3. 按 name 长度升序

    return 0;
}
