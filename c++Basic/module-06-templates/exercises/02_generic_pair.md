# 任务: 实现泛型 Pair

## 需求背景

需要一个通用的键值对，类似 std::pair 但加上比较和输出。

## 需求描述

实现 `template<typename T1, typename T2> class Pair`:
- 构造函数
- `operator==`, `operator!=`
- `operator<<` 输出流
- `getFirst()`, `getSecond()` 访问
- `makePair(T1, T2)` 工厂函数

## 验收标准
- [ ] Pair<int, string> 工作正常
- [ ] 比较逻辑正确
