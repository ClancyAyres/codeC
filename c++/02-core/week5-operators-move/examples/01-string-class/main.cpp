// Example 01: String Class — 完整的 String 类展示运算符重载和移动语义
// 演示：构造/析构、拷贝/移动、operator==、operator<<、operator+

#include <cstring>
#include <iostream>
#include <utility>

class String {
    char* data_ = nullptr;
    size_t size_ = 0;

public:
    // 默认构造
    String() = default;

    // 从 C 字符串构造
    String(const char* s) {
        size_ = std::strlen(s);
        data_ = new char[size_ + 1];
        std::strcpy(data_, s);
    }

    // 拷贝构造 — 深拷贝
    String(const String& other) {
        size_ = other.size_;
        data_ = new char[size_ + 1];
        std::strcpy(data_, other.data_);
    }

    // 移动构造 — 转移所有权
    String(String&& other) noexcept
        : data_(std::exchange(other.data_, nullptr))
        , size_(std::exchange(other.size_, 0)) {
    }

    // 拷贝赋值
    String& operator=(const String& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new char[size_ + 1];
            std::strcpy(data_, other.data_);
        }
        return *this;
    }

    // 移动赋值
    String& operator=(String&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = std::exchange(other.data_, nullptr);
            size_ = std::exchange(other.size_, 0);
        }
        return *this;
    }

    ~String() { delete[] data_; }

    // operator== 比较
    bool operator==(const String& other) const {
        return size_ == other.size_ &&
               std::strcmp(data_, other.data_) == 0;
    }

    bool operator!=(const String& other) const {
        return !(*this == other);
    }

    // operator+ 拼接
    String operator+(const String& other) const {
        String result;
        result.size_ = size_ + other.size_;
        result.data_ = new char[result.size_ + 1];
        if (data_) std::strcpy(result.data_, data_); else result.data_[0] = '\0';
        if (other.data_) std::strcat(result.data_, other.data_);
        return result;
    }

    // operator<< 流输出（友元）
    friend std::ostream& operator<<(std::ostream& os, const String& s) {
        return os << (s.data_ ? s.data_ : "");
    }

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
};

int main() {
    std::cout << "=== Construction ===\n";
    String s1("Hello");
    String s2(" World");
    std::cout << "s1: " << s1 << " (size=" << s1.size() << ")\n";
    std::cout << "s2: " << s2 << " (size=" << s2.size() << ")\n";

    std::cout << "\n=== Copy ===\n";
    String s3 = s1;  // 拷贝构造
    std::cout << "s3 (copy of s1): " << s3 << '\n';
    std::cout << "s1 is unchanged: " << s1 << '\n';

    std::cout << "\n=== Move ===\n";
    String s4 = std::move(s3);  // 移动构造
    std::cout << "s4 (moved from s3): " << s4 << '\n';
    std::cout << "s3 after move: \"" << s3 << "\" (size=" << s3.size() << ")\n";

    std::cout << "\n=== operator+ ===\n";
    String s5 = s1 + s2;
    std::cout << "s1 + s2 = " << s5 << " (size=" << s5.size() << ")\n";

    std::cout << "\n=== operator== ===\n";
    String s6("Hello");
    std::cout << "s1 == s6: " << (s1 == s6 ? "true" : "false") << '\n';
    std::cout << "s1 == s2: " << (s1 == s2 ? "true" : "false") << '\n';
    std::cout << "s1 != s2: " << (s1 != s2 ? "true" : "false") << '\n';

    std::cout << "\n=== Move Assignment ===\n";
    s1 = String("NewValue");
    std::cout << "s1 after move assignment: " << s1 << '\n';

    std::cout << "\n=== Self Assignment Safety ===\n";
    s1 = s1;  // 自赋值安全
    std::cout << "s1 after self-assignment: " << s1 << '\n';

    return 0;
}
