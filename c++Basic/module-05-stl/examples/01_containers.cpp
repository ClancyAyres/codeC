// 模块 5 示例: 核心容器用法
// 相关规范: Core Guidelines SL.con; Google Style: Use of STL

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <string>
#include <algorithm>

struct Task {
    int id;
    std::string title;
    int priority;
};

int main() {
    // === vector: 替代 Java ArrayList ===
    std::vector<int> nums = {5, 2, 8, 1, 9};
    nums.push_back(3);
    nums.pop_back();

    std::vector<Task> tasks;
    tasks.emplace_back(Task{1, "Fix bug", 5});
    tasks.emplace_back(Task{2, "Add feature", 3});

    // === map: 有序 (红黑树) 替代 Java TreeMap ===
    std::map<std::string, int> orderedScores;
    orderedScores["Alice"] = 90;
    orderedScores["Bob"] = 85;
    orderedScores["Charlie"] = 95;

    for (const auto& [name, score] : orderedScores) {
        std::cout << name << ": " << score << '\n';
    }

    // ✅ 插入不覆盖 — 类似 Java putIfAbsent
    auto [it, inserted] = orderedScores.insert({"Alice", 100});
    if (!inserted) {
        std::cout << "Alice already exists with score " << it->second << '\n';
    }

    // === unordered_map: 哈希表 替代 Java HashMap ===
    std::unordered_map<std::string, int> hashScores;
    hashScores["Alice"] = 90;

    // === set: 有序集合 ===
    std::set<int> sorted = {3, 1, 4, 1, 5, 9};
    std::cout << "set: ";
    for (int x : sorted) std::cout << x << ' ';
    std::cout << '\n';

    // === 查找 ===
    if (sorted.find(4) != sorted.end()) {
        std::cout << "Found 4\n";
    }

    return 0;
}
