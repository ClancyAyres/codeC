# 习题 04：多返回值（引用参数）

## 目标

实现函数 `min_max`，通过引用参数返回多个值。

## 要求

实现函数：

```cpp
void min_max(const int* arr, size_t size, int& min, int& max);
```

- 找出数组中的最小值和最大值
- 通过 `min` 和 `max` 引用参数输出结果
- 处理空数组：将 `min` 和 `max` 都设为 0
- 如果数组只有一个元素，min 和 max 应相等

## 提示

- 初始化 `min` 和 `max` 为第一个元素
- 遍历剩余元素更新 min 和 max
- 不要使用额外数组，不要排序

## 示例

```
输入: arr = {5, 2, 9, 1, 7}, size = 5
输出: min = 1, max = 9

输入: arr = {42}, size = 1
输出: min = 42, max = 42

输入: arr = nullptr, size = 0
输出: min = 0, max = 0
```
