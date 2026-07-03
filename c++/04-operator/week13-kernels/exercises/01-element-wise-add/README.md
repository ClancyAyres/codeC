# 习题 01：模板化逐元素加法

## 目标

实现模板化的逐元素数组操作，支持多种数据类型和操作。

## 要求

1. **实现 `elementWiseAdd` 模板函数**：
   - 接受三个同类型数组指针 `a`, `b`, `c` 和长度 `n`
   - 计算 `c[i] = a[i] + b[i]`
   - 必须同时支持 `float` 和 `int` 类型

2. **实现 `scalarMultiply` 模板函数**：
   - 接受输入数组、标量值、输出数组
   - 计算 `out[i] = in[i] * scalar`

3. **实现 `inPlaceRelu` 模板函数**：
   - 接受数据数组和长度
   - 原地修改：`data[i] = max(data[i], T(0))`

4. **在 main() 中测试**：
   - float 加法并验证结果
   - int 加法并验证结果
   - 标量乘法验证
   - ReLU 原地修改验证

## 提示

- `template<typename T>` 声明模板函数
- 参数使用 `const T*` 表示只读输入，`T*` 表示输出
- 验证：定义 `expected` 数组，使用 `compare()` 函数比较
- ReLU 实现：`if (data[i] < 0) data[i] = 0;` 或 `data[i] = std::max(data[i], T(0));`

## 预期结果

所有测试用例输出 `✓ 通过`，验证 float 和 int 版本产生正确结果。
