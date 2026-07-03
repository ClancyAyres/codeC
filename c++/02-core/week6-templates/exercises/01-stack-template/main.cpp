#include <iostream>
#include <stdexcept>

// TODO: 实现 Stack<T> 类模板
template<typename T>
class Stack {
    T* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;

public:
    Stack() = default;

    ~Stack() {
        // TODO: 释放内存
    }

    void push(const T& value) {
        // TODO: 如果 size_ >= capacity_，扩容（capacity_==0 → 4，否则 → 2倍）
        // TODO: 放入新元素
    }

    void pop() {
        // TODO: 如果为空，抛出 std::underflow_error("Stack is empty")
        // TODO: --size_
    }

    T& top() {
        // TODO: 如果为空，抛出 std::underflow_error("Stack is empty")
        // TODO: 返回 data_[size_-1]
    }

    const T& top() const {
        // TODO: const 版本
    }

    bool empty() const {
        // TODO
        return true;
    }

    size_t size() const {
        // TODO
        return 0;
    }
};

int main() {
    Stack<int> s;

    std::cout << "=== Push ===\n";
    s.push(10);
    s.push(20);
    s.push(30);
    std::cout << "Pushed 10, 20, 30\n";
    std::cout << "Top: " << s.top() << '\n';
    std::cout << "Size: " << s.size() << '\n';
    std::cout << "Empty: " << (s.empty() ? "true" : "false") << '\n';

    std::cout << "\n=== Pop ===\n";
    s.pop();
    std::cout << "Popped. New top: " << s.top() << '\n';
    std::cout << "Size: " << s.size() << '\n';

    std::cout << "\n=== Pop All ===\n";
    std::cout << "Popped: " << s.top() << '\n';
    s.pop();
    std::cout << "Popped: " << s.top() << '\n';
    s.pop();
    std::cout << "Stack empty: " << (s.empty() ? "true" : "false") << '\n';
    std::cout << "Size: " << s.size() << '\n';

    std::cout << "\n=== Re-push ===\n";
    s.push(100);
    s.push(200);
    std::cout << "Pushed 100, 200\n";
    std::cout << "Top: " << s.top() << '\n';

    return 0;
}
