// Exercise 03: Memory Pool — 简单内存池
// TODO: 实现一个预分配大块 + 原地增长的 BumpAllocator

#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <new>
#include <iomanip>
#include <string>

// TODO: 实现 BumpAllocator 类
// 要求:
//   1. 构造函数 BumpAllocator(size_t capacityBytes)
//      - 使用 std::aligned_alloc 分配 capacityBytes 内存
//      - 对齐到 64 字节 (方便 AVX-512)
//   2. allocate(size_t bytes) → void*
//      - 返回当前空闲区起始指针
//      - 将空闲区指针向前移动
//      - 需将 bytes 对齐到 8 字节
//      - 若空间不足: 打印错误或 throw std::bad_alloc
//   3. deallocate(void* ptr) → 本练习中不做回收 (bump allocator)
//   4. reset() → 重置分配指针到起始位置 (所有分配"释放")
//   5. used() / capacity() → 返回已用和总容量
//   6. ~BumpAllocator() → 释放底层内存

class BumpAllocator {
public:
    // TODO: 添加构造函数
    // explicit BumpAllocator(size_t capacityBytes);

    // TODO: 禁用拷贝（管理独占资源）
    // BumpAllocator(const BumpAllocator&) = delete;
    // BumpAllocator& operator=(const BumpAllocator&) = delete;

    // TODO: 分配内存
    // void* allocate(size_t size);

    // TODO: 释放（占位，不真正回收）
    // void deallocate(void* ptr);

    // TODO: 重置
    // void reset();

    // TODO: 统计
    // size_t used() const;
    // size_t capacity() const;

    // TODO: 析构
    // ~BumpAllocator();

private:
    // TODO: 添加成员变量
};

// ── 分配器适配器（集成到 std::vector） ──────────────────
// TODO: 实现一个简单的分配器模板，内部使用 BumpAllocator&
// 注意: 这需要处理 rebind 和跨类型比较

// ── main（验证） ─────────────────────────────────────────

int main() {
    std::cout << "========== 内存池练习 ==========\n\n";

    // TODO: 创建内存池
    // BumpAllocator pool(1024);  // 1024 字节

    // TODO: 打印初始状态
    // std::cout << "Capacity: " << pool.capacity() << " bytes\n";
    // std::cout << "Used:     " << pool.used() << " bytes\n\n";

    // TODO: 分配几个小对象
    // int* a = static_cast<int*>(pool.allocate(sizeof(int)));
    // *a = 42;
    // double* b = static_cast<double*>(pool.allocate(sizeof(double)));
    // *b = 3.14;
    // char* c = static_cast<char*>(pool.allocate(100));
    // std::strcpy(c, "Hello from pool!");

    // TODO: 验证值和 used
    // std::cout << "After allocations:\n";
    // std::cout << "  *a = " << *a << "\n";
    // std::cout << "  *b = " << *b << "\n";
    // std::cout << "   c = " << c << "\n";
    // std::cout << "  used: " << pool.used() << " bytes\n\n";

    // TODO: reset 后重新分配
    // pool.reset();
    // float* d = static_cast<float*>(pool.allocate(256 * sizeof(float)));
    // std::cout << "After reset, allocated 256 floats\n";
    // std::cout << "  used: " << pool.used() << " bytes\n";

    // TODO: 测试耗尽
    // try { pool.allocate(1024); } catch (...) { ... }

    std::cout << "TODO: 实现 BumpAllocator 后解除注释\n";
    std::cout << "验证点:\n";
    std::cout << "  1. allocate 返回对齐指针 (64字节对齐)\n";
    std::cout << "  2. 多次 allocate 返回不重叠的内存\n";
    std::cout << "  3. reset() 后 used() == 0\n";
    std::cout << "  4. 容量耗尽时正确处理\n";
    std::cout << "  5. 析构函数正确释放底层内存\n";

    return 0;
}
