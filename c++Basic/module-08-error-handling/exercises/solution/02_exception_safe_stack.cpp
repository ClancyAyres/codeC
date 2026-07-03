#include <iostream>
#include <vector>
#include <stdexcept>
#include <utility>

template<typename T>
class ExceptionSafeStack {
public:
    ExceptionSafeStack() = default;
    ~ExceptionSafeStack() = default;

    // 拷贝: 可能抛异常（依赖 T 的拷贝构造）
    ExceptionSafeStack(const ExceptionSafeStack& other) : data_(other.data_) {}

    ExceptionSafeStack& operator=(const ExceptionSafeStack& other) {
        if (this != &other) {
            data_ = other.data_;  // 如果抛异常，data_ 保持原状
        }
        return *this;
    }

    // 移动: noexcept（依赖 T 的 noexcept 移动）
    ExceptionSafeStack(ExceptionSafeStack&& other) noexcept
        : data_(std::move(other.data_)) {}

    ExceptionSafeStack& operator=(ExceptionSafeStack&& other) noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
        }
        return *this;
    }

    void push(const T& value) { data_.push_back(value); }
    void push(T&& value) { data_.push_back(std::move(value)); }

    T pop() {
        if (data_.empty()) {
            throw std::runtime_error("Stack is empty");
        }
        T top = std::move(data_.back());
        data_.pop_back();
        return top;
    }

    const T& top() const {
        if (data_.empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return data_.back();
    }

    bool empty() const noexcept { return data_.empty(); }
    size_t size() const noexcept { return data_.size(); }

private:
    std::vector<T> data_;
};

int main() {
    ExceptionSafeStack<int> s;
    s.push(1);
    s.push(2);

    std::cout << "Top: " << s.top() << '\n';
    std::cout << "Pop: " << s.pop() << '\n';

    try {
        ExceptionSafeStack<int> emptyStack;
        emptyStack.pop();
    } catch (const std::runtime_error& e) {
        std::cout << "Exception: " << e.what() << '\n';
    }

    // 验证 noexcept
    std::cout << "empty() noexcept: " << noexcept(s.empty()) << '\n';
    std::cout << "size() noexcept: " << noexcept(s.size()) << '\n';

    return 0;
}
