// Example 04: Alignment & SIMD — 对齐控制与 SIMD 友好布局
// 目的：展示 alignas/alignof、伪共享消除、SIMD 数据布局

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <thread>
#include <atomic>
#include <random>
#include <cstring>

// ── 1. alignof / alignas 基础 ──────────────────────────────

void demoAlignment() {
    std::cout << "── alignof / alignas 基础 ──\n";

    std::cout << "  int:             alignof = " << alignof(int) << ", sizeof = " << sizeof(int) << '\n';
    std::cout << "  double:          alignof = " << alignof(double) << ", sizeof = " << sizeof(double) << '\n';
    std::cout << "  long double:     alignof = " << alignof(long double) << ", sizeof = " << sizeof(long double) << '\n';
    std::cout << "  void*:           alignof = " << alignof(void*) << ", sizeof = " << sizeof(void*) << '\n';
    std::cout << "  max_align_t:     alignof = " << alignof(std::max_align_t) << ", sizeof = " << sizeof(std::max_align_t) << '\n';

    struct DefaultAlign {
        char c;     // 1 字节
        int i;      // 4 字节 → 3 字节 padding
        double d;   // 8 字节
    };
    std::cout << "\n  struct DefaultAlign { char; int; double; }:\n";
    std::cout << "    sizeof = " << sizeof(DefaultAlign) << " (1 + 3pad + 4 + 8 = 16)\n";
    std::cout << "    alignof = " << alignof(DefaultAlign) << '\n';

    struct Reordered {
        double d;   // 8 字节（放最前面）
        int i;      // 4 字节
        char c;     // 1 字节 → 3 字节尾 padding
    };
    std::cout << "  struct Reordered { double; int; char; }:\n";
    std::cout << "    sizeof = " << sizeof(Reordered) << " (8 + 4 + 1 + 3pad = 16)\n";
    std::cout << "    alignof = " << alignof(Reordered) << '\n';

    struct alignas(64) OverAligned {
        int x;
    };
    std::cout << "\n  struct alignas(64) OverAligned { int; }:\n";
    std::cout << "    sizeof = " << sizeof(OverAligned) << " (64, 编译器填充到 64 倍数)\n";
    std::cout << "    alignof = " << alignof(OverAligned) << '\n';

    std::cout << '\n';
}

// ── 2. 伪共享 (false sharing) ──────────────────────────────

constexpr int FALSE_SHARING_ITERS = 100'000'000;

// 普通版本：a 和 b 在同一 cache line
struct NormalCounters {
    std::atomic<int64_t> a{0};
    std::atomic<int64_t> b{0};
    // a 和 b 距离 8 字节 → 肯定在同一条 64 字节 cache line
};

// 防伪共享版本：每个计数器占满一条 cache line
struct alignas(64) PaddedCounter {
    std::atomic<int64_t> value{0};
};

struct PaddedCounters {
    PaddedCounter a;
    PaddedCounter b;
};

template<typename F>
double benchmarkUs(F&& func, int iterations) {
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) func();
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
           / static_cast<double>(iterations);
}

void demoFalseSharing() {
    std::cout << "── 伪共享 (False Sharing) 演示 ──\n";
    std::cout << "每个线程递增计数器 " << FALSE_SHARING_ITERS << " 次\n\n";

    // 测试普通版本（有伪共享）
    NormalCounters nc;
    auto t1 = std::chrono::steady_clock::now();
    {
        std::thread t1([&] {
            for (int i = 0; i < FALSE_SHARING_ITERS; ++i)
                nc.a.fetch_add(1, std::memory_order_relaxed);
        });
        std::thread t2([&] {
            for (int i = 0; i < FALSE_SHARING_ITERS; ++i)
                nc.b.fetch_add(1, std::memory_order_relaxed);
        });
        t1.join(); t2.join();
    }
    auto t2 = std::chrono::steady_clock::now();
    auto normalTime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "  普通版本（同一 cache line）: " << normalTime << " ms\n";
    std::cout << "    地址: &a=" << &nc.a << ", &b=" << &nc.b
              << " (差 " << (reinterpret_cast<char*>(&nc.b) - reinterpret_cast<char*>(&nc.a)) << " 字节)\n";

    // 测试对齐版本（无伪共享）
    PaddedCounters pc;
    t1 = std::chrono::steady_clock::now();
    {
        std::thread t1([&] {
            for (int i = 0; i < FALSE_SHARING_ITERS; ++i)
                pc.a.value.fetch_add(1, std::memory_order_relaxed);
        });
        std::thread t2([&] {
            for (int i = 0; i < FALSE_SHARING_ITERS; ++i)
                pc.b.value.fetch_add(1, std::memory_order_relaxed);
        });
        t1.join(); t2.join();
    }
    t2 = std::chrono::steady_clock::now();
    auto paddedTime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "  对齐版本（不同 cache line）: " << paddedTime << " ms\n";
    std::cout << "    地址: &a=" << &pc.a << ", &b=" << &pc.b
              << " (差 " << (reinterpret_cast<char*>(&pc.b) - reinterpret_cast<char*>(&pc.a)) << " 字节)\n";

    std::cout << "  加速比: 普通/对齐 = "
              << std::fixed << std::setprecision(2)
              << static_cast<double>(normalTime) / paddedTime << "x\n";
    std::cout << "  结论: 对齐后消除伪共享，快 3-10×\n\n";
}

// ── 3. SIMD 友好布局 ───────────────────────────────────────

constexpr size_t SIMD_N = 1'000'000;

// AoS 布局 — SIMD 不友好
struct Vec3_AoS {
    float x, y, z;
};

// SoA 布局 — SIMD 友好
struct VecArray_SoA {
    std::vector<float> x, y, z;
    VecArray_SoA(size_t n) : x(n), y(n), z(n) {}

    void add(const VecArray_SoA& other) {
        for (size_t i = 0; i < x.size(); ++i) {
            x[i] += other.x[i];  // 连续访问 → 自动向量化
            y[i] += other.y[i];
            z[i] += other.z[i];
        }
    }
};

void demoSIMDLayout() {
    std::cout << "── SIMD 友好布局 (SoA) ──\n";

    // 初始化
    std::vector<Vec3_AoS> aos(SIMD_N);
    VecArray_SoA soa1(SIMD_N), soa2(SIMD_N);

    std::mt19937 rng(42);
    std::uniform_real_distribution<float> dist(-10.f, 10.f);
    for (size_t i = 0; i < SIMD_N; ++i) {
        float x = dist(rng), y = dist(rng), z = dist(rng);
        aos[i] = {x, y, z};
        soa1.x[i] = x; soa1.y[i] = y; soa1.z[i] = z;
        soa2.x[i] = dist(rng); soa2.y[i] = dist(rng); soa2.z[i] = dist(rng);
    }

    // AoS 向量加法
    auto t1 = std::chrono::steady_clock::now();
    for (size_t i = 0; i < SIMD_N; ++i) {
        aos[i].x += 1.0f;
        aos[i].y += 1.0f;
        aos[i].z += 1.0f;
    }
    asm volatile("" : : "r"(&aos[0]) : "memory");
    auto t2 = std::chrono::steady_clock::now();

    // SoA 向量加法
    auto t3 = std::chrono::steady_clock::now();
    for (size_t i = 0; i < SIMD_N; ++i) {
        soa1.x[i] += 1.0f;
        soa1.y[i] += 1.0f;
        soa1.z[i] += 1.0f;
    }
    asm volatile("" : : "r"(&soa1.x[0]) : "memory");
    auto t4 = std::chrono::steady_clock::now();

    auto aosUs = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto soaUs = std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count();

    std::cout << "  AoS (vec3 数组, N=" << SIMD_N << "): "
              << aosUs << " us\n";
    std::cout << "    布局: [x0,y0,z0] [x1,y1,z1] ...\n";
    std::cout << "    SIMD: 需要 gather/scatter 拆分 x,y,z → 无法自动向量化\n\n";

    std::cout << "  SoA (3×float 数组, N=" << SIMD_N << "): "
              << soaUs << " us\n";
    std::cout << "    布局: [x0,x1,x2,...] [y0,y1,y2,...] [z0,z1,z2,...]\n";
    std::cout << "    SIMD: 连续 float → load 128/256/512 bits 直接向量加法\n\n";

    std::cout << "  加速比: AoS/SoA = "
              << std::fixed << std::setprecision(2)
              << static_cast<double>(aosUs) / soaUs << "x\n";

    std::cout << "\n  SIMD 对齐要求:\n";
    std::cout << "    SSE  (__m128):    alignas(16)\n";
    std::cout << "    AVX  (__m256):    alignas(32)\n";
    std::cout << "    AVX-512 (__m512): alignas(64)\n";
    std::cout << "    未对齐访问: 有性能惩罚（-O3 -march=native 可减轻）\n\n";
}

// ── main ──────────────────────────────────────────────────

int main() {
    std::cout << "========== 对齐与 SIMD 布局 ==========\n\n";

    demoAlignment();
    demoFalseSharing();
    demoSIMDLayout();

    std::cout << "========== 要点总结 ==========\n";
    std::cout << "1. alignof 查询对齐，alignas 指定对齐\n";
    std::cout << "2. 成员顺序影响 sizeof（大对齐类型放前面）\n";
    std::cout << "3. 伪共享：多线程写相邻变量 → 填充到不同 cache line\n";
    std::cout << "4. SIMD 需要连续内存 → SoA 布局\n";
    std::cout << "5. SSE/AVX 需对齐 load（_mm_load_ps vs _mm_loadu_ps）\n";

    return 0;
}
