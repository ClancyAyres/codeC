#include <algorithm>
#include <iostream>
#include <string>
#include <utility>

class StringArray {
    size_t size_;
    std::string* data_;

public:
    explicit StringArray(size_t size)
        : size_(size), data_(new std::string[size]) {
        std::cout << "[ctor] StringArray(size=" << size_ << ") constructed\n";
    }

    ~StringArray() {
        std::cout << "[dtor] StringArray(size=" << size_ << ") destroyed\n";
        delete[] data_;
    }

    StringArray(const StringArray& other)
        : size_(other.size_), data_(new std::string[other.size_]) {
        std::cout << "[copy-ctor] StringArray(size=" << size_ << ") copied\n";
        std::copy(other.data_, other.data_ + size_, data_);
    }

    StringArray& operator=(const StringArray& other) {
        std::cout << "[copy-assign] StringArray(size=" << size_
                  << ") <- StringArray(size=" << other.size_ << ")\n";
        if (this == &other) return *this;

        std::string* newData = new std::string[other.size_];
        std::copy(other.data_, other.data_ + other.size_, newData);

        delete[] data_;
        data_ = newData;
        size_ = other.size_;
        return *this;
    }

    StringArray(StringArray&& other) noexcept
        : size_(std::exchange(other.size_, 0)),
          data_(std::exchange(other.data_, nullptr)) {
        std::cout << "[move-ctor] StringArray(size=" << size_ << ") moved\n";
    }

    StringArray& operator=(StringArray&& other) noexcept {
        std::cout << "[move-assign] StringArray(size=" << size_
                  << ") <- StringArray(size=" << other.size_ << ")\n";
        if (this == &other) return *this;

        delete[] data_;
        data_ = std::exchange(other.data_, nullptr);
        size_ = std::exchange(other.size_, 0);
        return *this;
    }

    size_t size() const { return size_; }

    std::string& operator[](size_t i) { return data_[i]; }
    const std::string& operator[](size_t i) const { return data_[i]; }
};

int main() {
    std::cout << "--- Test 1: Constructor ---\n";
    StringArray a(3);
    a[0] = "hello";
    a[1] = "world";
    a[2] = "C++";
    std::cout << "  a[0] = " << a[0] << '\n';
    std::cout << "  a[1] = " << a[1] << '\n';
    std::cout << "  a[2] = " << a[2] << "\n\n";

    std::cout << "--- Test 2: Copy constructor ---\n";
    StringArray b = a;
    b[0] = "modified";
    std::cout << "  a[0] = " << a[0] << " (expect hello, deep copy)\n";
    std::cout << "  b[0] = " << b[0] << " (expect modified)\n\n";

    std::cout << "--- Test 3: Copy assignment ---\n";
    StringArray c(4);
    c[0] = "old";
    std::cout << "  c.size() before = " << c.size() << " (expect 4)\n";
    c = a;
    std::cout << "  c.size() after  = " << c.size() << " (expect 3)\n";
    std::cout << "  c[0] = " << c[0] << " (expect hello)\n\n";

    std::cout << "--- Test 4: Move constructor ---\n";
    StringArray d = std::move(c);
    std::cout << "  d.size() = " << d.size() << " (expect 3)\n";
    std::cout << "  c.size() = " << c.size() << " (expect 0, moved-from)\n\n";

    std::cout << "--- Test 5: Move assignment ---\n";
    StringArray e(1);
    e[0] = "temp";
    std::cout << "  e.size() before = " << e.size() << " (expect 1)\n";
    e = std::move(d);
    std::cout << "  e.size() after  = " << e.size() << " (expect 3)\n";
    std::cout << "  d.size() = " << d.size() << " (expect 0, moved-from)\n\n";

    std::cout << "--- Test 6: Self-assignment ---\n";
    a = a;
    std::cout << "  Self-assignment handled correctly\n\n";

    std::cout << "--- End of main — destructors run below ---\n";
    return 0;
}
