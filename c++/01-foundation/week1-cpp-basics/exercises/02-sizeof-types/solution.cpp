#include <iostream>
#include <cstdint>
#include <limits>

int main() {
    // 内置类型 sizeof
    std::cout << "bool:           " << sizeof(bool) << " 字节\n";
    std::cout << "char:           " << sizeof(char) << " 字节\n";
    std::cout << "short:          " << sizeof(short) << " 字节\n";
    std::cout << "int:            " << sizeof(int) << " 字节\n";
    std::cout << "long:           " << sizeof(long) << " 字节\n";
    std::cout << "long long:      " << sizeof(long long) << " 字节\n";
    std::cout << "float:          " << sizeof(float) << " 字节\n";
    std::cout << "double:         " << sizeof(double) << " 字节\n";
    std::cout << "long double:    " << sizeof(long double) << " 字节\n";
    std::cout << "指针 (void*):   " << sizeof(void*) << " 字节\n";

    // 定长类型 sizeof
    std::cout << '\n';
    std::cout << "int8_t:         " << sizeof(int8_t) << " 字节\n";
    std::cout << "uint8_t:        " << sizeof(uint8_t) << " 字节\n";
    std::cout << "int16_t:        " << sizeof(int16_t) << " 字节\n";
    std::cout << "uint16_t:       " << sizeof(uint16_t) << " 字节\n";
    std::cout << "int32_t:        " << sizeof(int32_t) << " 字节\n";
    std::cout << "uint32_t:       " << sizeof(uint32_t) << " 字节\n";
    std::cout << "int64_t:        " << sizeof(int64_t) << " 字节\n";
    std::cout << "uint64_t:       " << sizeof(uint64_t) << " 字节\n";
    std::cout << "size_t:         " << sizeof(size_t) << " 字节\n";
    std::cout << "ptrdiff_t:      " << sizeof(ptrdiff_t) << " 字节\n";

    // 挑战：取值范围
    std::cout << '\n';
    std::cout << "int 最大值:     " << std::numeric_limits<int>::max() << '\n';
    std::cout << "int 最小值:     " << std::numeric_limits<int>::min() << '\n';
    std::cout << "uint 最大值:    " << std::numeric_limits<unsigned int>::max() << '\n';
    std::cout << "uint 最小值:    " << std::numeric_limits<unsigned int>::min() << '\n';

    return 0;
}
