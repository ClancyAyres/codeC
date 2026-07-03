# 任务: 并行求和

## 需求背景

处理大数据集时需要并行计算和。

## 需求描述

实现 `long long parallelSum(const std::vector<int>& data, size_t numThreads)`:
- 把 data 分成 numThreads 份
- 每个线程计算自己那份的和
- 汇总所有部分和
- 用 `std::thread` 和 `std::atomic`/`std::future`

## 验收标准
- [ ] 结果和单线程 sum 一致
- [ ] 使用多个线程并行计算
