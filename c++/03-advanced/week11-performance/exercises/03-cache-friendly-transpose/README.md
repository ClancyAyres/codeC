# 习题 03：Cache-Friendly Matrix Transpose

## 目标

实现两种矩阵转置算法，理解缓存局部性对性能的巨大影响。

## 背景

矩阵转置 `dst[j][i] = src[i][j]` 中：
- 读 `src[i][j]` 是 row-major，连续访问 → cache 友好
- 写 `dst[j][i]` 是 column-major，跨行写入 → 大量 cache miss

当矩阵大小超过 L1 cache（~32KB）时，每次写入都 miss L1。

## 要求

### 1. 朴素转置 `transposeNaive`

```cpp
Matrix transposeNaive(const Matrix& src);
```

简单的双层循环 `for(i) for(j) dst[j][i] = src[i][j]`。
虽然读是连续的，但写是跳跃的。

### 2. 分块转置 `transposeBlocked`

```cpp
Matrix transposeBlocked(const Matrix& src, int blockSize = 16);
```

分块算法：
```
for (int ii = 0; ii < rows; ii += blockSize)
    for (int jj = 0; jj < cols; jj += blockSize)
        for (int i = ii; i < min(ii+blockSize, rows); ++i)
            for (int j = jj; j < min(jj+blockSize, cols); ++j)
                dst[j][i] = src[i][j];
```

一个 16×16 的 int 块占 1024 字节，完全放入 32KB L1 cache。
内两层循环（i, j）的读写全在 L1 内命中。

## 提示

- `blockSize` 一般取 8、16、32 或 64
- 需要处理矩阵尺寸不整除 blockSize 的情况（min 边界检查）
- 验证函数 `verify()` 已提供，确保两种实现结果相同
- 用 `std::vector<std::vector<int>>` 作为矩阵类型（`Matrix`）

## 预期结果

| 矩阵大小 | 朴素转置 | 分块转置 | 加速比 |
|----------|---------|---------|--------|
| 512×512  | ~10 ms  | ~3 ms   | ~3×    |
| 1024×1024| ~80 ms  | ~15 ms  | ~5×    |
| 2048×2048| ~400 ms | ~40 ms  | ~10×   |

## 深入思考

- 为什么分块越大不一定越好？（blockSize 太大超出 L1 → 块内也 miss）
- 为什么块也不能太小？（循环开销比例变大）
- 最优 blockSize 如何确定？（用 benchmark 在目标机器上实测）
