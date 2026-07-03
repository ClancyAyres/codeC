#include <iostream>

// TODO: 实现 min_max
// 通过 min 和 max 引用参数返回数组中的最小值和最大值
// 如果 size == 0，将 min 和 max 都设为 0
void min_max(const int* arr, size_t size, int& min, int& max) {
    // TODO: 处理空数组
    // TODO: 初始化为第一个元素
    // TODO: 遍历数组，更新 min 和 max
}

int main() {
    // 测试 1：普通数组
    int arr1[] = {5, 2, 9, 1, 7};
    int min1 = 0, max1 = 0;
    min_max(arr1, 5, min1, max1);
    std::cout << "Test 1: min=" << min1 << ", max=" << max1
              << " (expect min=1, max=9)\n";

    // 测试 2：单元素数组
    int arr2[] = {42};
    int min2 = 0, max2 = 0;
    min_max(arr2, 1, min2, max2);
    std::cout << "Test 2: min=" << min2 << ", max=" << max2
              << " (expect min=42, max=42)\n";

    // 测试 3：空数组
    int min3 = -1, max3 = -1;
    min_max(nullptr, 0, min3, max3);
    std::cout << "Test 3: min=" << min3 << ", max=" << max3
              << " (expect min=0, max=0)\n";

    // 测试 4：所有元素相同
    int arr4[] = {7, 7, 7, 7};
    int min4 = 0, max4 = 0;
    min_max(arr4, 4, min4, max4);
    std::cout << "Test 4: min=" << min4 << ", max=" << max4
              << " (expect min=7, max=7)\n";

    // 测试 5：递减数组
    int arr5[] = {10, 9, 8, 7, 6};
    int min5 = 0, max5 = 0;
    min_max(arr5, 5, min5, max5);
    std::cout << "Test 5: min=" << min5 << ", max=" << max5
              << " (expect min=6, max=10)\n";

    return 0;
}
