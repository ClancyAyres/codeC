# 任务: Top-K 问题

## 需求背景

排行榜系统需要找出得分最高的 K 个用户。

## 需求描述

实现函数 `std::vector<int> topK(const std::vector<int>& data, size_t k)`:
- 返回 data 中最大的 k 个元素（降序）
- 如果 k > data.size()，返回全部（降序）
- 使用 `std::partial_sort` 或 `std::nth_element`

## 验收标准
- [ ] topK({5,1,9,3,7}, 3) → {9,7,5}
- [ ] k 大于数组大小时安全处理
