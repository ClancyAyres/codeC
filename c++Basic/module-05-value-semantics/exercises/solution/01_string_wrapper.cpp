// 模块 4 习题答案: StringWrapper — 三五法则完整实现
// 规范: Core Guidelines C.21, C.80, C.160, C.161

#include <iostream>
#include <string>
#include <cstring>
#include <utility>

class StringWrapper {
public:
    StringWrapper(const char* str) : size_(std::strlen(str)), data_(new char[size_ + 1]) {
        std::strcpy(data_, str);
    }

    explicit StringWrapper(const std::string& str) : StringWrapper(str.c_str()) {}

    StringWrapper(const StringWrapper& other)
        : size_(other.size_), data_(new char[size_ + 1]) {
        std::strcpy(data_, other.data_);
    }

    StringWrapper& operator=(const StringWrapper& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new char[size_ + 1];
            std::strcpy(data_, other.data_);
        }
        return *this;
    }

    StringWrapper(StringWrapper&& other) noexcept
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    StringWrapper& operator=(StringWrapper&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    ~StringWrapper() { delete[] data_; }

    StringWrapper operator+(const StringWrapper& rhs) const {
        StringWrapper result;
        result.size_ = size_ + rhs.size_;
        result.data_ = new char[result.size_ + 1];
        std::strcpy(result.data_, data_);
        std::strcat(result.data_, rhs.data_);
        return result;
    }

    bool operator==(const StringWrapper& rhs) const {
        return std::strcmp(data_, rhs.data_) == 0;
    }

    bool operator!=(const StringWrapper& rhs) const {
        return !(*this == rhs);
    }

    size_t length() const { return size_; }
    const char* c_str() const { return data_; }

private:
    StringWrapper() = default;
    char* data_ = nullptr;
    size_t size_ = 0;
};

std::ostream& operator<<(std::ostream& os, const StringWrapper& sw) {
    return os << sw.c_str();
}

int main() {
    StringWrapper s1("Hello");
    StringWrapper s2 = s1;     // 拷贝
    StringWrapper s3("World");

    StringWrapper s4 = std::move(s3);  // 移动
    StringWrapper s5 = s1 + s2;         // 拼接

    std::cout << "s1: " << s1 << " (len=" << s1.length() << ")\n";
    std::cout << "s2: " << s2 << '\n';
    std::cout << "s4: " << s4 << '\n';
    std::cout << "s5: " << s5 << '\n';
    std::cout << "s3 length after move: " << s3.length() << '\n';
    std::cout << "s1 == s2: " << (s1 == s2) << '\n';

    return 0;
}
