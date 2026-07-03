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

    // TODO: 实现拷贝构造 — 深拷贝数据，打印 "(copy constructor — expensive!)"
    Buffer(const Buffer& other) {
        // TODO
    }

    // TODO: 实现移动构造 — 转移所有权，打印 "(move constructor — cheap!)"
    // 使用 std::exchange
    Buffer(Buffer&& other) noexcept {
        // TODO
    }

    // TODO: 实现拷贝赋值 — 注意自赋值保护
    Buffer& operator=(const Buffer& other) {
        // TODO
        return *this;
    }

    // TODO: 实现移动赋值 — 注意自赋值保护
    Buffer& operator=(Buffer&& other) noexcept {
        // TODO
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
        v.push_back(b1);           // 拷贝
        v.push_back(b1);           // 拷贝 + 扩容时拷贝
        v.push_back(b1);           // 拷贝 + 扩容时拷贝
        std::cout << "  vector size: " << v.size() << '\n';
    }

    std::cout << "\n=== Move Test ===\n";
    {
        std::vector<Buffer> v;
        v.push_back(Buffer(1000));  // 移动临时对象
        v.push_back(Buffer(1000));  // 移动 + 扩容时移动
        v.push_back(Buffer(1000));  // 移动 + 扩容时移动
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
        buf = buf;  // 自赋值安全
        std::cout << "  survived self-assignment\n";
    }

    return 0;
}
