// Exercise 03: 矩阵乘法分块优化 — 完整解法

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <random>
#include <cstring>
#include <cmath>

template<typename F>
double benchmark(F&& func, int warmup, int iterations) {
    for (int i = 0; i < warmup; ++i) func();
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) func();
    auto end = std::chrono::steady_clock::now();
    auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return static_cast<double>(total.count()) / iterations;
}

double computeGFLOPS(size_t M, size_t N, size_t K, double usPerIter) {
    double seconds = usPerIter / 1'000'000.0;
    double flops = 2.0 * M * N * K;
    return flops / (seconds * 1e9);
}

// ── Level 0: 朴素实现 ──────────────────────────────────
void matmulNaive(const double* A, const double* B, double* C,
                  size_t M, size_t N, size_t K) {
    std::memset(C, 0, M * N * sizeof(double));
    for (size_t i = 0; i < M; ++i)
        for (size_t j = 0; j < N; ++j)
            for (size_t k = 0; k < K; ++k)
                C[i * N + j] += A[i * K + k] * B[k * N + j];
}

// ── Level 1: 循环交换优化 ───────────────────────────────
void matmulIkj(const double* A, const double* B, double* C,
                size_t M, size_t N, size_t K) {
    std::memset(C, 0, M * N * sizeof(double));
    for (size_t i = 0; i < M; ++i)
        for (size_t k = 0; k < K; ++k) {
            double aik = A[i * K + k];
            for (size_t j = 0; j < N; ++j)
                C[i * N + j] += aik * B[k * N + j];
        }
}

// ── Level 2: 分块优化 ──────────────────────────────────
void matmulBlocked(const double* A, const double* B, double* C,
                    size_t M, size_t N, size_t K) {
    constexpr size_t BLOCK = 64;
    std::memset(C, 0, M * N * sizeof(double));

    for (size_t i0 = 0; i0 < M; i0 += BLOCK)
        for (size_t k0 = 0; k0 < K; k0 += BLOCK)
            for (size_t j0 = 0; j0 < N; j0 += BLOCK) {
                size_t iEnd = std::min(i0 + BLOCK, M);
                size_t jEnd = std::min(j0 + BLOCK, N);
                size_t kEnd = std::min(k0 + BLOCK, K);

                for (size_t i = i0; i < iEnd; ++i)
                    for (size_t k = k0; k < kEnd; ++k) {
                        double aik = A[i * K + k];
                        for (size_t j = j0; j < jEnd; ++j)
                            C[i * N + j] += aik * B[k * N + j];
                    }
            }
}

// ── 验证 ──────────────────────────────────────────────
bool verify(const double* a, const double* b, size_t n, double eps = 1e-9) {
    for (size_t i = 0; i < n; ++i)
        if (std::abs(a[i] - b[i]) > eps) return false;
    return true;
}

int main() {
    std::cout << "========== 习题03: 矩阵乘法分块优化 ==========\n\n";

    constexpr size_t SIZE = 512;
    constexpr size_t M = SIZE, N = SIZE, K = SIZE;

    std::cout << "矩阵规模: A(" << M << "×" << K << ") × B("
              << K << "×" << N << ") = C(" << M << "×" << N << ")\n";
    std::cout << "BLOCK = 64 (子块: 64×64×8B = 32KB)\n";
    std::cout << "每个iteration计算: " << (2.0 * M * N * K / 1e9) << " GFLOPs\n\n";

    // ── 初始化 ─────────────────────────────────────────
    std::vector<double> A(M * K), B(K * N);
    std::vector<double> C_naive(M * N), C_ikj(M * N), C_blocked(M * N);

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    for (auto& x : A) x = dist(rng);
    for (auto& x : B) x = dist(rng);

    // ── 基准测试 ──────────────────────────────────────
    constexpr int WARMUP = 5;
    constexpr int ITER = 10;

    std::cout << "── 基准测试 (热身 " << WARMUP << ", 迭代 " << ITER << ") ──\n\n";

    double tNaive = benchmark([&] {
        matmulNaive(A.data(), B.data(), C_naive.data(), M, N, K);
    }, WARMUP, ITER);

    double tIkj = benchmark([&] {
        matmulIkj(A.data(), B.data(), C_ikj.data(), M, N, K);
    }, WARMUP, ITER);

    double tBlocked = benchmark([&] {
        matmulBlocked(A.data(), B.data(), C_blocked.data(), M, N, K);
    }, WARMUP, ITER);

    double gf0 = computeGFLOPS(M, N, K, tNaive);
    double gf1 = computeGFLOPS(M, N, K, tIkj);
    double gf2 = computeGFLOPS(M, N, K, tBlocked);

    // ── 输出表格 ──────────────────────────────────────
    std::cout << std::left
              << std::setw(18) << "方法"
              << std::right
              << std::setw(12) << "时间(us)"
              << std::setw(12) << "GFLOPS"
              << std::setw(10) << "加速比" << "\n";
    std::cout << std::string(52, '-') << "\n";

    std::cout << std::left << std::setw(18) << "朴素 ijk"
              << std::right << std::setw(10) << std::fixed << std::setprecision(0) << tNaive
              << std::setw(11) << std::setprecision(4) << gf0
              << std::setw(9) << "1.00×" << "\n";

    std::cout << std::left << std::setw(18) << "循环交换 ikj"
              << std::right << std::setw(10) << std::setprecision(0) << tIkj
              << std::setw(11) << std::setprecision(4) << gf1
              << std::setw(8) << std::setprecision(2) << (tNaive / tIkj) << "×" << "\n";

    std::cout << std::left << std::setw(18) << "分块 blocking"
              << std::right << std::setw(10) << std::setprecision(0) << tBlocked
              << std::setw(11) << std::setprecision(4) << gf2
              << std::setw(8) << std::setprecision(2) << (tNaive / tBlocked) << "×" << "\n";

    // ── 验证 ──────────────────────────────────────────
    std::cout << "\n── 正确性验证 ──\n";
    bool ikjOk = verify(C_naive.data(), C_ikj.data(), M * N);
    bool blkOk = verify(C_naive.data(), C_blocked.data(), M * N);
    std::cout << "  循环交换 vs 朴素: " << (ikjOk ? "✓ 一致" : "✗ 不一致") << "\n";
    std::cout << "  分块 vs 朴素:     " << (blkOk ? "✓ 一致" : "✗ 不一致") << "\n";

    // ── 分析 ──────────────────────────────────────────
    std::cout << "\n── 分析 ──\n";
    std::cout << "  GFLOPS = 2×M×N×K / time ≈ " << std::setprecision(2) << gf2 << " GFLOPS\n";
    std::cout << "  典型单核双精度峰值: 25-50 GFLOPS（取决于 CPU）\n";
    std::cout << "  效率 = " << std::setprecision(1) << (gf2 / 50.0 * 100) << "%（假设峰值 50 GFLOPS）\n";

    std::cout << "\n── 全部测试完成 ──\n";
    return 0;
}
