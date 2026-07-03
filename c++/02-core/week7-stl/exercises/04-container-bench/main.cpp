#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <chrono>
#include <iomanip>
#include <string>

using Clock = std::chrono::high_resolution_clock;

// 辅助：输出单行结果
void printResult(const std::string& name, long long ms, const std::string& note = "") {
    std::cout << std::left << std::setw(10) << name
              << std::setw(8) << ms << "ms";
    if (!note.empty()) std::cout << "  " << note;
    std::cout << '\n';
}

// TODO: 实现 benchPushBack
void benchPushBack(size_t N) {
    std::cout << "\n=== push_back (N=" << N << ") ===\n";
    // TODO: 分别测试 vector, list, deque 的 push_back 性能
}

// TODO: 实现 benchPushFront
void benchPushFront(size_t N) {
    std::cout << "\n=== push_front (N=" << N << ") ===\n";
    // TODO: 测试头部插入性能
    // vector 不支持 push_front，用 insert(begin) 模拟
}

// TODO: 实现 benchRandomAccess
void benchRandomAccess(size_t N) {
    std::cout << "\n=== random_access (N=" << N << ") ===\n";
    // TODO: 测试随机访问性能
    // list 不支持 operator[]，用 std::advance 模拟
}

// TODO: 实现 benchIteration
void benchIteration(size_t N) {
    std::cout << "\n=== iteration (N=" << N << ") ===\n";
    // TODO: 测试顺序遍历性能
}

// TODO: 实现 benchInsertMiddle
void benchInsertMiddle(size_t N) {
    std::cout << "\n=== insert_middle (N=" << N << ") ===\n";
    // TODO: 测试在中间位置插入的性能
    // 在已有 N 个元素的容器中间插入 1000 个元素
}

int main() {
    const size_t N = 100000;

    benchPushBack(N);
    benchPushFront(N);
    benchRandomAccess(N);
    benchIteration(N);
    benchInsertMiddle(N);

    std::cout << "\n=== Summary ===\n";
    std::cout << "vector: best for random access, tail ops, iteration\n";
    std::cout << "deque:  best for front+back ops, good random access\n";
    std::cout << "list:   best for middle insert/delete, no random access\n";

    return 0;
}
