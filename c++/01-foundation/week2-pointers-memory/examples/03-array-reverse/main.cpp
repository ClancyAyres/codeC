#include <iostream>

void reverse(int* begin, int* end) {
    // end 指向最后一个元素之后的位置（遵循 STL 惯例）
    while (begin < end) {
        --end;
        int temp = *begin;
        *begin = *end;
        *end = temp;
        ++begin;
    }
}

void print(const int* begin, const int* end) {
    std::cout << "[";
    for (const int* p = begin; p != end; ++p) {
        if (p != begin) std::cout << ", ";
        std::cout << *p;
    }
    std::cout << "]\n";
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t size = sizeof(arr) / sizeof(arr[0]);

    std::cout << "Original:  ";
    print(arr, arr + size);

    reverse(arr, arr + size);

    std::cout << "Reversed:  ";
    print(arr, arr + size);

    // 演示指针与数组下标的等价性
    std::cout << "\nPointer/index equivalence:\n";
    int* ptr = arr;
    std::cout << "  ptr[2]    = " << ptr[2] << '\n';
    std::cout << "  2[ptr]    = " << 2[ptr] << '\n';
    std::cout << "  *(ptr+2)  = " << *(ptr + 2) << '\n';
    std::cout << "  *(2+ptr)  = " << *(2 + ptr) << '\n';

    // 指针算术演示
    std::cout << "\nPointer arithmetic:\n";
    int* p = arr;
    std::cout << "  *p       = " << *p << " (arr[0])\n";
    p += 2;
    std::cout << "  p += 2 → *p = " << *p << " (arr[2])\n";
    p--;
    std::cout << "  p--    → *p = " << *p << " (arr[1])\n";

    ptrdiff_t diff = (arr + size) - arr;
    std::cout << "\n  Elements between begin and end: " << diff << '\n';

    return 0;
}
