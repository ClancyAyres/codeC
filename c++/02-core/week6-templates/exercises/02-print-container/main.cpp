#include <iostream>
#include <vector>
#include <list>
#include <string>

// TODO: 实现 printContainer 模板函数
template<typename Container>
void printContainer(const Container& c, const char* separator = " ") {
    // TODO: 使用 std::begin/end 遍历容器（支持 C 数组和 STL 容器）
    // 提示：用 bool first = true 控制分隔符
}

int main() {
    std::cout << "=== vector<int> ===\n";
    std::vector<int> v = {1, 2, 3, 4, 5};
    printContainer(v);
    std::cout << '\n';
    printContainer(v, ", ");
    std::cout << '\n';

    std::cout << "\n=== vector<string> ===\n";
    std::vector<std::string> sv = {"hello", "world", "C++", "templates"};
    printContainer(sv);
    std::cout << '\n';

    std::cout << "\n=== list<double> ===\n";
    std::list<double> l = {3.14, 2.72, 1.41};
    printContainer(l);
    std::cout << '\n';

    std::cout << "\n=== Custom Array ===\n";
    int arr[] = {10, 20, 30, 40};
    // C++ 数组也可以用 begin/end 迭代
    printContainer(arr);
    std::cout << '\n';

    return 0;
}
