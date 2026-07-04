// 模块 1 示例: C 数组 vs std::array vs std::vector
// 相关规范: Core Guidelines SL.con.1, SL.con.2; Google Style: Arrays

#include <iostream>
#include <array>
#include <vector>

// ❌ C 数组作为参数: 退化为指针，丢失大小信息
void printCArray(const int* arr, size_t size) {
    std::cout << "[";
    for (size_t i = 0; i < size; ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << arr[i];
    }
    std::cout << "]\n";
}

// ✅ std::array: 不退化，自动知道大小
template<size_t N>
void printStdArray(const std::array<int, N>& arr) {
    std::cout << "[";
    for (size_t i = 0; i < arr.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << arr[i];
    }
    std::cout << "] (size=" << arr.size() << ")\n";
}

// ✅ std::vector: 最灵活
void printVector(const std::vector<int>& vec) {
    std::cout << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << vec[i];
    }
    std::cout << "] (size=" << vec.size() << ")\n";
}

int main() {
    // === C 风格数组 ===
    int cArr[] = {1, 2, 3, 4, 5};
    size_t cSize = sizeof(cArr) / sizeof(cArr[0]);  // 笨办法算大小
    printCArray(cArr, cSize);

    // === std::array (C++11) ===
    std::array<int, 5> stdArr = {1, 2, 3, 4, 5};
    printStdArray(stdArr);
    // ⚠️ .at() 有边界检查, [] 没有
    // std::cout << stdArr.at(10) << '\n';  // 抛 std::out_of_range

    // === std::vector (最常用) ===
    std::vector<int> vec = {1, 2, 3};
    vec.push_back(4);
    vec.push_back(5);
    printVector(vec);

    // vector 常用操作
    vec.pop_back();        // 删除最后一个
    vec.reserve(100);      // 预留容量（避免重新分配）
    std::cout << "size: " << vec.size() << ", capacity: " << vec.capacity() << '\n';

    // ❌ 常见扩展: 遍历中修改
    // for (auto& x : vec) {
    //     if (x == 2) vec.push_back(99);  // 危险! 可能使迭代器失效
    // }

    return 0;
}
