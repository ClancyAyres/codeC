# 习题 01：词频统计 — unordered_map

## 目标

使用 `std::unordered_map` 统计一段文本中每个单词出现的次数，按频率降序输出。

## 要求

实现 `wordCount` 函数：
```cpp
// 输入文本，返回单词→频率映射
std::unordered_map<std::string, int> wordCount(const std::string& text);
```

- 单词由字母组成，忽略大小写（统一转小写）
- 忽略标点符号
- 在 `main` 中按频率降序输出结果

## 提示

- 使用 `std::istringstream` 分割单词
- `std::isalpha` 过滤非字母字符
- `std::tolower` 转换大小写
- `std::vector<std::pair<...>>` + `std::sort` 实现频率排序

## 预期输出

```
Word frequency (descending):
the: 4
cpp: 3
is: 2
powerful: 2
...
```
