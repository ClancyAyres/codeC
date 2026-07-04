// 模块 7 示例: Lambda 表达式
// 相关规范: Core Guidelines ES.28

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <functional>

int main() {
    std::vector<int> data = {5, 2, 8, 1, 9, 3};

    // ✅ 正确: lambda 替代命名函数对象，就地定义排序谓词 — Core Guidelines ES.28
    std::sort(data.begin(), data.end(),
              [](int a, int b) { return a > b; });
    std::cout << "Sorted desc: ";
    // ✅ 正确: const auto& 避免拷贝，const 表明只读意图 — Core Guidelines ES.71
    for (const auto& x : data) std::cout << x << ' ';
    std::cout << '\n';

    // ✅ 正确: 值捕获 [multiplier] 明确列出依赖变量，避免隐式 [=] — Core Guidelines ES.28
    int multiplier = 2;
    auto doubler = [multiplier](int x) { return x * multiplier; };
    std::cout << "5 * multiplier = " << doubler(5) << '\n';

    // ✅ 正确: 引用捕获 [&counter] 明确捕获目标，可修改外部变量 — Core Guidelines ES.28
    int counter = 0;
    auto incrementer = [&counter]() { ++counter; };
    incrementer();
    incrementer();
    std::cout << "counter after 2 calls: " << counter << '\n';

    // 混合捕获: [=, &counter] 默认值捕获，counter 引用捕获
    // [&, multiplier] 默认引用捕获，multiplier 值捕获

    // ✅ 正确: 泛型 lambda 用 auto 参数实现多态，替代模板函数对象 — Core Guidelines ES.28
    auto genericAdd = [](auto a, auto b) { return a + b; };
    std::cout << "int: " << genericAdd(1, 2) << '\n';
    std::cout << "string: " << genericAdd(std::string("hello "), std::string("world")) << '\n';

    // ✅ 正确: std::function 类型擦除存储 lambda，灵活但略有开销 — Core Guidelines ES.28
    std::function<int(int, int)> calculator = [](int a, int b) { return a + b; };
    std::cout << "calculator: " << calculator(3, 4) << '\n';

    return 0;
}
