#include <iostream>
#include <utility>

class UniqueResource {
    int* data_ = nullptr;
    size_t size_ = 0;

public:
    explicit UniqueResource(size_t n)
        : data_(new int[n]{}), size_(n) {
        std::cout << "  Created with size " << size_ << '\n';
    }

    ~UniqueResource() {
        delete[] data_;
    }

    UniqueResource(const UniqueResource&) = delete;
    UniqueResource& operator=(const UniqueResource&) = delete;

    UniqueResource(UniqueResource&& other) noexcept
        : data_(std::exchange(other.data_, nullptr))
        , size_(std::exchange(other.size_, 0)) {
        std::cout << "  (moved)\n";
    }

    UniqueResource& operator=(UniqueResource&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = std::exchange(other.data_, nullptr);
            size_ = std::exchange(other.size_, 0);
        }
        std::cout << "  (move-assigned)\n";
        return *this;
    }

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    int& operator[](size_t index) {
        return data_[index];
    }

    const int& operator[](size_t index) const {
        return data_[index];
    }

    void swap(UniqueResource& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    int* release() noexcept {
        size_ = 0;
        return std::exchange(data_, nullptr);
    }
};

int main() {
    std::cout << "=== Create ===\n";
    UniqueResource res1(5);
    for (size_t i = 0; i < res1.size(); ++i) {
        res1[i] = static_cast<int>((i + 1) * 10);
    }

    std::cout << "\n=== Access via operator[] ===\n";
    std::cout << "  res1[0] = " << res1[0] << '\n';
    std::cout << "  res1[4] = " << res1[4] << '\n';

    std::cout << "\n=== Move Constructor ===\n";
    UniqueResource res2 = std::move(res1);
    std::cout << "  res1 is empty: " << (res1.empty() ? "true" : "false") << '\n';
    std::cout << "  res2 size: " << res2.size() << '\n';

    std::cout << "\n=== Move Assignment ===\n";
    UniqueResource res3(1);
    res3 = std::move(res2);
    std::cout << "  res2 is empty: " << (res2.empty() ? "true" : "false") << '\n';
    std::cout << "  res3 size: " << res3.size() << '\n';

    std::cout << "\n=== Swap ===\n";
    UniqueResource res4(3);
    for (size_t i = 0; i < res4.size(); ++i) res4[i] = static_cast<int>(i + 100);
    std::cout << "  Before: res3 size=" << res3.size() << ", res4 size=" << res4.size() << '\n';
    res3.swap(res4);
    std::cout << "  After:  res3 size=" << res3.size() << ", res4 size=" << res4.size() << '\n';

    std::cout << "\n=== Release ===\n";
    int* raw = res3.release();
    std::cout << "  res3 is empty: " << (res3.empty() ? "true" : "false") << '\n';
    std::cout << "  raw[0] = " << raw[0] << ", raw[2] = " << raw[2] << '\n';
    delete[] raw;

    return 0;
}
