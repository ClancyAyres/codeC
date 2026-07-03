// Exercise 01: Parallel Matrix Multiplication
// TODO: 实现并行矩阵乘法，将行分配给不同线程

#include <iostream>
#include <vector>
#include <thread>
#include <iomanip>

using Matrix = std::vector<std::vector<int>>;

// TODO: 实现矩阵乘法
// - 将结果矩阵的每一行分配给不同线程计算
// - 使用指定数量的线程
// - 确保矩阵维度兼容（A 的列数 == B 的行数）
Matrix multiply(const Matrix& A, const Matrix& B) {
    // TODO: 参数校验（A 列数 != B 行数时抛异常）
    // TODO: 初始化结果矩阵（A 行数 × B 列数，全 0）
    // TODO: 创建线程，每个线程计算若干行
    // TODO: 等待所有线程完成
    return {};
}

// TODO: 实现并行矩阵乘法（可指定线程数）
Matrix parallelMultiply(const Matrix& A, const Matrix& B, int numThreads) {
    // TODO: 如果 numThreads <= 0，使用硬件并发数
    // TODO: 分派行到各线程
    // TODO: join 所有线程
    return {};
}

void printMatrix(const char* name, const Matrix& m) {
    std::cout << name << ":\n";
    for (const auto& row : m) {
        for (int val : row)
            std::cout << std::setw(4) << val;
        std::cout << '\n';
    }
}

int main() {
    Matrix A = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    Matrix B = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };

    printMatrix("A", A);
    printMatrix("B", B);

    std::cout << "\nHardware concurrency: "
              << std::thread::hardware_concurrency() << '\n';

    Matrix C = parallelMultiply(A, B, 2);
    printMatrix("A × B", C);

    // std::cout << "\nC[0][0] = " << C[0][0] << " (期望: 30)\n";
    // std::cout << "C[2][2] = " << C[2][2] << " (期望: 24)\n";

    return 0;
}
