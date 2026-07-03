// Exercise 01: Parallel Matrix Multiplication — 完整解法

#include <iostream>
#include <vector>
#include <thread>
#include <iomanip>
#include <stdexcept>
#include <algorithm>

using Matrix = std::vector<std::vector<int>>;

Matrix multiply(const Matrix& A, const Matrix& B) {
    if (A.empty() || B.empty() || A[0].empty() || B[0].empty())
        throw std::invalid_argument("Matrix must not be empty");
    if (A[0].size() != B.size())
        throw std::invalid_argument("Incompatible dimensions: A.cols != B.rows");

    int rowsA = A.size();
    int colsA = A[0].size();
    int colsB = B[0].size();

    Matrix C(rowsA, std::vector<int>(colsB, 0));

    for (int i = 0; i < rowsA; ++i) {
        for (int k = 0; k < colsA; ++k) {
            for (int j = 0; j < colsB; ++j) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}

Matrix parallelMultiply(const Matrix& A, const Matrix& B, int numThreads) {
    if (A.empty() || B.empty() || A[0].empty() || B[0].empty())
        throw std::invalid_argument("Matrix must not be empty");
    if (A[0].size() != B.size())
        throw std::invalid_argument("Incompatible dimensions: A.cols != B.rows");

    if (numThreads <= 0)
        numThreads = std::thread::hardware_concurrency();

    int rowsA = A.size();
    int colsA = A[0].size();
    int colsB = B[0].size();

    Matrix C(rowsA, std::vector<int>(colsB, 0));

    numThreads = std::min(numThreads, rowsA);
    std::vector<std::thread> threads;

    int rowsPerThread = rowsA / numThreads;
    int remainder = rowsA % numThreads;
    int startRow = 0;

    for (int t = 0; t < numThreads; ++t) {
        int count = rowsPerThread + (t < remainder ? 1 : 0);
        int endRow = startRow + count;

        threads.emplace_back([&C, &A, &B, startRow, endRow, colsA, colsB] {
            for (int i = startRow; i < endRow; ++i) {
                for (int k = 0; k < colsA; ++k) {
                    for (int j = 0; j < colsB; ++j) {
                        C[i][j] += A[i][k] * B[k][j];
                    }
                }
            }
        });

        startRow = endRow;
    }

    for (auto& t : threads)
        t.join();

    return C;
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

    std::cout << "\nC[0][0] = " << C[0][0] << " (期望: 30)\n";
    std::cout << "C[2][2] = " << C[2][2] << " (期望: 90)\n";

    return 0;
}
