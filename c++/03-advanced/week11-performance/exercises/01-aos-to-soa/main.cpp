// Exercise 01: AoS to SoA — 将 AoS 转换为 SoA 并基准测试
// TODO: 将 AoS 版粒子结构转换为 SoA，对比性能差异

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <random>
#include <cmath>
#include <numeric>
#include <algorithm>

// ── AoS 版本（已有） ─────────────────────────────────────
struct ParticleAoS {
    float x, y, z;        // 位置
    float vx, vy, vz;     // 速度
    float mass;           // 质量
    float life;           // 剩余生命
    int   id;             // 编号
};

std::vector<ParticleAoS> generateAoS(size_t n) {
    std::vector<ParticleAoS> p(n);
    std::mt19937 rng(42);
    std::uniform_real_distribution<float> dist(-100.f, 100.f);
    for (size_t i = 0; i < n; ++i) {
        p[i].x    = dist(rng);
        p[i].y    = dist(rng);
        p[i].z    = dist(rng);
        p[i].vx   = dist(rng) * 0.1f;
        p[i].vy   = dist(rng) * 0.1f;
        p[i].vz   = dist(rng) * 0.1f;
        p[i].mass = std::abs(dist(rng)) + 1.f;
        p[i].life = std::abs(dist(rng));
        p[i].id   = static_cast<int>(i);
    }
    return p;
}

// ── SoA 版本（需要实现） ─────────────────────────────────

// TODO: 定义 ParticleSoA 结构体
// - 包含 std::vector<float> x, y, z, vx, vy, vz, mass, life
// - 包含 std::vector<int> id
// - 构造函数接受 size_t n，初始化所有 vector
struct ParticleSoA {
    // TODO: 添加成员
    // TODO: 添加构造函数
};

// TODO: 实现从 AoS 到 SoA 的转换函数
// ParticleSoA convertToSoA(const std::vector<ParticleAoS>& aos);

// ── 操作函数 ─────────────────────────────────────────────

// 已有: AoS 版粒子位置更新（只访问位置和速度）
void updatePositionAoS(std::vector<ParticleAoS>& p, float dt) {
    for (size_t i = 0; i < p.size(); ++i) {
        p[i].x += p[i].vx * dt;
        p[i].y += p[i].vy * dt;
        p[i].z += p[i].vz * dt;
    }
}

// 已有: AoS 版粒子生命衰减 + 质量缩放（只访问 life 和 mass）
void decayLifeAoS(std::vector<ParticleAoS>& p, float decay) {
    for (size_t i = 0; i < p.size(); ++i) {
        if (p[i].life > 0) {
            p[i].life -= decay;
        }
        p[i].mass *= 0.999f;
    }
}

// TODO: 实现 SoA 版粒子位置更新
// void updatePositionSoA(ParticleSoA& p, float dt);

// TODO: 实现 SoA 版粒子生命衰减 + 质量缩放
// void decayLifeSoA(ParticleSoA& p, float decay);

// ── 基准测试工具（已提供） ────────────────────────────────

template<typename F>
double benchmark(F&& func, int iterations) {
    func();  // 预热
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) func();
    auto end = std::chrono::steady_clock::now();
    auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return static_cast<double>(total.count()) / iterations;
}

template<typename T>
void doNotOptimize(T const& value) {
    asm volatile("" : : "r,m"(value) : "memory");
}

// ── main ──────────────────────────────────────────────────

int main() {
    constexpr size_t N = 1'000'000;
    constexpr int ITER = 20;
    constexpr float DT = 0.016f;
    constexpr float DECAY = 0.001f;

    std::cout << "========== AoS → SoA 转换练习 ==========\n";
    std::cout << "粒子数: " << N << " | 迭代: " << ITER << "\n\n";

    // 生成 AoS 数据
    auto aos = generateAoS(N);

    // TODO: 转换为 SoA
    // auto soa = convertToSoA(aos);

    // ── 基准 AoS ───────────────────────────────────────
    std::cout << "── AoS 版本 ──\n";

    double tAoS1 = benchmark([&] {
        updatePositionAoS(aos, DT);
        doNotOptimize(aos[0].x);
    }, ITER);
    std::cout << "  位置更新: " << std::fixed << std::setprecision(2)
              << tAoS1 << " us/iter\n";

    double tAoS2 = benchmark([&] {
        decayLifeAoS(aos, DECAY);
        doNotOptimize(aos[0].life);
    }, ITER);
    std::cout << "  生命衰减: " << tAoS2 << " us/iter\n";

    // TODO: 基准 SoA
    std::cout << "\n── SoA 版本 ──\n";

    // TODO: 调用 SoA 版本并输出时间
    // double tSoA1 = benchmark([&] { ... }, ITER);
    // double tSoA2 = benchmark([&] { ... }, ITER);

    std::cout << "\nTODO: 实现 SoA 版本后解除注释\n";
    std::cout << "预期: SoA 在部分字段访问时快 2-5×（更高缓存利用率）\n";
    std::cout << "当需要访问所有字段时（AoS 的最佳场景），SoA 仍可能略快\n";

    return 0;
}
