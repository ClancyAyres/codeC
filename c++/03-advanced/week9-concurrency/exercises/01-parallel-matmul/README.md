# 习题 01：并行矩阵乘法

## 目标

实现并行矩阵乘法，将结果矩阵的行分配给多个线程计算。

## 要求

```cpp
Matrix parallelMultiply(const Matrix& A, const Matrix& B, int numThreads);
```

- 验证输入矩阵维度兼容（A 列数 == B 行数），不兼容时抛出 `std::invalid_argument`
- 将结果矩阵的行均匀分配给 `numThreads` 个线程
- 每个线程计算分配给它的那些行（C[i][j] = Σ A[i][k] * B[k][j]）
- 使用 `std::thread` 和 `join()`
- 如果 `numThreads <= 0`，使用 `std::thread::hardware_concurrency()`

## 提示

- `std::vector<std::vector<int>>` 不需要互斥锁——每个线程写不同行，无数据竞争
- 用 lambda 捕获 `[&, startRow, endRow]` 或 `[&, start, end]`
- 确保所有线程 `join()` 后才能访问结果矩阵

## 预期输出

```
A:
   1   2   3
   4   5   6
   7   8   9
B:
   9   8   7
   6   5   4
   3   2   1
A × B:
  30  24  18
  84  69  54
 138 114  90

C[0][0] = 30 (期望: 30)
C[2][2] = 90 (期望: 90)
```
