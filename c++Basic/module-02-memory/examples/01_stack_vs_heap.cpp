// 模块 2 示例: 栈 vs 堆
// 相关规范: Core Guidelines R.11, C.149; Google Style: Ownership and Smart Pointers

#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Resource {
    std::string name;
    explicit Resource(std::string n) : name(std::move(n)) {
        std::cout << "  [构造] " << name << '\n';
    }
    ~Resource() {
        std::cout << "  [析构] " << name << '\n';
    }
};

void stackExample() {
    std::cout << "--- 栈对象 ---\n";
    Resource r1("stack-obj");
    std::cout << "  在栈上安全使用 " << r1.name << '\n';
}

void heapRawExample() {
    std::cout << "--- 裸指针堆对象 ---\n";
    Resource* r2 = new Resource("heap-raw");
    std::cout << "  使用 " << r2->name << '\n';
    delete r2;
}

void heapSmartExample() {
    std::cout << "--- 智能指针堆对象 ---\n";
    auto r3 = std::make_unique<Resource>("heap-unique");
    std::cout << "  使用 " << r3->name << '\n';
}

int main() {
    stackExample();
    std::cout << "stackExample 调用结束，栈对象已自动析构\n\n";

    heapRawExample();
    std::cout << "heapRawExample 调用结束\n\n";

    heapSmartExample();
    std::cout << "heapSmartExample 调用结束\n\n";

    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::cout << "vector 在栈上，但它的数据在堆上，析构时自动释放\n";

    return 0;
}
