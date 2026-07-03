#include <iostream>
#include <initializer_list>
#include <algorithm>

template<typename T, size_t N>
class CompileArray {
    T data_[N];

public:
    CompileArray() {
        for (size_t i = 0; i < N; ++i) {
            data_[i] = T{};
        }
    }

    CompileArray(std::initializer_list<T> init) {
        size_t i = 0;
        for (const auto& elem : init) {
            if (i < N) {
                data_[i++] = elem;
            }
        }
        for (; i < N; ++i) {
            data_[i] = T{};
        }
    }

    T& operator[](size_t index) {
        return data_[index];
    }

    const T& operator[](size_t index) const {
        return data_[index];
    }

    constexpr size_t size() const {
        return N;
    }

    T* begin() {
        return data_;
    }

    T* end() {
        return data_ + N;
    }

    const T* begin() const {
        return data_;
    }

    const T* end() const {
        return data_ + N;
    }

    void fill(const T& value) {
        for (size_t i = 0; i < N; ++i) {
            data_[i] = value;
        }
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
