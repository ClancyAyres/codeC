#include <algorithm>
#include <iostream>
#include <utility>

class Buffer {
    size_t size_;
    int* data_;

public:
    explicit Buffer(size_t size)
        : size_(size), data_(new int[size]{}) {
        std::cout << "[ctor] Buffer(size=" << size_ << ") constructed\n";
    }

    ~Buffer() {
        std::cout << "[dtor] Buffer(size=" << size_ << ") destroyed\n";
        delete[] data_;
    }

    Buffer(const Buffer& other)
        : size_(other.size_), data_(new int[other.size_]) {
        std::cout << "[copy-ctor] Buffer(size=" << size_ << ") copied\n";
        std::copy(other.data_, other.data_ + size_, data_);
    }

    Buffer& operator=(const Buffer& other) {
        std::cout << "[copy-assign] Buffer(size=" << size_
                  << ") <- Buffer(size=" << other.size_ << ")\n";
        if (this == &other) return *this;

        int* newData = new int[other.size_];
        std::copy(other.data_, other.data_ + other.size_, newData);

        delete[] data_;
        data_ = newData;
        size_ = other.size_;
        return *this;
    }

    Buffer(Buffer&& other) noexcept
        : size_(std::exchange(other.size_, 0)),
          data_(std::exchange(other.data_, nullptr)) {
        std::cout << "[move-ctor] Buffer(size=" << size_ << ") moved\n";
    }

    Buffer& operator=(Buffer&& other) noexcept {
        std::cout << "[move-assign] Buffer(size=" << size_
                  << ") <- Buffer(size=" << other.size_ << ")\n";
        if (this == &other) return *this;

        delete[] data_;
        data_ = std::exchange(other.data_, nullptr);
        size_ = std::exchange(other.size_, 0);
        return *this;
    }

    size_t size() const { return size_; }
    int* data() const { return data_; }
    int& operator[](size_t i) { return data_[i]; }
    const int& operator[](size_t i) const { return data_[i]; }

    void fill(int value) {
        std::fill(data_, data_ + size_, value);
    }

    void print() const {
        std::cout << "  [";
        for (size_t i = 0; i < size_; ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << data_[i];
        }
        std::cout << "]\n";
    }
};

Buffer makeBuffer(size_t size, int value) {
    Buffer b(size);
    b.fill(value);
    return b;
}

int main() {
    std::cout << "--- 1. Constructor ---\n";
    Buffer b1(5);
    b1.fill(10);
    b1.print();

    std::cout << "\n--- 2. Copy constructor ---\n";
    Buffer b2 = b1;
    b2[0] = 99;
    std::cout << "  b1: ";
    b1.print();
    std::cout << "  b2: ";
    b2.print();

    std::cout << "\n--- 3. Copy assignment ---\n";
    Buffer b3(3);
    b3.fill(7);
    std::cout << "  b3 before: ";
    b3.print();
    b3 = b1;
    std::cout << "  b3 after:  ";
    b3.print();

    std::cout << "\n--- 4. Move constructor ---\n";
    Buffer b4 = std::move(b3);
    std::cout << "  b4 (moved-to): size=" << b4.size() << "\n";
    std::cout << "  b3 (moved-from): size=" << b3.size() << "\n";

    std::cout << "\n--- 5. Move assignment ---\n";
    Buffer b5(2);
    b5.fill(1);
    std::cout << "  b5 before: ";
    b5.print();
    b5 = std::move(b4);
    std::cout << "  b5 after:  ";
    b5.print();
    std::cout << "  b4 (moved-from): size=" << b4.size() << "\n";

    std::cout << "\n--- 6. RVO / move from factory ---\n";
    Buffer b6 = makeBuffer(4, 42);
    b6.print();

    std::cout << "\n--- 7. Self-assignment (safe) ---\n";
    b1 = b1;
    std::cout << "  b1 after self-assign: ";
    b1.print();

    std::cout << "\nAll done.\n";
    return 0;
}
