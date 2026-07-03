#include <iostream>

int main() {
    size_t m = 0, n = 0;
    std::cout << "Enter rows (m): ";
    std::cin >> m;
    std::cout << "Enter cols (n): ";
    std::cin >> n;

    // TODO: 使用 new 分配 m×n 的二维 int 数组
    // 步骤 1：分配行指针数组 int** arr = new int*[m];
    // 步骤 2：为每行分配列数组   arr[i] = new int[n];

    // TODO: 填充乘法表 arr[i][j] = (i+1) * (j+1)

    // TODO: 使用纯指针算术打印矩阵
    // 格式：每个数字占 4 个字符宽度，每行以换行结束
    // 必须使用 *(*(arr + i) + j)，不允许使用 arr[i][j]

    // TODO: 使用 delete[] 正确释放所有内存
    // 步骤 1：先释放每行的列数组  delete[] arr[i];
    // 步骤 2：再释放行指针数组    delete[] arr;

    return 0;
}
