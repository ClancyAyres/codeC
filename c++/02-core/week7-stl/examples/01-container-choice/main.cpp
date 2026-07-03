// Example 01: Container Choice — 各容器性能特征对比
// 演示：每种容器的插入/查找/删除复杂度及适用场景

#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <array>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <queue>
#include <string>
#include <iomanip>

template<typename T>
void printHeader(const T& title) {
    std::cout << "\n" << title << "\n";
    std::cout << std::string(70, '-') << '\n';
}

int main() {
    // ── 序列容器 ──────────────────────────────────────────
    printHeader("=== 序列容器 (Sequence Containers) ===");

    // vector: 动态数组，连续内存，尾部 O(1)，随机访问 O(1)
    {
        std::cout << "std::vector<int>  v = {1, 2, 3};\n";
        std::vector<int> v = {1, 2, 3};
        v.push_back(4);
        v.insert(v.begin(), 0);  // 头部插入 O(n)
        std::cout << "  push_back(4), insert(begin, 0) → ";
        for (auto x : v) std::cout << x << ' ';
        std::cout << "\n  v[2] = " << v[2] << "  (O(1) random access)\n";
        std::cout << "  &v[0]=" << v.data() << "  (contiguous memory)\n";
    }

    // deque: 双端队列，分段连续，两端 O(1)
    {
        std::cout << "\nstd::deque<int>  dq = {1, 2, 3};\n";
        std::deque<int> dq = {1, 2, 3};
        dq.push_back(4);
        dq.push_front(0);       // 头部 O(1)
        std::cout << "  push_front(0), push_back(4) → ";
        for (auto x : dq) std::cout << x << ' ';
        std::cout << "\n  dq[2] = " << dq[2] << "  (O(1) random access, slightly slower than vector)\n";
    }

    // list: 双向链表，任意位置插入 O(1)，无随机访问
    {
        std::cout << "\nstd::list<int>  lst = {1, 2, 3};\n";
        std::list<int> lst = {1, 2, 3};
        auto it = lst.begin();
        ++it;  // 指向第二个元素
        lst.insert(it, 99);     // 中间插入 O(1)
        std::cout << "  insert at position 1 value 99 → ";
        for (auto x : lst) std::cout << x << ' ';
        std::cout << "\n  No operator[] — only bidirectional iteration\n";
    }

    // forward_list: 单向链表，最省内存
    {
        std::cout << "\nstd::forward_list<int>  fl = {1, 2, 3};\n";
        std::forward_list<int> fl = {1, 2, 3};
        fl.push_front(0);
        std::cout << "  push_front(0) → ";
        for (auto x : fl) std::cout << x << ' ';
        std::cout << "\n  No push_back(), no size(), forward-only traversal\n";
    }

    // array: 固定大小，栈上分配，零开销
    {
        std::cout << "\nstd::array<int, 5>  arr = {1, 2, 3, 4, 5};\n";
        std::array<int, 5> arr = {1, 2, 3, 4, 5};
        std::cout << "  arr.size() = " << arr.size() << "  (compile-time constant)\n";
        std::cout << "  arr.front()=" << arr.front() << ", arr.back()=" << arr.back() << '\n';
        std::cout << "  Stack-allocated, no heap, wraps C array with STL interface\n";
    }

    // ── 关联容器（有序）────────────────────────────────────
    printHeader("=== 关联容器 — 有序 (Ordered, Red-Black Tree) ===");

    // map: 键有序，O(log n)
    {
        std::cout << "std::map<std::string, int>  scores;\n";
        std::map<std::string, int> scores;
        scores["Alice"] = 95;
        scores["Bob"] = 87;
        scores["Charlie"] = 92;
        scores.insert({"Diana", 88});
        std::cout << "  Sorted by key:\n";
        for (const auto& [name, score] : scores) {
            std::cout << "    " << name << ": " << score << '\n';
        }
        std::cout << "  scores[\"Alice\"] = " << scores["Alice"] << "  O(log n)\n";
    }

    // set: 有序唯一值
    {
        std::cout << "\nstd::set<int>  s = {3, 1, 4, 1, 5, 9, 2, 6};\n";
        std::set<int> s = {3, 1, 4, 1, 5, 9, 2, 6};
        std::cout << "  Auto-sorted, deduplicated → ";
        for (auto x : s) std::cout << x << ' ';
        std::cout << "\n  s.count(3)=" << s.count(3) << ", s.count(7)=" << s.count(7) << '\n';
    }

    // ── 关联容器（无序）────────────────────────────────────
    printHeader("=== 关联容器 — 无序 (Unordered, Hash Table) ===");

    // unordered_map: 哈希表，平均 O(1)
    {
        std::cout << "std::unordered_map<std::string, int>  cache;\n";
        std::unordered_map<std::string, int> cache;
        cache["key1"] = 100;
        cache["key2"] = 200;
        std::cout << "  cache[\"key1\"] = " << cache["key1"] << "  average O(1)\n";
        std::cout << "  bucket_count=" << cache.bucket_count()
                  << ", load_factor=" << cache.load_factor() << '\n';
    }

    // unordered_set: 哈希集
    {
        std::cout << "\nstd::unordered_set<int>  us = {3, 1, 4, 1, 5};\n";
        std::unordered_set<int> us = {3, 1, 4, 1, 5};
        std::cout << "  Deduplicated, unordered → ";
        for (auto x : us) std::cout << x << ' ';
        std::cout << "\n  find(4) " << (us.find(4) != us.end() ? "found" : "not found") << '\n';
    }

    // ── 容器适配器 ─────────────────────────────────────────
    printHeader("=== 容器适配器 (Container Adapters) ===");

    // stack: LIFO
    {
        std::cout << "std::stack<int>  stk;\n";
        std::stack<int> stk;
        stk.push(1); stk.push(2); stk.push(3);
        std::cout << "  top() = " << stk.top() << '\n';
        stk.pop();
        std::cout << "  after pop(), top() = " << stk.top() << '\n';
    }

    // queue: FIFO
    {
        std::cout << "\nstd::queue<int>  q;\n";
        std::queue<int> q;
        q.push(1); q.push(2); q.push(3);
        std::cout << "  front()=" << q.front() << ", back()=" << q.back() << '\n';
        q.pop();
        std::cout << "  after pop(), front()=" << q.front() << '\n';
    }

    // priority_queue: 大顶堆（默认）
    {
        std::cout << "\nstd::priority_queue<int>  pq;\n";
        std::priority_queue<int> pq;
        pq.push(3); pq.push(1); pq.push(4); pq.push(1); pq.push(5);
        std::cout << "  top() = " << pq.top() << "  (max element)\n";
        std::cout << "  Note: Java PriorityQueue is min-heap by default!\n";

        // 小顶堆
        std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
        minHeap.push(3); minHeap.push(1); minHeap.push(4);
        std::cout << "  Min-heap top() = " << minHeap.top()
                  << "  (using std::greater<int>)\n";
    }

    // ── 汇总表 ─────────────────────────────────────────────
    printHeader("=== 容器选择速查表 ===");
    std::cout << std::left
              << std::setw(24) << "Container"
              << std::setw(14) << "Insert"
              << std::setw(14) << "Lookup"
              << std::setw(14) << "Delete"
              << "Notes\n";
    std::cout << std::string(80, '-') << '\n';
    std::cout << std::setw(24) << "vector"
              << std::setw(14) << "O(1) back"
              << std::setw(14) << "O(1)"
              << std::setw(14) << "O(n)"
              << "Contiguous, cache-friendly\n";
    std::cout << std::setw(24) << "deque"
              << std::setw(14) << "O(1) ends"
              << std::setw(14) << "O(1)"
              << std::setw(14) << "O(n)"
              << "Chunked, good for queues\n";
    std::cout << std::setw(24) << "list"
              << std::setw(14) << "O(1) anywhere"
              << std::setw(14) << "O(n)"
              << std::setw(14) << "O(1)"
              << "No random access\n";
    std::cout << std::setw(24) << "map"
              << std::setw(14) << "O(log n)"
              << std::setw(14) << "O(log n)"
              << std::setw(14) << "O(log n)"
              << "Sorted by key\n";
    std::cout << std::setw(24) << "unordered_map"
              << std::setw(14) << "O(1) avg"
              << std::setw(14) << "O(1) avg"
              << std::setw(14) << "O(1) avg"
              << "Hash table\n";
    std::cout << std::setw(24) << "stack"
              << std::setw(14) << "O(1)"
              << std::setw(14) << "O(1) top"
              << std::setw(14) << "O(1)"
              << "LIFO adapter\n";
    std::cout << std::setw(24) << "queue"
              << std::setw(14) << "O(1)"
              << std::setw(14) << "O(1) front"
              << std::setw(14) << "O(1)"
              << "FIFO adapter\n";

    return 0;
}
