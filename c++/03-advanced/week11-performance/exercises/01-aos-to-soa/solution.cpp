// Exercise 01: AoS to SoA — 完整解法

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <random>
#include <cmath>
#include <numeric>
#include <algorithm>

struct ParticleAoS {
    float x, y, z;
    float vx, vy, vz;
    float mass;
    float life;
    int   id;
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

// ── SoA 定义 ────────────────────────────────────────────

struct ParticleSoA {
    std::vector<float> x, y, z;
    std::vector<float> vx, vy, vz;
    std::vector<float> mass;
    std::vector<float> life;
    std::vector<int>   id;

    explicit ParticleSoA(size_t n)
        : x(n), y(n), z(n)
        , vx(n), vy(n), vz(n)
        , mass(n), life(n), id(n) {}
};

ParticleSoA convertToSoA(const std::vector<ParticleAoS>& aos) {
    ParticleSoA soa(aos.size());
    for (size_t i = 0; i < aos.size(); ++i) {
        soa.x[i]    = aos[i].x;
        soa.y[i]    = aos[i].y;
        soa.z[i]    = aos[i].z;
        soa.vx[i]   = aos[i].vx;
        soa.vy[i]   = aos[i].vy;
        soa.vz[i]   = aos[i].vz;
        soa.mass[i] = aos[i].mass;
        soa.life[i] = aos[i].life;
        soa.id[i]   = aos[i].id;
    }
    return soa;
}

// ── AoS 操作 ────────────────────────────────────────────

void updatePositionAoS(std::vector<ParticleAoS>& p, float dt) {
    for (size_t i = 0; i < p.size(); ++i) {
        p[i].x += p[i].vx * dt;
        p[i].y += p[i].vy * dt;
        p[i].z += p[i].vz * dt;
    }
}

void decayLifeAoS(std::vector<ParticleAoS>& p, float decay) {
    for (size_t i = 0; i < p.size(); ++i) {
        if (p[i].life > 0) {
            p[i].life -= decay;
        }
        p[i].mass *= 0.999f;
    }
}

// ── SoA 操作 ────────────────────────────────────────────

void updatePositionSoA(ParticleSoA& p, float dt) {
    for (size_t i = 0; i < p.x.size(); ++i) {
        p.x[i] += p.vx[i] * dt;
        p.y[i] += p.vy[i] * dt;
        p.z[i] += p.vz[i] * dt;
    }
}

void decayLifeSoA(ParticleSoA& p, float decay) {
    for (size_t i = 0; i < p.life.size(); ++i) {
        if (p.life[i] > 0) {
            p.life[i] -= decay;
        }
        p.mass[i] *= 0.999f;
    }
}

// ── 基准测试 ────────────────────────────────────────────

template<typename F>
double benchmark(F&& func, int iterations) {
    func();
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

int main() {
    constexpr size_t N = 1'000'000;
    constexpr int ITER = 20;
    constexpr float DT = 0.016f;
    constexpr float DECAY = 0.001f;

    std::cout << "========== AoS → SoA 转换练习 ==========\n";
    std::cout << "粒子数: " << N << " | 迭代: " << ITER << "\n\n";

    auto aos = generateAoS(N);
    auto soa = convertToSoA(aos);

    std::cout << "sizeof(ParticleAoS) = " << sizeof(ParticleAoS) << " 字节\n";
    std::cout << "一条 cache line (64 字节) 可装 " << (64.0 / sizeof(ParticleAoS))
              << " 个 ParticleAoS\n\n";

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

    std::cout << "\n── SoA 版本 ──\n";
    double tSoA1 = benchmark([&] {
        updatePositionSoA(soa, DT);
        doNotOptimize(soa.x[0]);
    }, ITER);
    std::cout << "  位置更新: " << std::setprecision(2)
              << tSoA1 << " us/iter\n";

    double tSoA2 = benchmark([&] {
        decayLifeSoA(soa, DECAY);
        doNotOptimize(soa.life[0]);
    }, ITER);
    std::cout << "  生命衰减: " << tSoA2 << " us/iter\n";

    std::cout << "\n── 加速比 (AoS/SoA) ──\n";
    std::cout << "  位置更新: " << std::setprecision(2) << tAoS1 / tSoA1 << "x\n";
    std::cout << "  生命衰减: " << std::setprecision(2) << tAoS2 / tSoA2 << "x\n";

    return 0;
}
