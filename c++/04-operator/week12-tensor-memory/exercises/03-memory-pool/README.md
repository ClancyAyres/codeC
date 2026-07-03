# 习题 03：Memory Pool — 简单内存池

## 目标

实现一个 Bump Allocator（原地增长分配器），理解内存池如何减少频繁 `new`/`delete` 的开销。

## 要求

1. **`BumpAllocator(size_t capacity)`**：预分配 capacity 字节内存（64 字节对齐）
2. **`allocate(size_t)`**：返回当前空闲区指针，向前移动偏移（O(1)）；空间不足时 throw `std::bad_alloc`
3. **`deallocate(void*)`**：不回收（bump allocator 特性）
4. **`reset()`**：重置偏移到 0，回收所有分配
5. **`used()` / `capacity()`**：返回已用和总容量
6. **禁用拷贝**，允许移动

## 提示

- 使用 `std::aligned_alloc(64, capacity)` 分配初始内存
- `allocate()` 中将 size 对齐到 8 字节：`(size + 7) & ~7ULL`
- `reset()` 只需 `offset_ = 0`，不需要清空数据
- Bump allocator 不能单独释放对象，只能批量 reset

## 预期结果

- 多次 allocate 返回不重叠的指针
- allocate 返回的指针 64 字节对齐
- reset() 后 used() == 0，可重新分配
- 耗尽容量时正确抛出异常
- 析构函数释放底层内存
