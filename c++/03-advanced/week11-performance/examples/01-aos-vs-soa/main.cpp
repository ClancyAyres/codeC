// Example 01: AoS vs SoA — 结构体数组 vs 数组结构体性能对比
// 目的：展示不同内存布局对缓存利用率的影响

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <numeric>
#include <random>
#include <cmath>

constexpr size_t N = 10'000'000;
constexpr int WARMUP = 3;
constexpr int ITERATIONS = 10;

// ── AoS: Array of Structures ─────────────────────────────────
struct ParticleAoS {
    float x, y, z;
    float vx, vy, vz;
    float mass;
};

// ── SoA: Structure of Arrays ─────────────────────────────────
struct ParticleSoA {
    std::vector<float> x, y, z;
    std::vector<float> vx, vy, vz;
    std::vector<float> mass;

    ParticleSoA(size_t n)
        : x(n), y(n), z(n), vx(n), vy(n), vz(n), mass(n) {}
};

template<typename F>
double benchmark(F&& func, int iterations) {
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        func();
    }
    auto end = std::chrono::steady_clock::now();
    auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return static_cast<double>(total.count()) / iterations;
}

template<typename T>
void doNotOptimize(T const& value) {
    asm volatile("" : : "r,m"(value) : "memory");
}

int main() {
    std::cout << "========== AoS vs SoA 性能对比 (N = " << N << ") ==========\n";
    std::cout << "Cache line: 64 字节 | float = 4 字节 | 一行装 16 个 float\n\n";

    // ── 初始化数据 ─────────────────────────────────────
    std::vector<ParticleAoS> aos(N);

    std::mt19937 rng(42);
    std::uniform_real_distribution<float> dist(-100.f, 100.f);

    for (size_t i = 0; i < N; ++i) {
        aos[i].x   = dist(rng);
        aos[i].y   = dist(rng);
        aos[i].z   = dist(rng);
        aos[i].vx  = dist(rng);
        aos[i].vy  = dist(rng);
        aos[i].vz  = dist(rng);
        aos[i].mass = std::abs(dist(rng)) + 1.f;
    }

    ParticleSoA soa(N);
    for (size_t i = 0; i < N; ++i) {
        soa.x[i]   = aos[i].x;
        soa.y[i]   = aos[i].y;
        soa.z[i]   = aos[i].z;
        soa.vx[i]  = aos[i].vx;
        soa.vy[i]  = aos[i].vy;
        soa.vz[i]  = aos[i].vz;
        soa.mass[i]= aos[i].mass;
    }

    // ── 基准 1: AoS 只更新位置 ──────────────────────────
    std::cout << "── 测试1: 只更新位置 (x += vx, y += vy, z += vz) ──\n";

    auto benchAoS = [&] {
        for (size_t i = 0; i < N; ++i) {
            aos[i].x += aos[i].vx;
            aos[i].y += aos[i].vy;
            aos[i].z += aos[i].vz;
        }
        doNotOptimize(aos[0].x);
    };
    double timeAoS = benchmark(benchAoS, ITERATIONS);
    std::cout << "  AoS: " << std::fixed << std::setprecision(2)
              << timeAoS << " us/iter\n";
    std::cout << "  分析: 每次从 Ao" << std::fixed << std::setprecision(2)
              << "S 加载一条 cache line (64 字节)\n";
    std::cout << "        ParticleAoS = 28 字节，64 字节装 ~2.3 个\n";
    std::cout << "        但只用到了 x, y, z, vx, vy, vz (24 字节)\n";
    std::cout << "        有效利用率: " << (24.0 / 28.0 * 100) << "% 的结构，\n";
    std::cout << "        加上跨 cache line 浪费 → 实际更低\n\n";

    // ── 基准 2: SoA 只更新位置 ──────────────────────────
    auto benchSoA = [&] {
        for (size_t i = 0; i < N; ++i) {
            soa.x[i] += soa.vx[i];
            soa.y[i] += soa.vy[i];
            soa.z[i] += soa.vz[i];
        }
        doNotOptimize(soa.x[0]);
    };
    double timeSoA = benchmark(benchSoA, ITERATIONS);
    std::cout << "  SoA: " << std::setprecision(2)
              << timeSoA << " us/iter\n";
    std::cout << "  分析: 连续访问 float 数组 → 一条 cache line 装载 16 个元素\n";
    std::cout << "        有效利用率: 100%（所有 64 字节都被使用）\n";
    std::cout << "        预取器也能很好地预测访问模式\n\n";

    std::cout << "  加速比: AoS/SoA = " << std::setprecision(2)
              << timeAoS / timeSoA << "x\n\n";

    // ── 基准 3: AoS 访问所有字段（最优场景） ─────────────
    std::cout << "── 测试2: 访问所有字段（AoS 的最佳场景） ──\n";

    auto benchAoSFull = [&] {
        float totalMass = 0;
        for (size_t i = 0; i < N; ++i) {
            aos[i].x += aos[i].vx;
            aos[i].y += aos[i].vy;
            aos[i].z += aos[i].vz;
            totalMass += aos[i].mass;
        }
        doNotOptimize(totalMass);
    };
    double timeAoSFull = benchmark(benchAoSFull, ITERATIONS);

    auto benchSoAFull = [&] {
        float totalMass = 0;
        for (size_t i = 0; i < N; ++i) {
            soa.x[i] += soa.vx[i];
            soa.y[i] += soa.vy[i];
            soa.z[i] += soa.vz[i];
            totalMass += soa.mass[i];
        }
        doNotOptimize(totalMass);
    };
    double timeSoAFull = benchmark(benchSoAFull, ITERATIONS);

    std::cout << "  AoS: " << std::setprecision(2) << timeAoSFull << " us/iter\n";
    std::cout << "  SoA: " << std::setprecision(2) << timeSoAFull << " us/iter\n";
    std::cout << "  加速比: AoS/SoA = " << std::setprecision(2)
              << timeAoSFull / timeSoAFull << "x\n\n";

    std::cout << "========== 结论 ==========\n";
    std::cout << "SoA 在部分字段遍历时优势明显（缓存利用率高）\n";
    std::cout << "AoS 在需要访问所有字段时差距缩小\n";
    std::cout << "选择取决于访问模式：Hot path 用 SoA，随机访问用 AoS\n";

    return 0;
}
