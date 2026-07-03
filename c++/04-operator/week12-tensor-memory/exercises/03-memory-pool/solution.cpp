// Exercise 03: Memory Pool — 完整解法

#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <new>
#include <iomanip>
#include <string>

class BumpAllocator {
public:
    explicit BumpAllocator(size_t capacityBytes)
        : capacity_(capacityBytes), offset_(0)
    {
        memory_ = static_cast<char*>(std::aligned_alloc(64, capacityBytes));
        if (!memory_) throw std::bad_alloc();
    }

    BumpAllocator(const BumpAllocator&) = delete;
    BumpAllocator& operator=(const BumpAllocator&) = delete;

    BumpAllocator(BumpAllocator&& other) noexcept
        : memory_(other.memory_), capacity_(other.capacity_), offset_(other.offset_)
    {
        other.memory_ = nullptr;
        other.capacity_ = 0;
        other.offset_ = 0;
    }

    BumpAllocator& operator=(BumpAllocator&& other) noexcept {
        if (this != &other) {
            std::free(memory_);
            memory_ = other.memory_;
            capacity_ = other.capacity_;
            offset_ = other.offset_;
            other.memory_ = nullptr;
            other.capacity_ = 0;
            other.offset_ = 0;
        }
        return *this;
    }

    void* allocate(size_t size) {
        size_t alignedSize = (size + 7) & ~7ULL;
        if (offset_ + alignedSize > capacity_) {
            std::cerr << "BumpAllocator: out of memory (requested " << size
                      << " bytes, available " << (capacity_ - offset_) << " bytes)\n";
            throw std::bad_alloc();
        }
        void* ptr = memory_ + offset_;
        offset_ += alignedSize;
        return ptr;
    }

    void deallocate(void*) {
        // Bump allocator: no-op, only reset() reclaims
    }

    void reset() {
        offset_ = 0;
    }

    size_t used() const { return offset_; }
    size_t capacity() const { return capacity_; }

    bool isAligned(const void* ptr, size_t align) const {
        return reinterpret_cast<std::uintptr_t>(ptr) % align == 0;
    }

    ~BumpAllocator() {
        std::free(memory_);
    }

private:
    char*  memory_ = nullptr;
    size_t capacity_ = 0;
    size_t offset_ = 0;
};

int main() {
    std::cout << "========== 内存池 (Bump Allocator) ==========\n\n";

    BumpAllocator pool(1024);

    std::cout << "── 初始状态 ──\n";
    std::cout << "  Capacity: " << pool.capacity() << " bytes\n";
    std::cout << "  Used:     " << pool.used() << " bytes\n";
    std::cout << "\n";

    std::cout << "── 连续分配 ──\n";
    int* a = static_cast<int*>(pool.allocate(sizeof(int)));
    *a = 42;
    std::cout << "  int*    a = " << static_cast<void*>(a)
              << "  64B对齐? " << (pool.isAligned(a, 64) ? "YES" : "NO")
              << "  *a = " << *a << "\n";

    double* b = static_cast<double*>(pool.allocate(sizeof(double)));
    *b = 3.14159;
    std::cout << "  double* b = " << static_cast<void*>(b)
              << "  64B对齐? " << (pool.isAligned(b, 64) ? "YES" : "NO")
              << "  *b = " << *b << "\n";

    char* c = static_cast<char*>(pool.allocate(100));
    std::strcpy(c, "Hello from pool!");
    std::cout << "  char*   c = " << static_cast<void*>(c)
              << "  64B对齐? " << (pool.isAligned(c, 64) ? "YES" : "NO")
              << "  c = \"" << c << "\"\n";

    std::cout << "  Used: " << pool.used() << " bytes\n\n";

    std::cout << "── reset() ──\n";
    pool.reset();
    std::cout << "  Used after reset: " << pool.used() << " bytes\n\n";

    std::cout << "── 重新分配 (256 floats) ──\n";
    float* d = static_cast<float*>(pool.allocate(256 * sizeof(float)));
    for (int i = 0; i < 256; ++i) d[i] = static_cast<float>(i) * 0.5f;
    std::cout << "  float* d = " << static_cast<void*>(d) << "\n";
    std::cout << "  d[0]=" << d[0] << "  d[255]=" << d[255] << "\n";
    std::cout << "  Used: " << pool.used() << " bytes (256*4="
              << (256 * sizeof(float)) << ")\n\n";

    std::cout << "── 测试容量耗尽 ──\n";
    try {
        pool.allocate(1024);
        std::cout << "  (不应到达此处)\n";
    } catch (const std::bad_alloc&) {
        std::cout << "  正确抛出 std::bad_alloc!\n";
    }

    std::cout << "\n========== 关键理解 ==========\n";
    std::cout << "1. Bump allocator: 只向前增长，不回收单个对象\n";
    std::cout << "2. allocate O(1): 指针偏移 + 对齐\n";
    std::cout << "3. reset O(1): 重置偏移到 0\n";
    std::cout << "4. 使用场景: 编译阶段、每请求生命周期、帧分配器\n";
    std::cout << "5. 对齐到 64 字节: 确保返回指针可用于 AVX-512\n";
    std::cout << "6. deallocate 是 no-op: 批量回收在 reset()\n";

    return 0;
}
