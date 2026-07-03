// Example 02: Move Performance — 拷贝 vs 移动性能对比
// 演示：vector push_back 使用拷贝 vs 移动的性能差异

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// 一个大对象，模拟昂贵的拷贝
class BigObject {
    std::vector<int> data_;
    std::string label_;

public:
    explicit BigObject(size_t n, std::string label = "")
        : data_(n, 42), label_(std::move(label)) {
    }

    BigObject(const BigObject& other)
        : data_(other.data_), label_(other.label_) {
    }

    BigObject(BigObject&& other) noexcept
        : data_(std::move(other.data_))
        , label_(std::move(other.label_)) {
    }

    BigObject& operator=(const BigObject&) = default;
    BigObject& operator=(BigObject&&) = default;

    size_t size() const { return data_.size(); }
    const std::string& label() const { return label_; }
};

class Timer {
    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point start_;
    const char* name_;
public:
    explicit Timer(const char* name) : start_(Clock::now()), name_(name) {}
    ~Timer() {
        auto end = Clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start_).count();
        std::cout << std::setw(30) << std::left << name_
                  << ": " << std::setw(6) << ms << " ms\n";
    }
};

constexpr size_t DATA_SIZE = 10'000'000;  // 10M ints ≈ 40MB per object
constexpr int PUSH_COUNT = 20;

void testCopyPushBack() {
    Timer t("Copy push_back");
    std::vector<BigObject> v;
    BigObject obj(DATA_SIZE, "source");
    for (int i = 0; i < PUSH_COUNT; ++i) {
        v.push_back(obj);  // 每次拷贝 40MB
    }
    std::cout << "  vector size: " << v.size()
              << ", element size: " << v[0].size() << '\n';
}

void testMovePushBack() {
    Timer t("Move push_back");
    std::vector<BigObject> v;
    for (int i = 0; i < PUSH_COUNT; ++i) {
        v.push_back(BigObject(DATA_SIZE, "temp"));  // 移动临时对象
    }
    std::cout << "  vector size: " << v.size()
              << ", element size: " << v[0].size() << '\n';
}

void testEmplaceBack() {
    Timer t("Emplace back");
    std::vector<BigObject> v;
    for (int i = 0; i < PUSH_COUNT; ++i) {
        v.emplace_back(DATA_SIZE, "emplaced");  // 原地构造
    }
    std::cout << "  vector size: " << v.size()
              << ", element size: " << v[0].size() << '\n';
}

int main() {
    std::cout << "=== Move vs Copy Performance ===\n";
    std::cout << "Data size: " << DATA_SIZE << " ints ("
              << (DATA_SIZE * sizeof(int) / (1024.0 * 1024.0))
              << " MB) per object\n";
    std::cout << "Push count: " << PUSH_COUNT << "\n\n";

    testCopyPushBack();
    std::cout << '\n';
    testMovePushBack();
    std::cout << '\n';
    testEmplaceBack();

    std::cout << "\n=== Explanation ===\n";
    std::cout << "Copy:  Each push_back copies the full 40MB data.\n";
    std::cout << "       Vector reallocation also copies all elements.\n";
    std::cout << "Move:  push_back moves the temporary object (O(1)).\n";
    std::cout << "       Vector reallocation moves elements (O(1) each).\n";
    std::cout << "Emplace: Constructs in-place — no copy, no move.\n";

    return 0;
}
