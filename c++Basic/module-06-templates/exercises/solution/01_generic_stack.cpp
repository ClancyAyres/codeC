#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <utility>

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

    const T& top() const {
        if (data_.empty()) throw std::runtime_error("Stack is empty");
        return data_.back();
    }

    bool empty() const { return data_.empty(); }
    size_t size() const { return data_.size(); }
    void clear() { data_.clear(); }

private:
    std::vector<T> data_;
};

int main() {
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    std::cout << "size: " << s.size() << '\n';
    std::cout << "top: " << s.top() << '\n';

    while (!s.empty()) {
        std::cout << s.pop() << ' ';
    }
    std::cout << '\n';

    // 测试 string
    Stack<std::string> words;
    words.push("hello");
    words.push("world");
    std::cout << words.pop() << '\n';

    // 测试空栈异常
    try {
        Stack<int> emptyStack;
        emptyStack.pop();
    } catch (const std::runtime_error& e) {
        std::cout << "Exception: " << e.what() << '\n';
    }

    return 0;
}
