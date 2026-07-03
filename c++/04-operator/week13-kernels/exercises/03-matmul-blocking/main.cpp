// Exercise 03: 矩阵乘法分块优化 + benchmark
// TODO: 实现 matmulNaive (基准实现)
// TODO: 实现 matmulBlocked (分块优化)
// TODO: 实现 benchmark 函数并对比性能
// TODO: 实现验证函数确认分块结果正确

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <random>
#include <cstring>
#include <cmath>

// ── 基准测试工具（已提供） ────────────────────────────────

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

// ── Level 0: 朴素实现 (TODO: 完成) ──────────────────────
// 三重循环 i-j-k 顺序
void matmulNaive(const double* A, const double* B, double* C,
                  size_t M, size_t N, size_t K) {
    // TODO: 实现朴素矩阵乘法 C = A × B
    // C[i*N + j] = sum_k A[i*K + k] * B[k*N + j]
    // 注意先清零 C

    // TODO: 清零 C
    // TODO: 三重循环计算
}

// ── Level 1: 循环交换优化 (TODO: 完成) ───────────────────
// i-k-j 顺序：内层连续访问
void matmulIkj(const double* A, const double* B, double* C,
                size_t M, size_t N, size_t K) {
    // TODO: 实现 ikj 循环顺序
    // for i in 0..M:
    //     for k in 0..K:
    //         aik = A[i*K + k]
    //         for j in 0..N:
    //             C[i*N + j] += aik * B[k*N + j]
}

// ── Level 2: 分块优化 (TODO: 完成) ──────────────────────
// 外层分块循环，内层用 ikj 顺序
void matmulBlocked(const double* A, const double* B, double* C,
                    size_t M, size_t N, size_t K) {
    // TODO: 选择分块大小 BLOCK
    // TODO: 四重循环：i0, k0, j0, i, k, j
    // 外层 i0/k0/j0 以 BLOCK 步进
    // 内层 i,k,j 在 BLOCK 范围内运行 ikj 顺序
    // 注意边界处理 min(i0+BLOCK, M)
}

// ── 验证函数（已提供） ──────────────────────────────────

bool verify(const double* a, const double* b, size_t n, double eps = 1e-9) {
    for (size_t i = 0; i < n; ++i) {
        if (std::abs(a[i] - b[i]) > eps) return false;
    }
    return true;
}

// ── main ────────────────────────────────────────────────

int main() {
    std::cout << "========== 习题03: 矩阵乘法分块优化 ==========\n\n";

    constexpr size_t SIZE = 512;  // 可调：256(小) 或 512(大) 或 1024(很大)
    constexpr size_t M = SIZE, N = SIZE, K = SIZE;

    std::cout << "矩阵规模: A(" << M << "×" << K << ") × B("
              << K << "×" << N << ") = C(" << M << "×" << N << ")\n\n";

    // ── 初始化数据 ─────────────────────────────────────
    std::vector<double> A(M * K), B(K * N);
    std::vector<double> C_naive(M * N), C_ikj(M * N), C_blocked(M * N);

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    for (auto& x : A) x = dist(rng);
    for (auto& x : B) x = dist(rng);

    // TODO: 运行三种实现并计时
    // double tNaive = benchmark([&] { matmulNaive(...); }, 3, 5);
    // double tIkj = benchmark([&] { matmulIkj(...); }, 3, 5);
    // double tBlocked = benchmark([&] { matmulBlocked(...); }, 3, 5);

    // TODO: 验证正确性
    // bool ikjOk = verify(C_naive.data(), C_ikj.data(), M*N);
    // bool blockedOk = verify(C_naive.data(), C_blocked.data(), M*N);

    // TODO: 输出性能对比表格
    // std::cout << "方法         时间(us)   GFLOPS   加速比\n";
    // std::cout << "朴素         ...\n";
    // std::cout << "循环交换     ...\n";
    // std::cout << "分块         ...\n";

    std::cout << "TODO: 实现三种矩阵乘法并对比性能\n";
    std::cout << "\n提示：\n";
    std::cout << "  - 分块大小 BLOCK 通常选 32/64/128（使子块装入 L1 cache）\n";
    std::cout << "  - 三块大小: BLOCK×BLOCK×8 bytes ≈ 32KB (L1 典型大小)\n";
    std::cout << "  - 内层用 ikj 顺序保证连续访问\n";
    std::cout << "  - 边界用 std::min(i0+BLOCK, M) 处理非对齐情况\n";
    std::cout << "  - GFLOPS = (2*M*N*K) / (seconds * 1e9)\n";

    return 0;
}
