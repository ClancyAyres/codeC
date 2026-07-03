// 模块 4 习题: StringWrapper — 三五法则完整实现
// 请在标注 TODO 的位置实现

#include <iostream>
#include <string>
#include <cstring>
#include <utility>

class StringWrapper {
public:
    // TODO: 构造函数
    StringWrapper(const char* str) {
        // ...
    }

    explicit StringWrapper(const std::string& str) : StringWrapper(str.c_str()) {}

    // TODO: 拷贝构造 (三五法则)
    // TODO: 拷贝赋值
    // TODO: 移动构造
    // TODO: 移动赋值
    // TODO: 析构

    // TODO: 拼接 operator+
    // TODO: 比较 operator==
    // TODO: 输出 operator<<

    size_t length() const { /* ... */ }
    const char* c_str() const { /* ... */ }

private:
    char* data_ = nullptr;
    size_t size_ = 0;
};

int main() {
    // TODO: 测试拷贝、移动、拼接、比较
    return 0;
}
