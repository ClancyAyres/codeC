# Week 12 知识点笔记：张量的内存模型

## 一、行优先 (Row-Major) vs 列优先 (Column-Major)

### 1.1 概念本质

行优先和列优先描述的是多维数组在**一维线性内存**中的排布方式。计算机内存是一维的字节序列，多维张量必须降维成一维才能存储。

```
┌─────────────────────────────────────────────────────────┐
│  逻辑张量 (2×3×4)                                        │
│                                                         │
│  dim0=0:                                                │
│    dim1=0: [0,0,0] [0,0,1] [0,0,2] [0,0,3]             │
│    dim1=1: [0,1,0] [0,1,1] [0,1,2] [0,1,3]             │
│    dim1=2: [0,2,0] [0,2,1] [0,2,2] [0,2,3]             │
│  dim0=1:                                                │
│    dim1=0: [1,0,0] [1,0,1] [1,0,2] [1,0,3]             │
│    dim1=1: [1,1,0] [1,1,1] [1,1,2] [1,1,3]             │
│    dim1=2: [1,2,0] [1,2,1] [1,2,2] [1,2,3]             │
└─────────────────────────────────────────────────────────┘
```

**Row-Major（行优先，C-order）**：
```
内存: [0,0,0] [0,0,1] [0,0,2] [0,0,3] [0,1,0] [0,1,1] ... [1,2,3]
      └─────────── dim2 连续 ──────────┘
      └─────────────────── dim1=0 全部 ──────────────────┘
      └────────────────────── dim0=0 全部 ───────────────────────┘
```
- 最后维（最内层）的元素在内存中相邻
- C/C++、Python、NumPy、PyTorch 默认
- 遍历最后维最快（`for k: for j: for i:` 或 `for i: for j: for k:` 中的内层循环遍历 k）

**Column-Major（列优先，Fortran-order）**：
```
内存: [0,0,0] [1,0,0] [0,1,0] [1,1,0] [0,2,0] [1,2,0] [0,0,1] ... [1,2,3]
      └──── dim0 连续 ────┘
      └─────────── dim1=0 全部 ──────────┘
      └────────────────── dim2=0 全部 ──────────────────┘
```
- 第一维（最外层逻辑上的最内层物理上）的元素在内存中相邻
- Fortran、MATLAB、R、BLAS/LAPACK 内部
- 遍历第一维最快

### 1.2 通用线性索引公式

**Row-Major (N 维)**：
```
linear = offset + Σ_{d=0}^{N-1} index[d] * stride[d]

其中 stride[d] = Π_{k=d+1}^{N-1} shape[k]   （默认连续 case）
```

推导示例（3D, shape = [D, H, W]）：
```
stride[0] = H * W      // dim0 移动1步 = 跳过整个 dim1×dim2 平面
stride[1] = W           // dim1 移动1步 = 跳过一行
stride[2] = 1           // dim2 移动1步 = 下一个元素

linear = offset + i * (H*W) + j * W + k
```

**Column-Major (N 维)**：
```
linear = offset + Σ_{d=0}^{N-1} index[d] * stride[d]

其中 stride[d] = Π_{k=0}^{d-1} shape[k]   （默认连续 case）
```

推导示例（3D, shape = [D, H, W]）：
```
stride[0] = 1           // dim0 移动1步 = 下一个元素
stride[1] = D            // dim1 移动1步 = 跳过 dim0 整段
stride[2] = D * H       // dim2 移动1步 = 跳过 dim0×dim1 整个平面

linear = offset + i + j * D + k * (D*H)
```

### 1.3 性能影响

```cpp
// 假设 shape = [1024, 1024] 的矩阵，row-major 存储
// 好的访问模式（逐行遍历，内层循环沿连续方向）：
for (int i = 0; i < 1024; ++i)       // 外层: 慢变维
    for (int j = 0; j < 1024; ++j)   // 内层: 快变维 (j=cols, stride=1)
        sum += matrix[i * 1024 + j]; // 连续: 每次访问下一个内存地址 → cache友好

// 坏的访问模式（逐列遍历，内层循环跨大步长）：
for (int j = 0; j < 1024; ++j)        // 外层: 快变维
    for (int i = 0; i < 1024; ++i)    // 内层: 慢变维 (i变化时内存跳跃1024个元素)
        sum += matrix[i * 1024 + j]; // 跨步: 每次跳跃4096字节 → cache miss

// 差距: 10-100倍（取决于矩阵大小 vs cache 大小）
```

### 1.4 循环顺序与缓存

| 遍历顺序 | Row-Major 存储 | Column-Major 存储 |
|----------|---------------|-------------------|
| `for i: for j: for k:` | 好（k连续） | 差（i需在k的最内层） |
| `for k: for j: for i:` | 差（i在外层跨越） | 好（i连续） |
| `for i: for k: for j:` | 中等 | 差 |
| 矩阵乘 `C(i,j) += A(i,k) * B(k,j)` | A 逐行好, B 逐列差 | A 逐行差, B 逐列好 |

## 二、Stride 语义

### 2.1 什么是 Stride

Stride 是**每个维度上移动一个逻辑位置，需要在物理内存中跳过的元素个数**。

```cpp
// 传统: stride 由 shape 隐式推导
// shape=[3, 4] → strides=[4, 1] (row-major)
// 访问 (i, j) → offset + i*4 + j*1

// Stride 张量: stride 是独立元数据，可以任意指定
// shape=[3, 4], strides=[8, 2] → 数据有"空洞"
// 访问 (i, j) → offset + i*8 + j*2
// 物理存储可能 > 逻辑大小: 可用作 aligned padding 或 batch 切片的复用
```

### 2.2 Stride 支持的操作

```cpp
// 1. 切片 (Slice): 不拷贝，只改 offset 和 shape
//    原张量: shape=[10, 20], strides=[20, 1], offset=0
//    slice [2:5, :] → shape=[3, 20], offset=2*20=40, strides 不变
//    物理数据完全相同，仅逻辑视图变化

// 2. 转置 (Transpose): 交换 strides
//    原张量: shape=[3, 4], strides=[4, 1]
//    T → shape=[4, 3], strides=[1, 4]
//    访问 T(j,i) → offset + j*1 + i*4 = 原张量(i,j) 的同一位置!
//    没有任何数据移动

// 3. 广播 (Broadcast): stride=0 表示该维"复用同一个值"
//    原张量: shape=[1, 5], strides=[5, 1]
//    broadcast dim0 到 3: shape=[3, 5], strides=[0, 1]
//    访问 (i,j): offset + i*0 + j*1 = offset + j
//    结果: i=0,1,2 都访问同样的 5 个值 → 零拷贝广播！

// 4. 维度置换 (Permute/Transpose N-D): 重排 strides 和 shape
//    原张量: shape=[2, 3, 4], strides=[12, 4, 1]
//    permute(2, 0, 1) → shape=[4, 2, 3], strides=[1, 12, 4]
//    同样零拷贝

// 5. 对角线 (Diag): offset + stride 组合
//    shape=[3, 3], strides=[5, 5] + offset=0 → 对角线视图
//    只有 shape[0] 个有效元素，却用 2D 索引访问
```

### 2.3 连续性 (Contiguity)

连续张量是最常见的情况：内存中没有任何空洞，stride 恰好是 shape 的累积积。

```cpp
// 判定连续 (C-order / row-major contiguous):
bool isContiguous(const int64_t* shape, const int64_t* strides, int ndim) {
    int64_t expected = 1;
    for (int d = ndim - 1; d >= 0; --d) {
        if (shape[d] != 1 && strides[d] != expected)
            return false;
        expected *= shape[d];
    }
    return true;
}

// 为什么需要 contiguous():
// - BLAS/LAPACK 等外部库要求连续内存（它们用 stride=1 的最内层优化）
// - 自定义 kernel 可能要求连续（简化索引逻辑）
// - reshape 操作要求连续（才能安全改变 shape 而不破坏数据顺序）
// - 向 GPU 拷贝时（cudaMemcpy 按行拷贝，不连续则需逐元素）
```

## 三、零拷贝切片

### 3.1 原理

传统深拷贝切片：
```
原数据: [a, b, c, d, e, f, g, h, i, j]
slice [2:5]: 新分配 [c, d, e] — 3次拷贝
```

零拷贝切片：
```
原数据: [a, b, c, d, e, f, g, h, i, j]  ← 共享同一块内存
         ↑        ↑
视图1:  slice [0:3] = [a, b, c]           ← offset=0, shape=[3], strides=[1]
视图2:  slice [2:5] = [c, d, e]           ← offset=2, shape=[3], strides=[1]
视图3:  slice [5:8] = [f, g, h]           ← offset=5, shape=[3], strides=[1]
```

```cpp
// 核心数据结构
template<typename T>
struct Tensor {
    T* data_;                          // 共享的数据指针（非拥有）
    std::shared_ptr<T[]> storage_;     // 真正的数据所有者（引用计数）
    std::vector<int64_t> shape_;
    std::vector<int64_t> strides_;
    int64_t offset_;

    // 零拷贝切片
    Tensor slice(int64_t dim, int64_t start, int64_t end) const {
        Tensor view = *this;           // 复制元数据（轻量）
        view.offset_ += start * strides_[dim];
        view.shape_[dim] = end - start;
        return view;                   // 共享 storage_，仅 O(ndim) 开销
    }
};
```

### 3.2 优势与风险

| 优势 | 风险 |
|------|------|
| O(1) 创建（仅改元数据） | 修改视图会影响原始数据 |
| 零额外内存 | 生命周期：原数据释放后视图悬挂 |
| 管道友好（多个中间结果不拷贝） | 非连续视图传给 BLAS 需先 contiguous() |
| 编译器可优化掉中间张量 | 多线程下需同步 |

### 3.3 生命周期管理

```cpp
// 方案1: shared_ptr 引用计数（推荐）
std::shared_ptr<T[]> storage_ = std::make_shared<T[]>(size);
// 所有 view 共享同一个 storage_，最后一个释放后自动 free

// 方案2: 原始指针 + 手动管理（危险）
T* data_ = new T[size];  // 谁负责 delete? 何时释放?
// 需要引用计数或明确的所有权语义

// 方案3: unique_ptr + 禁止拷贝视图（过于严格）
std::unique_ptr<T[]> storage_;
// 视图不能共享，只能用索引范围 + 不创建对象
```

## 四、自定义 Allocator

### 4.1 C++ Allocator 接口

```cpp
// C++17 标准 Allocator 最小接口
template<typename T>
struct MyAllocator {
    using value_type = T;

    MyAllocator() = default;
    template<typename U> MyAllocator(const MyAllocator<U>&) {}

    T* allocate(std::size_t n) {
        // 分配 n 个 T 的内存
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t n) {
        // 释放 n 个 T 的内存
        ::operator delete(p);
    }
};

// 满足标准 Allocator 要求的额外声明
template<typename T, typename U>
bool operator==(const MyAllocator<T>&, const MyAllocator<U>&) { return true; }
template<typename T, typename U>
bool operator!=(const MyAllocator<T>&, const MyAllocator<U>&) { return false; }

// 可用于 STL 容器：
std::vector<int, MyAllocator<int>> vec;
```

### 4.2 对齐分配器

```cpp
// 需要 aligned_alloc 支持（C++17）或 posix_memalign（POSIX）
template<typename T, std::size_t Alignment = 64>
struct AlignedAllocator {
    using value_type = T;

    T* allocate(std::size_t n) {
        std::size_t size = n * sizeof(T);
        // 确保 size 是 Alignment 的倍数（C++17 aligned_alloc 要求）
        if (size % Alignment != 0)
            size = ((size / Alignment) + 1) * Alignment;

        void* ptr = std::aligned_alloc(Alignment, size);
        if (!ptr) throw std::bad_alloc();
        return static_cast<T*>(ptr);
    }

    void deallocate(T* p, std::size_t) {
        std::free(p);  // aligned_alloc 必须用 free 释放
    }
};

// 使用:
std::vector<double, AlignedAllocator<double, 64>> vec;
// vec.data() 现在是对齐到 64 字节边界的指针
// 适合 AVX-512 (64 字节) 直接加载
```

### 4.3 C++17 的 `operator new` 对齐重载

```cpp
// C++17 起，编译器自动为 alignas 类型选择对齐的 operator new:
struct alignas(64) AlignedFloat {
    float value;
};
auto* p = new AlignedFloat[100];  // 自动调用 operator new[](size, align_val_t{64})

// 手动指定对齐:
void* ptr = operator new(1024, std::align_val_t(64));
operator delete(ptr, std::align_val_t(64));

// 对于 std::vector<T> 中的 alignas 类型，需自定义 allocator
// （因为 std::allocator 默认用未对齐的 ::operator new）
```

## 五、内存池概念

### 5.1 为什么需要内存池

```cpp
// 频繁 new/delete 的问题:
for (int i = 0; i < 1000000; ++i) {
    auto* p = new float[1024];   // 系统调用 → 内核态 → 页表操作
    // ... 使用 ...
    delete[] p;                   // 系统调用 → 归还给 OS
}
// 每次 new/delete 都是昂贵的内核态切换

// 内存池方案:
MemoryPool pool(1024 * sizeof(float) * 1024);   // 预分配大块
for (int i = 0; i < 1000000; ++i) {
    auto* p = pool.allocate(1024 * sizeof(float));  // 指针偏移 (O(1), 用户态)
    // ... 使用 ...
    pool.deallocate(p);                            // 可惰性（或什么都不做）
}
pool.reset();  // 批量释放
```

### 5.2 内存池设计

```
┌─────────────────────────────────────────────┐
│              内存池 (MemoryPool)              │
│                                              │
│  ┌──────────────────────────────────────┐   │
│  │ 大块内存 (通过 mmap/sbrk 预先分配)      │   │
│  │                                      │   │
│  │  [已分配] [已分配] [空闲] [空闲] [空闲]  │   │
│  │           ↑                          │   │
│  │         next_ 指针                    │   │
│  │                                      │   │
│  │  allocate(n) → 返回 next_, next_ += n │   │
│  │  reset()     → next_ = start         │   │
│  └──────────────────────────────────────┘   │
│                                              │
│  策略类型:                                    │
│  - Bump allocator:   只增不改, 适合阶段生命期 │
│  - Free list:        释放链表, 可重用     │
│  - Slab allocator:   固定大小块, 极致速度  │
│  - Arena allocator:  区域分配, 批量回收      │
│  - Pool allocator:   多尺寸池, 通用         │
└─────────────────────────────────────────────┘
```

### 5.3 常见策略对比

| 策略 | allocate | deallocate | 碎片 | 适用 |
|------|----------|------------|------|------|
| Bump (原地增长) | O(1) | 不支持 (reset全部) | 零 | 编译器、请求级生命周期 |
| Free List | O(1) | O(1) | 有 | 通用高频分配 |
| Slab | O(1) | O(1) | 极少 | 同大小对象 |
| Arena | O(1) | 批量 | 零 | 阶段式计算 |
| Pool | O(1) | O(1) | 低 | 通用 |
| Buddy System | O(log n) | O(log n) | 低 | 内核、大块管理 |

### 5.4 Bump Allocator 实现要点

```cpp
class BumpAllocator {
    char* memory_;      // 预分配内存块
    size_t capacity_;
    size_t offset_;     // 当前空闲区起始偏移

public:
    BumpAllocator(size_t size) : capacity_(size), offset_(0) {
        memory_ = static_cast<char*>(std::aligned_alloc(64, size));
        if (!memory_) throw std::bad_alloc();
    }

    void* allocate(size_t size) {
        // 对齐到 8 字节
        size = ((size + 7) / 8) * 8;
        if (offset_ + size > capacity_)
            throw std::bad_alloc();  // 或扩容
        void* ptr = memory_ + offset_;
        offset_ += size;
        return ptr;
    }

    void reset() { offset_ = 0; }  // "释放所有"

    ~BumpAllocator() { std::free(memory_); }
};
```

### 5.5 内存池与 Tensor 集成

```cpp
// 张量算子内存池场景
// 理由: 算子计算过程中会产生大量中间张量，每个算子都需要输入/输出缓冲区
// 保守估计: ResNet50 一次前向传播 ~30 个中间张量
// 内存池避免每个中间张量都 new/delete → 减少 90%+ 分配开销

// 集成方式:
// 1. 每个算子接受 MemoryPool&，从中分配输出缓冲区
// 2. 推理框架维护一个 per-inference pool，前向完成后 reset
// 3. 大张量从专用 pool 分配（减少碎片）

template<typename T>
Tensor<T> matmul(const Tensor<T>& a, const Tensor<T>& b, MemoryPool& pool) {
    auto [M, K] = a.shape();  // M×K
    auto [_, N] = b.shape();  // K×N
    // 从内存池分配输出张量
    T* output = static_cast<T*>(pool.allocate(M * N * sizeof(T)));
    // ... 计算 ...
    return Tensor<T>(output, {M, N}, pool);  // 共享 pool 所有权
}
```

## 六、`aligned_alloc` / `posix_memalign` / `aligned_alloc` 对比

| 函数 | 平台 | C++标准 | 释放函数 | size 要求 |
|------|------|---------|---------|----------|
| `std::aligned_alloc` | 跨平台 | C++17 | `std::free` | size 必须是 align 倍数 |
| `posix_memalign` | POSIX | C | `free` | 无 (自动调整) |
| `_aligned_malloc` | Windows | MSVC | `_aligned_free` | 无 |
| `memalign` | POSIX (已废弃) | C | `free` | 无 (但可能非标准) |
| `operator new(size, align)` | 跨平台 | C++17 | `operator delete` | 无 |

### 对齐的硬件原因

```cpp
// 1. SIMD 指令要求:
//    SSE (128-bit):  16 字节对齐 → _mm_load_ps (aligned) vs _mm_loadu_ps (unaligned)
//    AVX (256-bit):  32 字节对齐 → _mm256_load_ps vs _mm256_loadu_ps
//    AVX-512 (512-bit): 64 字节对齐
//    对齐版本: 1 条指令; 未对齐: 可能 2+ 条指令 (拆分跨 cache line 访问)

// 2. Cache line 边界:
//    不对齐的数据可能跨越两条 cache line → 两次加载
//    对齐到 64 字节确保数据总在一条 cache line 内

// 3. 页表 (TLB) 效率:
//    不对齐可能跨越 page 边界 → TLB miss
//    对齐减少页面边界交叉

// 关键: 对齐不仅是"好习惯"，而是"正确使用硬件"的必要条件
```

## 七、真实张量库中的内存布局

| 库 | 默认布局 | 可配置布局 | stride 支持 | 内存管理 |
|----|---------|-----------|------------|---------|
| NumPy | Row-major (C) | `order='F'` | 支持 | Python GC + refcount |
| PyTorch | Row-major | 支持 | 支持 | `c10::StorageImpl` + refcount |
| TensorFlow | Row-major | NHWC/NCHW 可配 | 支持 | `TensorBuffer` + refcount |
| Eigen | Column-major 默认 | Row-major 可选 | 支持 | 自定义 allocator |
| OpenCV `cv::Mat` | Row-major | 支持 | `step` 参数 (类比 stride) | refcount + 自定义 allocator |
| BLAS/LAPACK | Column-major | — | 无 (需连续) | 用户管理 |
| CUDA cuBLAS | Column-major | — | 无 (需连续) | GPU 显存 |
| MKL-DNN/oneDNN | 灵活 (blocked layout) | 多种 | 内部优化 | 自定义 allocator + 内存池 |

## 八、学习检查清单

- [ ] 能写出 N 维张量 row-major 线性索引公式
- [ ] 能解释为什么 transpose 可以零拷贝（stride 交换）
- [ ] 能写出判断张量连续性的代码
- [ ] 理解什么时候必须调用 `contiguous()` 并拷贝
- [ ] 能实现一个 bump allocator
- [ ] 能从无到有手写一个简单的 2D 张量类（含索引、打印、shape/stride）
- [ ] 能用 aligned_alloc 分配对齐内存，并理解对 SIMD 的影响
- [ ] 了解 PyTorch stride/shape/offset 的关系
