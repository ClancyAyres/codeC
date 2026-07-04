# 任务: 实现 Vec2d 类

## 需求背景

游戏引擎中需要 2D 向量运算。实现一个支持运算符重载的 2D 向量类。

## 需求描述

实现 `Vec2d` 类:
1. 构造函数 `Vec2d(double x, double y)`
2. `operator+`, `operator-`, `operator*`(标量), `operator/(标量)`
3. `dot(const Vec2d&)` 点积
4. `length()` 模长
5. `normalized()` 归一化
6. `operator==`, `operator<<`

## 验收标准

- [ ] 所有运算符正确
- [ ] 归一化除零保护
