#include <iostream>
#include <iomanip>

int main() {
    size_t m = 0, n = 0;
    std::cout << "Enter rows (m): ";
    std::cin >> m;
    std::cout << "Enter cols (n): ";
    std::cin >> n;

    int** arr = new int*[m];
    for (size_t i = 0; i < m; ++i) {
        arr[i] = new int[n];
    }

    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            arr[i][j] = static_cast<int>((i + 1) * (j + 1));
        }
    }

    std::cout << "\nMultiplication table (" << m << "x" << n << "):\n";
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            std::cout << std::setw(4) << *(*(arr + i) + j);
        }
        std::cout << '\n';
    }

    for (size_t i = 0; i < m; ++i) {
        delete[] arr[i];
    }
    delete[] arr;

    return 0;
}
