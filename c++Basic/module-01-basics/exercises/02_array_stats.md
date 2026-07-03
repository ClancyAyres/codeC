# 任务: 数组统计函数

## 需求背景

系统中有传感器数据数组，需要计算统计信息。你的任务是实现两个统计函数，一个返回结果，一个通过输出参数返回结果。

## 需求描述

在 `02_array_stats.cpp` 中实现:

1. `computeStats(const int* data, size_t size, int* outMin, int* outMax, double* outAvg)` — 通过输出指针返回最小值、最大值、平均值。如果 `data` 为空或 `size` 为 0，不修改任何输出参数。
2. 在 `main` 中准备测试数据，调用函数并输出结果。

## 编码规范检查清单

- [ ] 输入数据是 const 的吗？
- [ ] 输出参数是否检查了 nullptr？
- [ ] 指针参数语义清晰：只读 vs 可写

## 验收标准

- [ ] 正确计算 min/max/avg
- [ ] nullptr 检查：输出参数为 nullptr 时不崩溃
- [ ] 空数组安全处理
