# 习题 01：Tensor2D 类

## 目标

实现一个支持 row-major stride 的 2D 张量类，包含创建、索引、打印功能。

## 要求

1. **`Tensor2D(int64_t rows, int64_t cols)`**：构造 `rows×cols` 张量，分配连续内存，计算 row-major strides
2. **`at(i, j)`**：按 `data[offset + i*stride0 + j*stride1]` 访问元素，越界时输出错误或抛出异常
3. **`shape()` / `strides()`**：返回形状和步长
4. **`print(label)`**：等宽打印张量内容
5. **`data()` / `size()`**：获取原始数据指针和总元素数

## 提示

- Row-major: `stride0 = cols`, `stride1 = 1`
- `at(i,j)` 公式: `linear = i * stride0 + j * stride1 = i * cols + j`
- 使用 `std::shared_ptr<T[]>` 管理内存生命周期（C++17）
- `print()` 中每行等宽对齐

## 预期结果

- 所有 at(i,j) 访问返回正确值
- `data()[i*cols + j] == at(i,j)` 恒成立
- 越界访问有适当处理
- print() 输出清晰可读
