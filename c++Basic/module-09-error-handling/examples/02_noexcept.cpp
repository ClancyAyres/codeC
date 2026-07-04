// 模块 8 示例: noexcept

#include <iostream>
#include <vector>
#include <string>

// ✅ noexcept: 承诺不抛异常
// 移动构造标记 noexcept 可以让 vector 在扩容时用移动而非拷贝
class NoThrowMovable {
public:
    explicit NoThrowMovable(std::string data) : data_(std::move(data)) {
        std::cout << "  [构造] " << data_ << '\n';
    }

    // ✅ noexcept 移动构造
    NoThrowMovable(NoThrowMovable&& other) noexcept : data_(std::move(other.data_)) {
        std::cout << "  [移动构造] " << data_ << '\n';
    }

    // ❌ 拷贝构造不标记 noexcept（因为它可能抛异常）
    NoThrowMovable(const NoThrowMovable& other) : data_(other.data_) {
        std::cout << "  [拷贝构造] " << data_ << '\n';
    }

    ~NoThrowMovable() { std::cout << "  [析构] " << data_ << '\n'; }

private:
    std::string data_;
};

// ✅ noexcept 可条件化
template<typename T>
void swapIfNoexcept(T& a, T& b) noexcept(noexcept(a.swap(b))) {
    a.swap(b);
}

int main() {
    std::cout << "--- vector 扩容测试 ---\n";
    std::vector<NoThrowMovable> vec;
    vec.reserve(1);

    std::cout << "push_back #1: (不移动)\n";
    vec.emplace_back("first");

    // 扩容时: 因为移动构造是 noexcept，vector 使用移动而非拷贝 — 高效!
    std::cout << "push_back #2: (扩容，用移动)\n";
    vec.emplace_back("second");

    std::cout << "push_back #3: (扩容，继续用移动)\n";
    vec.emplace_back("third");

    return 0;
}
