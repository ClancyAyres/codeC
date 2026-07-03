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

    // TODO: 实现拷贝构造函数 — 深拷贝
    StringArray(const StringArray& other)
        : size_(other.size_), data_(new std::string[other.size_]) {
        // TODO: 输出 "[copy-ctor] StringArray(size=N) copied"
        // TODO: 拷贝元素: std::copy(other.data_, other.data_ + size_, data_)
    }

    // TODO: 实现拷贝赋值运算符 — 深拷贝，处理自赋值
    StringArray& operator=(const StringArray& other) {
        // TODO: 输出 "[copy-assign] StringArray(size=N) <- StringArray(size=M)"
        // TODO: 自赋值检查
        // TODO: 分配新数组，拷贝，释放旧数组，更新 size_
        return *this;
    }

    // TODO: 实现移动构造函数 — 接管资源
    StringArray(StringArray&& other) noexcept
        : size_(0), data_(nullptr) {
        // TODO: 输出 "[move-ctor] StringArray(size=N) moved"
        // TODO: 接管 other 的 size_ 和 data_
        // TODO: 重置 other (data_ = nullptr, size_ = 0)
    }

    // TODO: 实现移动赋值运算符 — 释放旧资源，接管新资源
    StringArray& operator=(StringArray&& other) noexcept {
        // TODO: 输出 "[move-assign] StringArray(size=N) <- StringArray(size=M)"
        // TODO: 自赋值检查
        // TODO: 释放旧 data_
        // TODO: 接管 other 的资源
        // TODO: 重置 other
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
