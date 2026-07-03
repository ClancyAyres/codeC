#include <iostream>
#include <cstring>

// TODO: 实现 my_strlen — 用指针算术计算字符串长度
size_t my_strlen(const char* str) {
    // TODO: 使用指针算术，不使用 []
    // 提示：创建指针 p = str，while (*p) ++p，返回 p - str
    return 0;  // 占位
}

// TODO: 实现 my_strcpy — 用指针算术复制字符串
char* my_strcpy(char* dest, const char* src) {
    // TODO: 使用指针算术，不使用 []
    // 提示：保存 dest 原始值用于返回，逐个复制包括 '\0'
    return dest;  // 占位
}

// TODO: 实现 my_strcmp — 用指针算术比较字符串
int my_strcmp(const char* s1, const char* s2) {
    // TODO: 使用指针算术，不使用 []
    // 提示：while (*s1 && *s1 == *s2) { ++s1; ++s2; }
    //       返回 *s1 - *s2
    return 0;  // 占位
}

int main() {
    const char* hello = "Hello, World!";
    const char* empty = "";

    // 测试 my_strlen
    std::cout << "my_strlen(\"" << hello << "\") = " << my_strlen(hello) << '\n';
    std::cout << "my_strlen(\"" << empty << "\") = " << my_strlen(empty) << '\n';
    std::cout << "Expected: " << std::strlen(hello) << " and 0\n\n";

    // 测试 my_strcpy
    char buffer[100] = {};
    char* result = my_strcpy(buffer, hello);
    std::cout << "my_strcpy result: \"" << buffer << "\"\n";
    std::cout << "Returned pointer == buffer? " << (result == buffer ? "yes" : "no") << '\n';
    std::cout << "Expected: \"" << hello << "\" and yes\n\n";

    // 测试 my_strcmp
    std::cout << "my_strcmp(\"abc\", \"abc\") = " << my_strcmp("abc", "abc") << " (expect 0)\n";
    std::cout << "my_strcmp(\"abc\", \"abd\") = " << my_strcmp("abc", "abd") << " (expect negative)\n";
    std::cout << "my_strcmp(\"abd\", \"abc\") = " << my_strcmp("abd", "abc") << " (expect positive)\n";
    std::cout << "my_strcmp(\"a\",   \"ab\")  = " << my_strcmp("a", "ab") << " (expect negative)\n";
    std::cout << "my_strcmp(\"\",    \"\")    = " << my_strcmp("", "") << " (expect 0)\n";

    return 0;
}
