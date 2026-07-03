// Exercise 03: Cache-Friendly Transpose — 完整解法

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <random>
#include <cstring>
#include <algorithm>

using Matrix = std::vector<std::vector<int>>;

template<typename F>
double benchmark(const char* name, F&& func, int iterations) {
    func();
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) func();
    auto end = std::chrono::steady_clock::now();
    auto total = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    double avg = static_cast<double>(total.count()) / iterations;
    std::cout << "  " << std::left << std::setw(35) << name
              << std::right << std::setw(10) << std::fixed << std::setprecision(2)
              << avg << " ms/iter\n";
    return avg;
}

Matrix createMatrix(int rows, int cols) {
    Matrix m(rows, std::vector<int>(cols));
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, 1000);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            m[i][j] = dist(rng);
    return m;
}

Matrix transposeNaive(const Matrix& src) {
    if (src.empty() || src[0].empty())
        return {};

    int rows = static_cast<int>(src.size());
    int cols = static_cast<int>(src[0].size());

    Matrix dst(cols, std::vector<int>(rows));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            dst[j][i] = src[i][j];  // 读连续，写跳跃 → cache miss
        }
    }

    return dst;
}

Matrix transposeBlocked(const Matrix& src, int blockSize = 16) {
    if (src.empty() || src[0].empty())
        return {};

    int rows = static_cast<int>(src.size());
    int cols = static_cast<int>(src[0].size());

    Matrix dst(cols, std::vector<int>(rows));

    // 分块转置：每个块在 L1 cache 内完成所有读写
    for (int ii = 0; ii < rows; ii += blockSize) {
        for (int jj = 0; jj < cols; jj += blockSize) {
            // 块内的行和列边界
            int iEnd = std::min(ii + blockSize, rows);
            int jEnd = std::min(jj + blockSize, cols);

            for (int i = ii; i < iEnd; ++i) {
                for (int j = jj; j < jEnd; ++j) {
                    dst[j][i] = src[i][j];
                }
            }
        }
    }

    return dst;
}

bool verify(const Matrix& src, const Matrix& transposed) {
    if (transposed.empty()) return false;
    int rows = static_cast<int>(src.size());
    int cols = static_cast<int>(src[0].size());
    if (static_cast<int>(transposed.size()) != cols) return false;
    if (static_cast<int>(transposed[0].size()) != rows) return false;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (transposed[j][i] != src[i][j])
                return false;
        }
    }
    return true;
}

int main() {
    std::cout << "========== 缓存友好矩阵转置 ==========\n\n";

    // 验证
    {
        std::cout << "── 小矩阵验证（2×3） ──\n";
        Matrix small = {{1, 2, 3}, {4, 5, 6}};
        std::cout << "  原始: " << small.size() << "×" << small[0].size() << '\n';

        auto t1 = transposeNaive(small);
        std::cout << "  朴素转置: " << t1.size() << "×" << t1[0].size()
                  << "  验证: " << (verify(small, t1) ? "通过" : "失败") << '\n';

        auto t2 = transposeBlocked(small, 8);
        std::cout << "  分块转置: " << t2.size() << "×" << t2[0].size()
                  << "  验证: " << (verify(small, t2) ? "通过" : "失败") << "\n\n";
    }

    // 基准测试
    constexpr int SIZE = 2048;
    constexpr int ITER = 3;

    std::cout << "── 大矩阵基准（" << SIZE << "×" << SIZE << "）──\n";
    std::cout << "  矩阵大小: " << SIZE << "×" << SIZE << "×4 字节 = "
              << (SIZE * SIZE * 4 / 1024 / 1024) << " MB\n";
    std::cout << "  L1 cache 典型: 32 KB → 可装约 64×64 int 块\n\n";

    Matrix src = createMatrix(SIZE, SIZE);

    // 验证结果正确性
    auto check = transposeNaive(src);
    auto blocked = transposeBlocked(src, 16);
    std::cout << "  结果验证: " << (verify(src, blocked) ? "通过" : "失败") << "\n\n";

    double tNaive = benchmark("朴素转置", [&] { transposeNaive(src); }, ITER);
    double tBlocked = benchmark("分块转置 (16×16)", [&] { transposeBlocked(src, 16); }, ITER);
    benchmark("分块转置 (32×32)", [&] { transposeBlocked(src, 32); }, ITER);
    benchmark("分块转置 (64×64)", [&] { transposeBlocked(src, 64); }, ITER);

    std::cout << "\n  加速比 (朴素/分块16): "
              << std::setprecision(2) << tNaive / tBlocked << "×\n";

    return 0;
}
