// 模块 6 示例: 函数模板
// 相关规范: Core Guidelines T.1, T.3

#include <iostream>
#include <string>
#include <vector>

// ✅ 函数模板: 替代 Java 泛型方法
template<typename T>
T maxOf(T a, T b) {
    return (a > b) ? a : b;
}

// ✅ 多类型参数模板
template<typename T, typename U>
auto multiply(T a, U b) -> decltype(a * b) {
    return a * b;
}

// C++14: 返回类型自动推导
template<typename T, typename U>
auto multiply14(T a, U b) {
    return a * b;
}

// ✅ 模板 + 引用: 避免拷贝
template<typename T>
void printElements(const std::vector<T>& vec) {
    std::cout << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << vec[i];
    }
    std::cout << "]\n";
}

int main() {
    std::cout << maxOf(3, 7) << '\n';          // int
    std::cout << maxOf(3.14, 2.71) << '\n';     // double
    std::cout << maxOf(std::string("abc"), std::string("xyz")) << '\n';  // string

    std::cout << multiply(3, 2.5) << '\n';       // int * double → double
    std::cout << multiply14(3, 2.5) << '\n';

    std::vector<int> nums = {1, 2, 3};
    printElements(nums);

    std::vector<std::string> strs = {"hello", "world"};
    printElements(strs);

    return 0;
}
