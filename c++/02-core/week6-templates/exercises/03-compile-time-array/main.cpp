#include <iostream>
#include <initializer_list>
#include <algorithm>

// TODO: 实现 CompileArray<T, N> 类模板
template<typename T, size_t N>
class CompileArray {
    T data_[N];

public:
    CompileArray() {
        // TODO: 用 T{} 填充所有元素
    }

    CompileArray(std::initializer_list<T> init) {
        // TODO: 从初始化列表拷贝元素
        // 提示：用 std::copy 或手动循环
    }

    T& operator[](size_t index) {
        // TODO
    }

    const T& operator[](size_t index) const {
        // TODO
    }

    constexpr size_t size() const {
        // TODO: 返回 N
        return 0;
    }

    T* begin() {
        // TODO
        return nullptr;
    }

    T* end() {
        // TODO
        return nullptr;
    }

    const T* begin() const {
        // TODO
        return nullptr;
    }

    const T* end() const {
        // TODO
        return nullptr;
    }

    void fill(const T& value) {
        // TODO: 用 value 填充所有元素
    }
};

int main() {
    std::cout << "=== Default Construction ===\n";
    CompileArray<int, 5> arr1;
    std::cout << "arr1 size: " << arr1.size() << '\n';
    std::cout << "arr1 elements: ";
    for (size_t i = 0; i < arr1.size(); ++i) {
        std::cout << arr1[i] << ' ';
    }
    std::cout << '\n';

    std::cout << "\n=== Initializer List ===\n";
    CompileArray<int, 3> arr2 = {10, 20, 30};
    std::cout << "arr2 size: " << arr2.size() << '\n';
    std::cout << "arr2 elements: ";
    for (size_t i = 0; i < arr2.size(); ++i) {
        std::cout << arr2[i] << ' ';
    }
    std::cout << '\n';

    std::cout << "\n=== Fill ===\n";
    arr1.fill(7);
    std::cout << "After fill(7): ";
    for (size_t i = 0; i < arr1.size(); ++i) {
        std::cout << arr1[i] << ' ';
    }
    std::cout << '\n';

    std::cout << "\n=== Element Access ===\n";
    std::cout << "arr2[0] = " << arr2[0] << '\n';
    std::cout << "arr2[2] = " << arr2[2] << '\n';

    std::cout << "\n=== Range-based For ===\n";
    for (const auto& elem : arr2) {
        std::cout << elem << ' ';
    }
    std::cout << '\n';

    std::cout << "\n=== Compile-time Size ===\n";
    CompileArray<int, 10> arr3;
    std::cout << "CompileArray<int, 5> and CompileArray<int, 10> are different types!\n";
    std::cout << "arr1.size() = " << arr1.size() << ", arr3.size() = " << arr3.size() << '\n';

    return 0;
}
