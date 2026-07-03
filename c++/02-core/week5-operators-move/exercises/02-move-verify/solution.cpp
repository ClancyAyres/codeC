#include <cstring>
#include <iostream>
#include <utility>
#include <vector>

class Buffer {
    int* data_ = nullptr;
    size_t size_ = 0;

public:
    explicit Buffer(size_t n) : data_(new int[n]), size_(n) {
        std::cout << "  (constructed with size " << size_ << ")\n";
    }

    ~Buffer() {
        delete[] data_;
    }

    Buffer(const Buffer& other)
        : data_(new int[other.size_])
        , size_(other.size_) {
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "  (copy constructor — expensive!)\n";
    }

    Buffer(Buffer&& other) noexcept
        : data_(std::exchange(other.data_, nullptr))
        , size_(std::exchange(other.size_, 0)) {
        std::cout << "  (move constructor — cheap!)\n";
    }

    Buffer& operator=(const Buffer& other) {
        std::cout << "  (copy assignment — expensive!)\n";
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new int[size_];
            std::copy(other.data_, other.data_ + size_, data_);
        }
        return *this;
    }

    Buffer& operator=(Buffer&& other) noexcept {
        std::cout << "  (move assignment — cheap!)\n";
        if (this != &other) {
            delete[] data_;
            data_ = std::exchange(other.data_, nullptr);
            size_ = std::exchange(other.size_, 0);
        }
        return *this;
    }

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
};

int main() {
    std::cout << "=== Copy Test ===\n";
    {
        Buffer b1(1000);
        std::vector<Buffer> v;
        v.push_back(b1);
        v.push_back(b1);
        v.push_back(b1);
        std::cout << "  vector size: " << v.size() << '\n';
    }

    std::cout << "\n=== Move Test ===\n";
    {
        std::vector<Buffer> v;
        v.push_back(Buffer(1000));
        v.push_back(Buffer(1000));
        v.push_back(Buffer(1000));
        std::cout << "  vector size: " << v.size() << '\n';
    }

    std::cout << "\n=== Move Assignment ===\n";
    {
        Buffer buf1(500);
        Buffer buf2(1000);
        buf1 = std::move(buf2);
        std::cout << "  buf1 size: " << buf1.size() << '\n';
        std::cout << "  buf2 is empty: " << (buf2.empty() ? "true" : "false") << '\n';
    }

    std::cout << "\n=== Self Assignment Safety ===\n";
    {
        Buffer buf(100);
        buf = buf;
        std::cout << "  survived self-assignment\n";
    }

    return 0;
}
