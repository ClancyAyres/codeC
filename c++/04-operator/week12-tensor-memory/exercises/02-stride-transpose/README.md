# 习题 02：Stride Transpose — 不拷贝数据的转置

## 目标

在已有的 Tensor2D 类上实现零拷贝转置和切片操作，理解 stride 元数据的力量。

## 要求

1. **`transpose()`**：返回新视图，交换 rows/cols 和 stride0/stride1，共享 storage
2. **`sliceRows(start, end)`**：返回行切片视图，修改 rows 和 offset
3. **`sliceCols(start, end)`**：返回列切片视图，修改 cols 和 offset
4. **验证**：修改转置/切片视图，原始张量对应位置也改变

## 提示

- `transpose`: `Tensor2D(storage, data, cols, rows, stride1, stride0, offset)` — 行列和步长都交换
- `sliceRows`: `offset += start * stride0`, `rows = end - start`
- `sliceCols`: `offset += start * stride1`, `cols = end - start`
- 所有视图的 `data_` 指向同一块内存起始位置
- 使用 private 构造函数创建视图

## 预期结果

- `transposed.at(j, i) == original.at(i, j)` 对所有 i,j 成立
- 修改转置视图，原始数据同步改变
- 切片后数据正确、共享相同 storage
- 所有操作 O(1)，无数据拷贝
