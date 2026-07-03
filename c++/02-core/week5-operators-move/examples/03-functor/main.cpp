// Example 03: Functor — operator() 实现自定义比较器和函数对象
// 演示：operator() 与 std::sort 结合使用

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Person {
    std::string name;
    int age;

    Person(std::string n, int a) : name(std::move(n)), age(a) {}
};

// 按年龄升序比较 — 函子
struct ByAgeAsc {
    bool operator()(const Person& a, const Person& b) const {
        return a.age < b.age;
    }
};

// 按年龄降序比较 — 函子
struct ByAgeDesc {
    bool operator()(const Person& a, const Person& b) const {
        return a.age > b.age;
    }
};

// 按名字排序 — 函子
struct ByName {
    bool operator()(const Person& a, const Person& b) const {
        return a.name < b.name;
    }
};

// 自定义绝对值排序 — 函子
struct AbsCompare {
    bool operator()(int a, int b) const {
        return std::abs(a) < std::abs(b);
    }
};

// 累计器函子 — 带状态
class Accumulator {
    int sum_ = 0;
    int count_ = 0;
public:
    void operator()(int x) {
        sum_ += x;
        ++count_;
    }

    double average() const {
        return count_ > 0 ? static_cast<double>(sum_) / count_ : 0.0;
    }
};

// 生成器函子 — 每次调用返回下一个值
class Counter {
    int value_;
public:
    explicit Counter(int start = 0) : value_(start) {}
    int operator()() { return value_++; }
};

// 乘法函子
struct Multiply {
    int factor_;
    explicit Multiply(int f) : factor_(f) {}
    int operator()(int x) const { return x * factor_; }
};

int main() {
    std::cout << "=== Custom Comparators ===\n";
    std::vector<Person> people = {
        {"Alice", 30}, {"Bob", 25}, {"Charlie", 35},
        {"Diana", 28}, {"Eve", 22}
    };

    // 使用函子排序
    std::sort(people.begin(), people.end(), ByAgeAsc{});
    std::cout << "By age (ascending):\n";
    for (const auto& p : people) {
        std::cout << "  " << p.name << ": " << p.age << '\n';
    }

    std::sort(people.begin(), people.end(), ByName{});
    std::cout << "\nBy name:\n";
    for (const auto& p : people) {
        std::cout << "  " << p.name << ": " << p.age << '\n';
    }

    std::cout << "\n=== AbsCompare ===\n";
    std::vector<int> nums = {-5, 3, -1, 4, -2, 0, 1};
    std::sort(nums.begin(), nums.end(), AbsCompare{});
    std::cout << "Sorted by absolute value:";
    for (int n : nums) std::cout << ' ' << n;
    std::cout << '\n';

    std::cout << "\n=== Accumulator Functor ===\n";
    Accumulator acc = std::for_each(nums.begin(), nums.end(), Accumulator{});
    std::cout << "Average: " << acc.average() << '\n';

    std::cout << "\n=== Counter (Generator) ===\n";
    std::vector<int> sequence(10);
    std::generate(sequence.begin(), sequence.end(), Counter(100));
    std::cout << "Generated sequence:";
    for (int n : sequence) std::cout << ' ' << n;
    std::cout << '\n';

    std::cout << "\n=== Multiply (Transformer) ===\n";
    Multiply times3(3);
    std::cout << "5 * 3 = " << times3(5) << '\n';
    std::cout << "7 * 3 = " << times3(7) << '\n';

    return 0;
}
