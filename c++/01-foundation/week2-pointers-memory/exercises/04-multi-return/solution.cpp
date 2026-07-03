#include <iostream>

void min_max(const int* arr, size_t size, int& min, int& max) {
    if (size == 0 || arr == nullptr) {
        min = 0;
        max = 0;
        return;
    }

    min = arr[0];
    max = arr[0];

    for (size_t i = 1; i < size; ++i) {
        if (arr[i] < min) {
            min = arr[i];
        }
        if (arr[i] > max) {
            max = arr[i];
        }
    }
}

int main() {
    int arr1[] = {5, 2, 9, 1, 7};
    int min1 = 0, max1 = 0;
    min_max(arr1, 5, min1, max1);
    std::cout << "Test 1: min=" << min1 << ", max=" << max1
              << " (expect min=1, max=9)\n";

    int arr2[] = {42};
    int min2 = 0, max2 = 0;
    min_max(arr2, 1, min2, max2);
    std::cout << "Test 2: min=" << min2 << ", max=" << max2
              << " (expect min=42, max=42)\n";

    int min3 = -1, max3 = -1;
    min_max(nullptr, 0, min3, max3);
    std::cout << "Test 3: min=" << min3 << ", max=" << max3
              << " (expect min=0, max=0)\n";

    int arr4[] = {7, 7, 7, 7};
    int min4 = 0, max4 = 0;
    min_max(arr4, 4, min4, max4);
    std::cout << "Test 4: min=" << min4 << ", max=" << max4
              << " (expect min=7, max=7)\n";

    int arr5[] = {10, 9, 8, 7, 6};
    int min5 = 0, max5 = 0;
    min_max(arr5, 5, min5, max5);
    std::cout << "Test 5: min=" << min5 << ", max=" << max5
              << " (expect min=6, max=10)\n";

    return 0;
}
