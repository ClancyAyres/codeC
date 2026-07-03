// Exercise 02: TypeDispatch — 编译时分发处理函数
// TODO: 实现 TypeDispatch<T> 和 Traits<T>

#include <iostream>
#include <type_traits>
#include <string>

// TODO: 实现 Traits<T>
template<typename T>
struct Traits {
    // TODO: static constexpr bool isIntegral = ...
    // TODO: static constexpr bool isFloating = ...
    // TODO: static constexpr bool isPointer = ...
    // TODO: static constexpr bool isString = ...
};

// TODO: 实现 TypeDispatch<T>::describe()
template<typename T>
struct TypeDispatch {
    static void describe() {
        std::cout << "Type: ";
        // TODO: if constexpr (Traits<T>::isIntegral) → "integral"
        // TODO: else if constexpr (Traits<T>::isFloating) → "floating-point"
        // TODO: else if constexpr (Traits<T>::isPointer) → "pointer"
        // TODO: else if constexpr (Traits<T>::isString) → "std::string"
        // TODO: else → "unknown"
        // TODO: std::cout << '\n';
    }
};

int main() {
    std::cout << "=== TypeDispatch ===\n";
    // TODO: uncomment
    // TypeDispatch<int>::describe();
    // TypeDispatch<double>::describe();
    // TypeDispatch<int*>::describe();
    // TypeDispatch<std::string>::describe();
    // TypeDispatch<char>::describe();
    // TypeDispatch<float>::describe();
    // TypeDispatch<long>::describe();

    std::cout << "\n=== Traits ===\n";
    // TODO: uncomment
    // std::cout << "int:\n";
    // std::cout << "  isIntegral:  " << Traits<int>::isIntegral << '\n';
    // std::cout << "  isFloating:  " << Traits<int>::isFloating << '\n';
    // std::cout << "  isPointer:   " << Traits<int>::isPointer << '\n';
    // std::cout << "  isString:    " << Traits<int>::isString << '\n';

    // std::cout << "double:\n";
    // std::cout << "  isIntegral:  " << Traits<double>::isIntegral << '\n';
    // std::cout << "  isFloating:  " << Traits<double>::isFloating << '\n';
    // std::cout << "  isPointer:   " << Traits<double>::isPointer << '\n';
    // std::cout << "  isString:    " << Traits<double>::isString << '\n';

    // std::cout << "std::string:\n";
    // std::cout << "  isIntegral:  " << Traits<std::string>::isIntegral << '\n';
    // std::cout << "  isFloating:  " << Traits<std::string>::isFloating << '\n';
    // std::cout << "  isPointer:   " << Traits<std::string>::isPointer << '\n';
    // std::cout << "  isString:    " << Traits<std::string>::isString << '\n';

    return 0;
}
