# 习题 02：多维归约

## 目标

实现沿指定轴的归约操作（求和与求最大值），理解多维索引到一维平面的映射。

## 要求

1. **实现 `reduceSum1D`**：一维数组求和，返回标量

2. **实现 `reduceSumAxis0`**：二维归约沿 axis=0（列归约）
   - 输入 shape (rows, cols) → 输出 shape (cols)
   - `output[c] = sum(input[r*cols + c] for r in 0..rows-1)`

3. **实现 `reduceSumAxis1`**：二维归约沿 axis=1（行归约）
   - 输入 shape (rows, cols) → 输出 shape (rows)
   - `output[r] = sum(input[r*cols + c] for c in 0..cols-1)`

4. **实现 `reduceSum2D`**：通用版，根据 axis 参数调度

5. **实现 `reduceMaxAxis1`**：沿 axis=1 求每行最大值
   - 初始值用 `std::numeric_limits<T>::lowest()`

6. **在 main() 中测试**所有归约操作并验证结果

## 提示

- 一维索引公式：`index = r * cols + c`
- axis=0 归约：外层循环 c，内层循环 r（累加列元素）
- axis=1 归约：外层循环 r，内层循环 c（累加行元素）
- 输出数组大小：axis=0 → cols, axis=1 → rows

## 预期结果

所有测试用例输出 `✓ 通过`。
