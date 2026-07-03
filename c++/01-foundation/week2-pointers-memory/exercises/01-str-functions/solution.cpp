#include <iostream>
#include <cstring>

size_t my_strlen(const char* str) {
    const char* p = str;
    while (*p) {
        ++p;
    }
    return p - str;
}

char* my_strcpy(char* dest, const char* src) {
    char* original = dest;
    while (*src) {
        *dest = *src;
        ++dest;
        ++src;
    }
    *dest = '\0';
    return original;
}

int my_strcmp(const char* s1, const char* s2) {
    while (*s1 && *s1 == *s2) {
        ++s1;
        ++s2;
    }
    return static_cast<int>(*s1) - static_cast<int>(*s2);
}

int main() {
    const char* hello = "Hello, World!";
    const char* empty = "";

    std::cout << "my_strlen(\"" << hello << "\") = " << my_strlen(hello) << '\n';
    std::cout << "my_strlen(\"" << empty << "\") = " << my_strlen(empty) << '\n';
    std::cout << "Expected: " << std::strlen(hello) << " and 0\n\n";

    char buffer[100] = {};
    char* result = my_strcpy(buffer, hello);
    std::cout << "my_strcpy result: \"" << buffer << "\"\n";
    std::cout << "Returned pointer == buffer? " << (result == buffer ? "yes" : "no") << '\n';
    std::cout << "Expected: \"" << hello << "\" and yes\n\n";

    std::cout << "my_strcmp(\"abc\", \"abc\") = " << my_strcmp("abc", "abc") << " (expect 0)\n";
    std::cout << "my_strcmp(\"abc\", \"abd\") = " << my_strcmp("abc", "abd") << " (expect negative)\n";
    std::cout << "my_strcmp(\"abd\", \"abc\") = " << my_strcmp("abd", "abc") << " (expect positive)\n";
    std::cout << "my_strcmp(\"a\",   \"ab\")  = " << my_strcmp("a", "ab") << " (expect negative)\n";
    std::cout << "my_strcmp(\"\",    \"\")    = " << my_strcmp("", "") << " (expect 0)\n";

    return 0;
}
