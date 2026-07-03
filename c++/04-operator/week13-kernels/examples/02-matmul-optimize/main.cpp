// Example 02: 矩阵乘法优化阶梯 — 三级优化对比 + chrono 计时
// Level 0: 朴素 ijk  (基准线)
// Level 1: 循环交换 ikj (缓存友好)
// Level 2: 分块 blocking (L1 cache 驻留)

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>
#include <cstring>

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

// ── Level 0: 朴素 ijk ───────────────────────────────────
void matmulNaive(const double* A, const double* B, double* C,
                  size_t M, size_t N, size_t K) {
    std::memset(C, 0, M * N * sizeof(double));
    for (size_t i = 0; i < M; ++i)
        for (size_t j = 0; j < N; ++j)
            for (size_t k = 0; k < K; ++k)
                C[i * N + j] += A[i * K + k] * B[k * N + j];
}

// ── Level 1: 循环交换 ikj ───────────────────────────────
void matmulLoopInterchange(const double* A, const double* B, double* C,
                            size_t M, size_t N, size_t K) {
    std::memset(C, 0, M * N * sizeof(double));
    for (size_t i = 0; i < M; ++i)
        for (size_t k = 0; k < K; ++k) {
            double aik = A[i * K + k];
            for (size_t j = 0; j < N; ++j)
                C[i * N + j] += aik * B[k * N + j];
        }
}

// ── Level 2: 分块 blocking ──────────────────────────────
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

// ── 验证 ────────────────────────────────────────────────
bool verify(const double* a, const double* b, size_t n, double eps = 1e-9) {
    for (size_t i = 0; i < n; ++i) {
        if (std::abs(a[i] - b[i]) > eps)
            return false;
    }
    return true;
}

int main() {
    std::cout << "========== 矩阵乘法优化阶梯 ==========\n\n";

    constexpr size_t M = 512, N = 512, K = 512;
    std::cout << "矩阵规模: A(" << M << "×" << K << ") × B("
              << K << "×" << N << ") = C(" << M << "×" << N << ")\n";
    std::cout << "BLOCK size = 64 (64×64×8B = 32KB ≈ L1 cache)\n\n";

    // ── 初始化 ─────────────────────────────────────────
    std::vector<double> A(M * K), B(K * N);
    std::vector<double> C0(M * N), C1(M * N), C2(M * N);

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    for (auto& x : A) x = dist(rng);
    for (auto& x : B) x = dist(rng);

    // ── 小矩阵验证正确性 ──────────────────────────────
    std::cout << "── 正确性验证（小矩阵 4×4）──\n";
    constexpr size_t SM = 4, SN = 4, SK = 4;
    std::vector<double> sA = {1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16};
    std::vector<double> sB = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    std::vector<double> sC0(SM*SN), sC1(SM*SN), sC2(SM*SN);

    matmulNaive(sA.data(), sB.data(), sC0.data(), SM, SN, SK);
    matmulLoopInterchange(sA.data(), sB.data(), sC1.data(), SM, SN, SK);
    matmulBlocked(sA.data(), sB.data(), sC2.data(), SM, SN, SK);

    std::cout << "  朴素:          "; for (auto v : sC0) std::cout << v << " "; std::cout << "\n";
    std::cout << "  循环交换:      "; for (auto v : sC1) std::cout << v << " "; std::cout << "\n";
    std::cout << "  分块:          "; for (auto v : sC2) std::cout << v << " "; std::cout << "\n";
    std::cout << "  朴素 vs 交换:  " << (verify(sC0.data(), sC1.data(), SM*SN) ? "✓" : "✗") << "\n";
    std::cout << "  朴素 vs 分块:  " << (verify(sC0.data(), sC2.data(), SM*SN) ? "✓" : "✗") << "\n\n";

    // ── 大矩阵基准测试 ──────────────────────────────────
    std::cout << "── 性能基准 (" << M << "×" << K << " × " << K << "×" << N << ", 热身10, 迭代20) ──\n\n";

    double t0 = benchmark([&] { matmulNaive(A.data(), B.data(), C0.data(), M, N, K); }, 10, 20);
    double t1 = benchmark([&] { matmulLoopInterchange(A.data(), B.data(), C1.data(), M, N, K); }, 10, 20);
    double t2 = benchmark([&] { matmulBlocked(A.data(), B.data(), C2.data(), M, N, K); }, 10, 20);

    double gflops0 = computeGFLOPS(M, N, K, t0);
    double gflops1 = computeGFLOPS(M, N, K, t1);
    double gflops2 = computeGFLOPS(M, N, K, t2);

    std::cout << std::setw(20) << "方法"
              << std::setw(14) << "时间(us)"
              << std::setw(12) << "GFLOPS"
              << std::setw(10) << "加速比" << "\n";
    std::cout << std::string(56, '-') << "\n";

    std::cout << std::setw(20) << "朴素 ijk"
              << std::setw(12) << std::fixed << std::setprecision(0) << t0
              << std::setw(11) << std::setprecision(4) << gflops0
              << std::setw(9) << "1.00×" << "\n";

    std::cout << std::setw(20) << "循环交换 ikj"
              << std::setw(12) << std::setprecision(0) << t1
              << std::setw(11) << std::setprecision(4) << gflops1
              << std::setw(8) << std::setprecision(2) << (t0 / t1) << "×" << "\n";

    std::cout << std::setw(20) << "分块 blocking"
              << std::setw(12) << std::setprecision(0) << t2
              << std::setw(11) << std::setprecision(4) << gflops2
              << std::setw(8) << std::setprecision(2) << (t0 / t2) << "×" << "\n";

    std::cout << "\n验证: 循环交换 vs 朴素: "
              << (verify(C0.data(), C1.data(), M*N) ? "✓ 一致" : "✗ 不一致") << "\n";
    std::cout << "       分块 vs 朴素:     "
              << (verify(C0.data(), C2.data(), M*N) ? "✓ 一致" : "✗ 不一致") << "\n";

    std::cout << "\n── 分析 ──\n";
    std::cout << "朴素:   内层 B[k*N+j] → k 递增跳 N 个元素 → 每次 cache miss\n";
    std::cout << "交换:   aik 提取到寄存器, 内层 j 递增 → 3 个数组全部连续访问\n";
    std::cout << "分块:   BLOCK×BLOCK 子块常驻 L1 (32KB) → 缓存复用率最高\n";
    std::cout << "        理论峰值（单核双精度）: ~25-50 GFLOPS（取决于 CPU）\n";

    return 0;
}
