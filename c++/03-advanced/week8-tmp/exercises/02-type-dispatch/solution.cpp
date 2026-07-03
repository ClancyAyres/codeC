// Exercise 02 Solution: TypeDispatch — 编译时分发处理函数

#include <iostream>
#include <type_traits>
#include <string>

template<typename T>
struct Traits {
    static constexpr bool isIntegral = std::is_integral_v<T>;
    static constexpr bool isFloating = std::is_floating_point_v<T>;
    static constexpr bool isPointer  = std::is_pointer_v<T>;
    static constexpr bool isString   = std::is_same_v<T, std::string>;
};

template<typename T>
struct TypeDispatch {
    static void describe() {
        std::cout << "Type: ";
        if constexpr (Traits<T>::isIntegral) {
            std::cout << "integral";
        } else if constexpr (Traits<T>::isFloating) {
            std::cout << "floating-point";
        } else if constexpr (Traits<T>::isPointer) {
            std::cout << "pointer";
        } else if constexpr (Traits<T>::isString) {
            std::cout << "std::string";
        } else {
            std::cout << "unknown";
        }
        std::cout << '\n';
    }
};

int main() {
    std::cout << "=== TypeDispatch ===\n";
    TypeDispatch<int>::describe();
    TypeDispatch<double>::describe();
    TypeDispatch<int*>::describe();
    TypeDispatch<std::string>::describe();
    TypeDispatch<char>::describe();
    TypeDispatch<float>::describe();
    TypeDispatch<long>::describe();

    std::cout << "\n=== Traits ===\n";
    std::cout << "int:\n";
    std::cout << "  isIntegral:  " << Traits<int>::isIntegral << '\n';
    std::cout << "  isFloating:  " << Traits<int>::isFloating << '\n';
    std::cout << "  isPointer:   " << Traits<int>::isPointer << '\n';
    std::cout << "  isString:    " << Traits<int>::isString << '\n';

    std::cout << "double:\n";
    std::cout << "  isIntegral:  " << Traits<double>::isIntegral << '\n';
    std::cout << "  isFloating:  " << Traits<double>::isFloating << '\n';
    std::cout << "  isPointer:   " << Traits<double>::isPointer << '\n';
    std::cout << "  isString:    " << Traits<double>::isString << '\n';

    std::cout << "std::string:\n";
    std::cout << "  isIntegral:  " << Traits<std::string>::isIntegral << '\n';
    std::cout << "  isFloating:  " << Traits<std::string>::isFloating << '\n';
    std::cout << "  isPointer:   " << Traits<std::string>::isPointer << '\n';
    std::cout << "  isString:    " << Traits<std::string>::isString << '\n';

    return 0;
}
