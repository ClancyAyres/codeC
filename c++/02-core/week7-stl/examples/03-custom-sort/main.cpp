// Example 03: Custom Sort — lambda comparator, function object, multi-field sorting
// 演示：std::sort 的各种自定义排序方式

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

struct Person {
    std::string name;
    int age;
    double salary;

    friend std::ostream& operator<<(std::ostream& os, const Person& p) {
        return os << p.name << " (age=" << p.age << ", salary=" << p.salary << ')';
    }
};

// 方式1: 函数对象 (Functor)
struct ByAgeAsc {
    bool operator()(const Person& a, const Person& b) const {
        return a.age < b.age;
    }
};

struct BySalaryDesc {
    bool operator()(const Person& a, const Person& b) const {
        return a.salary > b.salary;
    }
};

// 方式2: 普通函数
bool byNameAsc(const Person& a, const Person& b) {
    return a.name < b.name;
}

void printPeople(const std::string& label, const std::vector<Person>& people) {
    std::cout << label << ":\n";
    for (const auto& p : people) {
        std::cout << "  " << p << '\n';
    }
}

int main() {
    std::vector<Person> people = {
        {"Alice",   30, 75000.0},
        {"Bob",     25, 85000.0},
        {"Charlie", 35, 65000.0},
        {"Diana",   25, 90000.0},
        {"Eve",     30, 80000.0},
    };
    printPeople("Original", people);

    // ── Lambda Comparator ─────────────────────────────────
    {
        auto sorted = people;
        std::sort(sorted.begin(), sorted.end(),
            [](const Person& a, const Person& b) {
                return a.age < b.age;  // 按年龄升序
            });
        printPeople("\nSort by age (lambda)", sorted);
    }

    // ── Function Object (Functor) ─────────────────────────
    {
        auto sorted = people;
        std::sort(sorted.begin(), sorted.end(), ByAgeAsc());
        printPeople("\nSort by age (functor ByAgeAsc)", sorted);
    }

    {
        auto sorted = people;
        std::sort(sorted.begin(), sorted.end(), BySalaryDesc());
        printPeople("\nSort by salary desc (functor BySalaryDesc)", sorted);
    }

    // ── Plain Function ────────────────────────────────────
    {
        auto sorted = people;
        std::sort(sorted.begin(), sorted.end(), byNameAsc);
        printPeople("\nSort by name (function)", sorted);
    }

    // ── std::greater / std::less ──────────────────────────
    {
        std::vector<int> nums = {3, 1, 4, 1, 5, 9, 2, 6};
        std::sort(nums.begin(), nums.end(), std::greater<int>());
        std::cout << "\nSort ints descending (std::greater): ";
        for (auto x : nums) std::cout << x << ' ';
        std::cout << '\n';
    }

    // ── Multi-Field Sorting ───────────────────────────────
    // 先按年龄升序，年龄相同按薪资降序
    {
        auto sorted = people;
        std::sort(sorted.begin(), sorted.end(),
            [](const Person& a, const Person& b) {
                if (a.age != b.age) return a.age < b.age;
                return a.salary > b.salary;
            });
        printPeople("\nSort by age asc, then salary desc (multi-field)", sorted);
    }

    // ── std::stable_sort ──────────────────────────────────
    // 稳定排序：相等元素保持原有顺序
    {
        auto sorted = people;
        std::stable_sort(sorted.begin(), sorted.end(),
            [](const Person& a, const Person& b) {
                return a.age < b.age;
            });
        printPeople("\nStable sort by age", sorted);
        std::cout << "  (Bob before Diana, Alice before Eve — original order preserved)\n";
    }

    // ── Partial Sort ──────────────────────────────────────
    // 找出 top 3 按薪资
    {
        auto top3 = people;
        std::partial_sort(top3.begin(), top3.begin() + 3, top3.end(),
            [](const Person& a, const Person& b) {
                return a.salary > b.salary;
            });
        printPeople("\nPartial sort: top 3 by salary", {top3.begin(), top3.begin() + 3});
    }

    // ── nth_element ───────────────────────────────────────
    // 找出中位数（按年龄）
    {
        auto median = people;
        auto mid = median.begin() + median.size() / 2;
        std::nth_element(median.begin(), mid, median.end(),
            [](const Person& a, const Person& b) {
                return a.age < b.age;
            });
        std::cout << "\nnth_element (median by age): " << *mid << '\n';
    }

    // ── sort with projection (C++20 ranges, concept preview) ──
    // C++20 ranges:  people | std::ranges::sort(less{}, &Person::age);
    // C++17 manual approach:
    {
        std::cout << "\n=== Sorting with projection (C++17 manual) ===\n";
        auto sorted = people;
        std::sort(sorted.begin(), sorted.end(),
            [](const Person& a, const Person& b) {
                return a.name.length() < b.name.length();
            });
        printPeople("Sort by name length", sorted);
    }

    return 0;
}
