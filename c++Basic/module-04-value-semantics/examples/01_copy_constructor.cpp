// 模块 4 示例: 拷贝构造与三五法则
// 相关规范: Core Guidelines C.21, C.80

#include <iostream>
#include <string>
#include <cstring>
#include <utility>

class Buffer {
public:
    explicit Buffer(const std::string& data)
        : size_(data.size()), data_(new char[size_ + 1]) {
        std::strcpy(data_, data.c_str());
        std::cout << "  [构造] \"" << data_ << "\"\n";
    }

    Buffer(const Buffer& other)
        : size_(other.size_), data_(new char[size_ + 1]) {
        std::strcpy(data_, other.data_);
        std::cout << "  [拷贝构造] \"" << data_ << "\"\n";
    }

    Buffer& operator=(const Buffer& other) {
        std::cout << "  [拷贝赋值] \"" << other.data_ << "\"\n";
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new char[size_ + 1];
            std::strcpy(data_, other.data_);
        }
        return *this;
    }

    Buffer(Buffer&& other) noexcept
        : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr;
        std::cout << "  [移动构造]\n";
    }

    Buffer& operator=(Buffer&& other) noexcept {
        std::cout << "  [移动赋值]\n";
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
        }
        return *this;
    }

    ~Buffer() {
        std::cout << "  [析构] ";
        if (data_) std::cout << '"' << data_ << '"';
        std::cout << '\n';
        delete[] data_;
    }

    const char* c_str() const { return data_; }

private:
    size_t size_;
    char* data_;
};

int main() {
    std::cout << "--- 拷贝构造 ---\n";
    Buffer b1("Hello");
    Buffer b2 = b1;

    std::cout << "b1: " << b1.c_str() << '\n';
    std::cout << "b2: " << b2.c_str() << '\n';

    std::cout << "\n--- 拷贝赋值 ---\n";
    Buffer b4("Temp");
    b4 = b1;

    std::cout << "\n--- 移动构造 ---\n";
    Buffer b5 = std::move(b4);

    std::cout << "\n离开 main, 析构顺序:\n";
    return 0;
}
