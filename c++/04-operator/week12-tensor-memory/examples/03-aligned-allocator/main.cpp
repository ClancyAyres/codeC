// Example 03: Aligned Allocator — alignas + aligned_alloc + 对齐分配器
// 目的：理解内存对齐的硬件原因、aligned_alloc 用法，以及如何构建一个对齐分配器

#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <iomanip>

// ── 对齐分配器 ─────────────────────────────────────────────
template<typename T, std::size_t Alignment = 64>
struct AlignedAllocator {
    using value_type = T;

    AlignedAllocator() = default;

    template<typename U>
    AlignedAllocator(const AlignedAllocator<U, Alignment>&) {}

    template<typename U>
    struct rebind {
        using other = AlignedAllocator<U, Alignment>;
    };

    T* allocate(std::size_t n) {
        std::size_t size = n * sizeof(T);
        std::size_t alignedSize = size;
        if (alignedSize % Alignment != 0) {
            alignedSize = ((alignedSize / Alignment) + 1) * Alignment;
        }
        void* ptr = std::aligned_alloc(Alignment, alignedSize);
        if (!ptr) throw std::bad_alloc();
        return static_cast<T*>(ptr);
    }

    void deallocate(T* p, std::size_t) {
        std::free(p);
    }

    template<typename U>
    bool operator==(const AlignedAllocator<U, Alignment>&) const { return true; }
    template<typename U>
    bool operator!=(const AlignedAllocator<U, Alignment>&) const { return false; }
};

// ── 检查指针是否对齐 ────────────────────────────────────────
bool isAligned(const void* ptr, std::size_t alignment) {
    return reinterpret_cast<std::uintptr_t>(ptr) % alignment == 0;
}

// ── 对齐包装器: 确保栈上和堆上都可以对齐 ────────────────────
template<typename T, std::size_t Alignment = 64>
struct alignas(Alignment) AlignedValue {
    T value;

    AlignedValue() : value{} {}
    explicit AlignedValue(T v) : value(v) {}
    operator T&() { return value; }
    operator const T&() const { return value; }
};

// ── 不使用对齐的 vector 分配测试 ────────────────────────────
template<typename T>
void testUnalignedVector(size_t n, const char* label) {
    std::vector<T> vec(n);
    void* ptr = static_cast<void*>(vec.data());
    std::cout << "  " << label << ": ptr=" << ptr
              << " alignof(T)=" << alignof(T)
              << " 64B对齐? " << (isAligned(ptr, 64) ? "YES" : "NO")
              << '\n';
}

// ── 使用对齐分配器的 vector 分配测试 ────────────────────────
template<typename T, std::size_t Align>
void testAlignedVector(size_t n, const char* label) {
    std::vector<T, AlignedAllocator<T, Align>> vec(n);
    void* ptr = static_cast<void*>(vec.data());
    std::cout << "  " << label << ": ptr=" << ptr
              << " alignof=" << Align
              << " 对齐? " << (isAligned(ptr, Align) ? "YES" : "NO")
              << '\n';
}

int main() {
    std::cout << "========== 对齐分配器演示 ==========\n\n";

    // ── 基础对齐知识 ──────────────────────────────────────
    std::cout << "── 基础对齐 ──\n";
    std::cout << "  alignof(char)       = " << alignof(char) << '\n';
    std::cout << "  alignof(int)        = " << alignof(int) << '\n';
    std::cout << "  alignof(double)     = " << alignof(double) << '\n';
    std::cout << "  alignof(long double)= " << alignof(long double) << '\n';
    std::cout << "  alignof(void*)      = " << alignof(void*) << '\n';
    std::cout << "  alignof(std::max_align_t) = " << alignof(std::max_align_t) << '\n';
    std::cout << "  alignof(AlignedValue<int, 64>) = " << alignof(AlignedValue<int, 64>) << '\n';

    // ── alignas 栈上对齐 ──────────────────────────────────
    std::cout << "\n── alignas 栈上对齐 ──\n";
    alignas(64) int stack_aligned_int = 42;
    std::cout << "  alignas(64) int: ptr=" << &stack_aligned_int
              << " 对齐? " << (isAligned(&stack_aligned_int, 64) ? "YES" : "NO") << '\n';

    AlignedValue<int, 64> aligned_val(123);
    std::cout << "  AlignedValue<int,64>: ptr=" << &aligned_val
              << " 对齐? " << (isAligned(&aligned_val, 64) ? "YES" : "NO") << '\n';

    // ── aligned_alloc 堆上对齐 ────────────────────────────
    std::cout << "\n── aligned_alloc 堆上对齐 ──\n";
    void* alignedPtr = std::aligned_alloc(64, 256);
    std::cout << "  aligned_alloc(64, 256): ptr=" << alignedPtr
              << " 对齐? " << (isAligned(alignedPtr, 64) ? "YES" : "NO") << '\n';
    std::free(alignedPtr);

    void* alignedPtr2 = std::aligned_alloc(32, 128);
    std::cout << "  aligned_alloc(32, 128): ptr=" << alignedPtr2
              << " 对齐? " << (isAligned(alignedPtr2, 32) ? "YES" : "NO") << '\n';
    std::free(alignedPtr2);

    // ── 对比: 普通 vector vs 对齐 vector ─────────────────
    std::cout << "\n── 普通 vector (default allocator) ──\n";
    testUnalignedVector<int>(16, "vector<int>(16)");
    testUnalignedVector<double>(16, "vector<double>(16)");
    testUnalignedVector<float>(64, "vector<float>(64)");

    std::cout << "\n── 对齐 vector (AlignedAllocator<T, 64>) ──\n";
    constexpr size_t N = 64;
    testAlignedVector<int, 64>(N, "vector<int> 64B对齐");
    testAlignedVector<double, 64>(N, "vector<double> 64B对齐");
    testAlignedVector<float, 64>(N, "vector<float> 64B对齐");
    testAlignedVector<int, 16>(N, "vector<int> 16B对齐");
    testAlignedVector<double, 32>(N, "vector<double> 32B对齐");

    // ── 对齐对 SIMD 的意义（概念演示） ────────────────────
    std::cout << "\n── 为什么对齐对 SIMD 重要 ──\n";
    std::cout << "  SSE (128-bit) : 需要 16 字节对齐 → _mm_load_ps  (aligned load, 1 cycle)\n";
    std::cout << "  AVX (256-bit) : 需要 32 字节对齐 → _mm256_load_ps (aligned load, 1 cycle)\n";
    std::cout << "  AVX-512 (512-bit): 64 字节对齐 → _mm512_load_ps\n";
    std::cout << "  未对齐加载: _mm_loadu_ps — 可能跨 cache line，耗时更长\n";
    std::cout << "  对齐保证: 一条 cache line (64B) 内数据从 0 开始 → 不跨 cache line\n";

    // ── false sharing 说明 ────────────────────────────────
    std::cout << "\n── alignas 防止 false sharing ──\n";
    struct PaddedCounter {
        alignas(64) int value = 0;
    };
    std::cout << "  sizeof(PaddedCounter) = " << sizeof(PaddedCounter) << '\n';
    std::cout << "  alignof(PaddedCounter) = " << alignof(PaddedCounter) << '\n';
    std::cout << "  两个 PaddedCounter 在不同 cache line → 多线程不互相 invalidate\n";

    // ── 跨页对齐说明 ──────────────────────────────────────
    std::cout << "\n── 对齐与页 (page) ──\n";
    std::cout << "  页大小: 通常 4096 字节\n";
    std::cout << "  64 字节对齐 → 数据在 1 个 cache line 内 (cache line = 64 字节)\n";
    std::cout << "  若数据不对齐 → 可能跨 page → 两次 TLB 查找\n";

    // ── 分配器比较表 ──────────────────────────────────────
    std::cout << "\n── 分配器对比 ──\n";
    std::cout << "  1. ::operator new    — 通用，对齐到 max_align_t (通常 16 字节)\n";
    std::cout << "  2. std::aligned_alloc — C++17，指定对齐 (size 必须是对齐倍数)\n";
    std::cout << "  3. posix_memalign    — POSIX，不限 size\n";
    std::cout << "  4. _mm_malloc        — Intel，对应 _mm_free\n";
    std::cout << "  5. AlignedAllocator  — 自定义，可配合 std::vector\n";

    std::cout << "\n========== 完成 ==========\n";
    return 0;
}
