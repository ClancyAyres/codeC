#include <iostream>
#include <cstdint>
#include <climits>
#include <cfloat>
#include <limits>
#include <iomanip>

int main() {
    std::cout << "========== 内置类型 sizeof ==========\n";
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

    std::cout << "\n========== 定长类型 (cstdint) sizeof ==========\n";
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

    std::cout << "\n========== 取值范围 (numeric_limits) ==========\n";
    std::cout << std::boolalpha;  // 让 bool 输出 true/false 而非 1/0

    std::cout << "int 最小值:     " << std::numeric_limits<int>::min() << '\n';
    std::cout << "int 最大值:     " << std::numeric_limits<int>::max() << '\n';
    std::cout << "int 是否有符号: " << std::numeric_limits<int>::is_signed << '\n';

    std::cout << "unsigned int 最大值: " << std::numeric_limits<unsigned int>::max() << '\n';

    std::cout << "double 最小值:  " << std::numeric_limits<double>::min() << '\n';
    std::cout << "double 最大值:  " << std::numeric_limits<double>::max() << '\n';
    std::cout << "double 精度:    " << std::numeric_limits<double>::epsilon() << '\n';

    // 也可以使用 C 风格宏
    std::cout << "\n========== C 风格宏 ==========\n";
    std::cout << "INT_MAX:        " << INT_MAX << '\n';
    std::cout << "INT_MIN:        " << INT_MIN << '\n';
    std::cout << "UINT_MAX:       " << UINT_MAX << '\n';
    std::cout << "LONG_MAX:       " << LONG_MAX << '\n';
    std::cout << "FLT_MAX:        " << FLT_MAX << '\n';

    std::cout << "\n========== 溢出演示 ==========\n";
    // 有符号整数溢出：未定义行为！
    // 下面代码在现代编译器开启优化时可能产生意想不到的结果
    int maxInt = std::numeric_limits<int>::max();
    std::cout << "int 最大值 + 1 (有符号溢出, UB): " << maxInt + 1 << "  <-- 不可依赖\n";

    // 无符号整数溢出：定义行为，回绕到 0
    unsigned int maxUInt = std::numeric_limits<unsigned int>::max();
    std::cout << "unsigned int 最大值 + 1 (无符号, 定义行为): " << maxUInt + 1 << '\n';

    // 0u - 1：无符号下溢，回绕到最大值
    unsigned int zero = 0u;
    std::cout << "0u - 1 (无符号下溢): " << zero - 1 << '\n';

    // 窄化转换：大类型赋值给小类型
    int big = 300;
    int8_t small = static_cast<int8_t>(big);  // 300 超出 int8_t 范围 [-128, 127]
    std::cout << "int8_t(300) 结果: " << static_cast<int>(small)
              << "  <-- 截断, 不可依赖\n";

    return 0;
}
