// Exercise 02: Benchmark Macro — 实现便捷的基准测试宏
// TODO: 实现 BENCHMARK 宏，简化微基准测试

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cmath>
#include <numeric>

// ── 辅助工具 ──────────────────────────────────────────────

template<typename T>
void doNotOptimize(T const& value) {
    asm volatile("" : : "r,m"(value) : "memory");
}

// ── TODO: 实现 BENCHMARK 宏 ────────────────────────────────
//
// 用法：BENCHMARK(block, iterations, "description")
// 示例：
//   BENCHMARK({
//     int s = 0;
//     for (int i = 0; i < 1000; ++i) s += i;
//     doNotOptimize(s);
//   }, 100, "sum 1..1000");
//
// 输出格式: [description]  平均: XXX ns  (N iters)
//
// 要求：
// 1. 预热 1 次
// 2. 执行 iterations 次
// 3. 用 steady_clock 计时
// 4. 输出纳秒精度平均值

#define BENCHMARK(block, iterations, description) \
    /* TODO: 实现基准测试宏 */                     \
    /* 提示: 用 do { ... } while(0) 包装多语句宏 */ \
    std::cout << "[TODO] " << description << '\n';

// ── TODO（进阶）: 实现 BENCHMARK_MULTI 宏 ──────────────────
//
// 用法：BENCHMARK_MULTI(block, warmup, measure, "description")
// - warmup 次预热，measure 次测量
// - 输出 平均/中位数/最小值
// - 示例：
//   BENCHMARK_MULTI({ ... }, 3, 10, "my test");

#define BENCHMARK_MULTI(block, warmup, measure, description) \
    /* TODO: 实现带统计的基准测试宏 */                        \
    std::cout << "[TODO] " << description << '\n';

// ── 被测函数 ──────────────────────────────────────────────

constexpr int WORK_SIZE = 10000;

// 函数1: vector push_back vs reserve
void vectorNoReserve() {
    std::vector<int> v;
    for (int i = 0; i < WORK_SIZE; ++i) {
        v.push_back(i);
    }
    doNotOptimize(v.data());
}

void vectorWithReserve() {
    std::vector<int> v;
    v.reserve(WORK_SIZE);
    for (int i = 0; i < WORK_SIZE; ++i) {
        v.push_back(i);
    }
    doNotOptimize(v.data());
}

// 函数2: 遍历顺序 row-major vs column-major
constexpr int MAT_SIZE = 2048;
static int matrix[MAT_SIZE][MAT_SIZE];  // NOLINT

void initMatrix() {
    for (int i = 0; i < MAT_SIZE; ++i)
        for (int j = 0; j < MAT_SIZE; ++j)
            matrix[i][j] = i + j;
}

int rowMajorSum() {
    int sum = 0;
    for (int i = 0; i < MAT_SIZE; ++i)
        for (int j = 0; j < MAT_SIZE; ++j)
            sum += matrix[i][j];  // 连续访问 → 空间局部性好
    return sum;
}

int columnMajorSum() {
    int sum = 0;
    for (int j = 0; j < MAT_SIZE; ++j)
        for (int i = 0; i < MAT_SIZE; ++i)
            sum += matrix[i][j];  // 跨行访问 → cache miss 多
    return sum;
}

// ── main ──────────────────────────────────────────────────

int main() {
    std::cout << "========== BENCHMARK 宏练习 ==========\n\n";

    // TODO: 用你的 BENCHMARK 宏测试以下函数
    initMatrix();

    std::cout << "── 测试1: vector::reserve 效果 ──\n";
    BENCHMARK({ vectorNoReserve(); }, 100, "push_back 无 reserve");
    BENCHMARK({ vectorWithReserve(); }, 100, "push_back 有 reserve");
    // 预期: reserve 版本显著更快（避免多次 realloc + 拷贝）

    std::cout << "\n── 测试2: Row-major vs Column-major ──\n";
    BENCHMARK({ doNotOptimize(rowMajorSum()); }, 10, "row-major 求和");
    BENCHMARK({ doNotOptimize(columnMajorSum()); }, 10, "column-major 求和");
    // 预期: row-major 快 10-50×（连续访问 vs 跨大步访问）

    std::cout << "\n── 测试3 (进阶): BENCHMARK_MULTI 带统计 ──\n";
    BENCHMARK_MULTI({ doNotOptimize(rowMajorSum()); }, 2, 5, "row-major (带统计)");

    std::cout << "\nTODO: 实现完整宏定义后查看实际时间\n";

    return 0;
}
