# 任务: 词频统计

## 需求背景

文本分析模块需要统计单词出现频率。

## 需求描述

实现函数 `std::map<std::string, int> wordCount(const std::vector<std::string>& words)`:
- 统计每个单词出现次数
- 返回按字母顺序排列的结果（用 map 自动排序）
- 忽略空字符串

## 编码规范检查清单
- [ ] map 的访问方式是否正确（不要用 [] 检查存在性）

## 验收标准
- [ ] 对 {"apple", "banana", "apple", "cherry"} 输出正确频率
- [ ] 空单词不计入
