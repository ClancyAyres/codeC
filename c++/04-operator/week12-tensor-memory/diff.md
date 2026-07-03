# Week 12：张量的内存模型 — 无Java对照，纯C++算子专项

> Java 生态中的张量库（如 ND4J、DJL、TensorFlow Java）在底层依赖 native（C/C++/CUDA）
> 实现内存布局，Java 层只做 thin wrapper。因此本节聚焦 C++ 原生实现，不设 Java 对照。

## 一、行优先 (Row-Major) vs 列优先 (Column-Major)

### 两种布局的内存对比

```
Row-major (C/C++ 默认, PyTorch):  最后维度连续变化
  shape [2, 3, 4] → 内存中: [0,0,0] [0,0,1] [0,0,2] [0,0,3] [0,1,0] [0,1,1] ...
  第0维变化最慢，最后维变化最快

Column-major (Fortran, MATLAB):  第一维连续变化
  shape [2, 3, 4] → 内存中: [0,0,0] [1,0,0] [0,1,0] [1,1,0] [0,2,0] [1,2,0] ...
  第0维变化最快，最后维变化最慢
```

| 特性 | Row-Major (C-order) | Column-Major (F-order) |
|------|---------------------|------------------------|
| 默认语言 | C, C++, Python/NumPy, PyTorch | Fortran, MATLAB, R |
| 最内层维度 | 最后一维 | 第一维 |
| 索引公式 (3D) | `(i*C+j)*W + k` | `i + (j+k*H)*R` |
| 矩阵乘优化 | 默认 BLAS 接口匹配 | 需转置或特殊 kernel |
| 逐行遍历效率 | 连续访问 (cache友好) | 跨行访问 (cache miss) |

### 索引公式 (Row-Major)

```
// 通用 N 维张量 row-major 线性索引:
//   linear = sum_{d=0}^{N-1} (index[d] * stride[d])
//   其中 stride[d] = prod_{k=d+1}^{N-1} shape[k]
//
// 2D: tensor[i][j] = data[i * cols + j]               stride[0]=cols, stride[1]=1
// 3D: tensor[i][j][k] = data[(i*C + j)*W + k]          stride[0]=C*W, stride[1]=W, stride[2]=1
// 4D: tensor[i][j][k][l] = data[(((i*C3 + j)*C2 + k)*C1) + l]
```

### 索引公式 (Column-Major)

```
// 2D: tensor[i][j] = data[j * rows + i]                stride[0]=1, stride[1]=rows
// 3D: tensor[i][j][k] = data[i + (j + k*H)*R]
```

## 二、Stride 语义：解耦逻辑形状与物理内存

### Stride: 逻辑维度 -> 物理步长

```cpp
// 传统定长张量: stride 由 shape 推导，不可变
// shape=[2,3] → strides=[3,1] (row-major) 或 [1,2] (col-major)

// Stride 张量: stride 是独立参数，与 shape 解耦
// shape=[2,3], strides=[6,2] → 数据按步长6/2排列，物理存储 > 逻辑大小
// shape=[2,3], strides=[1,2]  → 这是列优先! (C++ 原生行优先也不妨碍)

// 零拷贝操作: 共享数据 + 修改 strides/shape/offset → 无需拷贝
//   slice:     offset+=..., shape 缩小
//   transpose: strides 交换
//   broadcast: shape 变大、stride 出现 0
//   permute:   strides 按新轴序重排
```

| 操作 | 数据拷贝? | 修改内容 |
|------|----------|---------|
| slice | 否 | offset += start*stride, shape 变小 |
| transpose | 否 | 交换 strides[0] ↔ strides[1] |
| broadcast | 否 | shape[d] = N, strides[d] = 0 |
| permute | 否 | strides 按 target_axes 重排 |
| reshape | 可能 | 若数据连续则只改 shape; 否则需 contiguous() + 拷贝 |
| concatenate | 是 | 新分配内存，拷贝各段 |
| pad | 是 | 新分配内存，居中写入原数据 |

### 连续性的判定

```cpp
bool isContiguous(const int64_t* shape, const int64_t* strides, int ndim) {
    int64_t expected = 1;
    for (int d = ndim - 1; d >= 0; --d) {
        if (shape[d] != 1 && strides[d] != expected)
            return false;  // 非连续: stride 与预期不符
        expected *= shape[d];
    }
    return true;
}
// 连续张量: 物理内存完全对应逻辑视图，无空洞
// 非连续张量: transpose/slice 后常见，需 contiguous() 才能传给 BLAS
```

## 三、零拷贝 vs 深拷贝：内存效率对比

| 方案 | 内存 | 速度 | 使用场景 |
|------|------|------|---------|
| 深拷贝 | 独立数据 (O(size)) | O(size) 拷贝 | 需独立修改结果 |
| 零拷贝视图 | O(1) 元数据 | O(1) 创建 | 临时视图、管道中间结果 |
| COW (写时复制) | O(1)+延迟 O(size) | O(1) 创建 | 需惰性拷贝时 |
| copy-on-write 共享 | refcount 管理 | 原子操作开销 | 多线程只读共享 |

## 四、对齐内存分配：`posix_memalign` / `std::aligned_alloc` / `operator new` 对齐重载

| 机制 | C++标准 | 注意事项 |
|------|---------|---------|
| `alignas(N)` | C++11 | 仅栈/静态对象; 动态分配默认忽略 |
| `std::aligned_alloc(align,size)` | C++17 | size 必须是 align 的倍数; 用 `free()` 释放 |
| `posix_memalign(&ptr,align,size)` | POSIX | 可分配任意 size (不需倍数) |
| `operator new(size, std::align_val_t)` | C++17 | 编译器自动选择对齐重载 |
| `_mm_malloc(size, align)` | Intel SSE | 需 `_mm_free()` (非 `free()`) |
| `cudaMallocManaged` | CUDA | GPU 统一内存 |

```cpp
// 对比: 无对齐 vs 有对齐对 SIMD 性能的影响
// 无对齐: 数据可能跨 cache line, 跨 page → 需要两次加载
// 有对齐: 加载/存储指令可直接用 movaps (aligned) 而非 movups (unaligned)
//         → 部分指令集 (SSE3+) 对齐版本更快
//         → 更关键: 避免 false sharing (两个变量在同一 cache line 互相干扰)
```

## 五、自定义 Allocator：解耦内存分配与数据操作

### C++ Allocator 概念

| 维度 | `std::allocator` | 自定义 Allocator | 内存池 |
|------|-----------------|-----------------|--------|
| 分配粒度 | 每次 `::operator new` | 可控 | 批量预留 |
| 释放策略 | 每次 `::operator delete` | 可控 | 惰性/复用 |
| 对齐 | 默认 `alignof(T)` | 可控 | 可控 |
| 线程安全 | 默认是 | 用户保证 | 用户保证 |
| 性能影响 | 系统调用开销 | `mmap`/`sbrk` 绕过 | 几乎零开销 |
| 适用场景 | 通用 | 对齐/设备内存/调试 | 高频小对象 |
| 标准兼容 | `std::vector<T,A>` | `std::vector<T,A>` | 非标准 API |
| 实现要点 | — | `allocate/deallocate` | 块管理 + 空闲链表 |

## 六、快速参考：关键决策树

```
张量操作
 ├─ 只读遍历 → 零拷贝视图 (修改 strides/shape/offset)
 ├─ 需要修改结果 → 深拷贝
 ├─ 需要传给 BLAS/LAPACK → contiguous() + 深拷贝
 ├─ 性能关键 → 对齐分配 + 内存池
 ├─ 高频小张量 → 内存池/arena allocator
 ├─ GPU 运算 → 关心张量布局 (NCHW vs NHWC)
 └─ 多线程读 → shared_ptr + 原子引用计数 (或 immutable)
```

## 七、PyTorch / NumPy Stride 参考

| 概念 | NumPy `ndarray` | PyTorch `Tensor` | C++ 仿写 |
|------|----------------|-----------------|---------|
| shape | `.shape` | `.sizes()` | `std::vector<int64_t>` |
| strides | `.strides` (字节为单位的步长) | `.strides()` | `std::vector<int64_t>` |
| offset | `ndarray.__array_interface__['data'][0]` | `.data_ptr()` | `T* data_ + offset` |
| slice | `a[2:5]` → view (strides 不变 offset+=2) | `t.slice(0, 2, 5)` → view | 同上 |
| transpose | `a.T` → strides 反转 | `t.t()` → strides 反转 | strides 反转 |
| contiguous | `np.ascontiguousarray(a)` | `.contiguous()` | 深拷贝 |
| copy | `a.copy()` 深拷贝 | `.clone()` 深拷贝 | 深拷贝 |

```
// NumPy strides 是字节步长，C++ 仿写通常是元素步长（类型大小倍数）
// NumPy: shape=[2,3] float64 → strides=(24, 8)   (3*8, 1*8)
// C++:   shape=[2,3] float64 → strides=(3, 1)    (元素计数)
// 转换为字节步长: byte_stride[d] = stride[d] * sizeof(T)
```
