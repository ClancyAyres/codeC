// Exercise 03: Cache-Friendly Transpose — 缓存友好的矩阵转置
// TODO: 对比朴素转置（极慢）与分块转置（缓存友好）

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <random>
#include <cstring>

using Matrix = std::vector<std::vector<int>>;

// ── 基准测试 ──────────────────────────────────────────────

template<typename F>
double benchmark(const char* name, F&& func, int iterations) {
    func();  // 预热
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

template<typename T>
void doNotOptimize(T const& value) {
    asm volatile("" : : "r,m"(value) : "memory");
}

// ── 矩阵工具 ──────────────────────────────────────────────

Matrix createMatrix(int rows, int cols) {
    Matrix m(rows, std::vector<int>(cols));
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, 1000);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            m[i][j] = dist(rng);
    return m;
}

// ── TODO: 实现朴素转置 ────────────────────────────────────
//
// 输入: src (rows × cols)
// 输出: dst (cols × rows), dst[j][i] = src[i][j]
//
// 注意：这是"写时跳跃访问"，不是"读时跳跃"
// 朴素实现中写 dst[j][i] 是 column-major → 大量 cache miss
Matrix transposeNaive(const Matrix& src) {
    // TODO: 参数校验
    // TODO: 分配 dst(cols × rows)
    // TODO: 遍历 src[i][j] → dst[j][i]
    return {};
}

// ── TODO: 实现分块转置（Block/Tile） ─────────────────────
//
// 将矩阵分块（如 16×16），逐块转置。
// 每个小块可以完全放入 L1 cache → 减少 miss。
//
// 输入: src (rows × cols)
// 输出: dst (cols × rows)
Matrix transposeBlocked(const Matrix& src, int blockSize = 16) {
    // TODO: 参数校验
    // TODO: 分配 dst(cols × rows)
    // TODO: 外两层遍历块边界 (ii, jj)
    // TODO: 内两层遍历块内元素 (i, j)
    //   dst[jj + j][ii + i] = src[ii + i][jj + j];
    return {};
}

// ── 验证函数 ──────────────────────────────────────────────

bool verify(const Matrix& src, const Matrix& transposed) {
    if (transposed.empty()) return false;
    int rows = src.size();
    int cols = src[0].size();
    if (transposed.size() != static_cast<size_t>(cols)) return false;
    if (transposed[0].size() != static_cast<size_t>(rows)) return false;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (transposed[j][i] != src[i][j])
                return false;
        }
    }
    return true;
}

// ── main ──────────────────────────────────────────────────

int main() {
    std::cout << "========== 缓存友好矩阵转置 ==========\n\n";

    // 测试小矩阵（验证用）
    {
        std::cout << "── 小矩阵验证（3×3） ──\n";
        Matrix small = {{1, 2, 3}, {4, 5, 6}};
        std::cout << "  原始: " << small.size() << "×" << small[0].size() << '\n';

        // TODO: 调用你的转置函数
        // auto t = transposeNaive(small);
        // std::cout << "  转置: " << t.size() << "×" << t[0].size() << '\n';
        // std::cout << "  验证: " << (verify(small, t) ? "通过" : "失败") << "\n\n";

        std::cout << "  TODO: 取消注释上述代码\n\n";
    }

    // 基准测试大矩阵
    constexpr int SIZE = 2048;  // 2048×2048 int = 16 MB
    constexpr int ITER = 5;

    std::cout << "── 大矩阵基准（" << SIZE << "×" << SIZE << "）──\n";
    std::cout << "  矩阵大小: " << SIZE << "×" << SIZE << "×4 字节 = "
              << (SIZE * SIZE * 4 / 1024 / 1024) << " MB\n";
    std::cout << "  L1 cache 典型: 32 KB → 可装约 64×64 int 块\n\n";

    Matrix src = createMatrix(SIZE, SIZE);

    // TODO: benchmark(..) 调用你的朴素转置
    // TODO: benchmark(..) 调用你的分块转置

    std::cout << "\nTODO: 完成实现后取消注释\n";
    std::cout << "预期结果:\n";
    std::cout << "  朴素转置:  ~100-300 ms（大量 L3 miss）\n";
    std::cout << "  分块转置:  ~10-30 ms（L1 命中率高）\n";
    std::cout << "  加速比: ~5-15×\n";

    return 0;
}
