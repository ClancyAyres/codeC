// 模块 6 示例: 类模板 & 模板特化
// 相关规范: Core Guidelines T.1

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

// ✅ 类模板: 替代 Java 泛型类
template<typename T>
class Stack {
public:
    void push(const T& value) { data_.push_back(value); }
    void push(T&& value) { data_.push_back(std::move(value)); }

    T pop() {
        if (data_.empty()) throw std::runtime_error("Stack is empty");
        T top = std::move(data_.back());
        data_.pop_back();
        return top;
    }

    bool empty() const { return data_.empty(); }
    size_t size() const { return data_.size(); }

private:
    std::vector<T> data_;
};

// ✅ 模板特化: bool 专用版本（用更紧凑的存储）
template<>
class Stack<bool> {
public:
    void push(bool value) { bits_ = (bits_ << 1) | static_cast<int>(value); ++count_; }
    bool pop() {
        if (count_ == 0) throw std::runtime_error("Stack is empty");
        bool result = (bits_ >> (count_ - 1)) & 1;
        --count_;
        return result;
    }
    bool empty() const { return count_ == 0; }

private:
    unsigned long long bits_ = 0;
    size_t count_ = 0;
};

int main() {
    Stack<int> intStack;
    intStack.push(1);
    intStack.push(2);
    intStack.push(3);

    while (!intStack.empty()) {
        std::cout << intStack.pop() << ' ';
    }
    std::cout << '\n';

    Stack<std::string> strStack;
    strStack.push("hello");
    strStack.push("world");
    std::cout << strStack.pop() << '\n';

    // bool 特化
    Stack<bool> boolStack;
    boolStack.push(true);
    boolStack.push(false);
    boolStack.push(true);
    std::cout << std::boolalpha;
    while (!boolStack.empty()) {
        std::cout << boolStack.pop() << ' ';
    }
    std::cout << '\n';

    return 0;
}
