#include <iostream>
#include <stdexcept>
#include <utility>

template<typename T>
class Stack {
    T* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;

public:
    Stack() = default;

    ~Stack() {
        delete[] data_;
    }

    void push(const T& value) {
        if (size_ >= capacity_) {
            size_t newCapacity = capacity_ == 0 ? 4 : capacity_ * 2;
            T* newData = new T[newCapacity];
            for (size_t i = 0; i < size_; ++i) {
                newData[i] = std::move(data_[i]);
            }
            delete[] data_;
            data_ = newData;
            capacity_ = newCapacity;
        }
        data_[size_++] = value;
    }

    void pop() {
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        --size_;
    }

    T& top() {
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        return data_[size_ - 1];
    }

    const T& top() const {
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        return data_[size_ - 1];
    }

    bool empty() const {
        return size_ == 0;
    }

    size_t size() const {
        return size_;
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
