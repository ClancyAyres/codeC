# 任务: 实现泛型栈

## 需求背景

需要实现一个通用的栈数据结构，支持任意类型。

## 需求描述

实现 `template<typename T> class Stack`:
- `push(const T&)` 和 `push(T&&)` 入栈
- `pop()` 出栈并返回（栈空时抛异常）
- `top()` 查看栈顶（不弹出）
- `empty()`, `size()`
- `clear()` 清空

## 验收标准
- [ ] 支持 int, string, 自定义类型
- [ ] 空栈 pop/top 抛异常
