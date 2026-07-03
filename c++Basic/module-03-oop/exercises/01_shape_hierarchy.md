# 任务: 实现图形层次结构

## 需求背景

报表系统需要计算不同类型图形的面积和周长，并支持序列化输出。

## 需求描述

在 `01_shape_hierarchy.cpp` 中实现:

1. 抽象基类 `Shape`:
   - 纯虚函数 `area()` 和 `perimeter()`
   - 纯虚函数 `description()` 返回描述字符串
   - virtual 析构函数

2. 两个具体类:
   - `Circle(double radius)` — 面积 πr², 周长 2πr
   - `Rectangle(double width, double height)` — 面积 w×h, 周长 2(w+h)

3. 工厂函数 `std::unique_ptr<Shape> createShape(const std::string& type, double a, double b = 0)`:
   - "circle" → Circle(a)
   - "rectangle" → Rectangle(a, b)
   - 未知类型 → 抛异常

## 编码规范检查清单

- [ ] 基类析构函数是 virtual 的吗？
- [ ] 所有覆写都标注了 override 吗？
- [ ] 工厂函数返回的是智能指针吗？
- [ ] 成员变量是 private 吗？

## 验收标准

- [ ] 多态遍历 Shape 数组计算总面积和总周长
- [ ] 工厂函数正确创建不同类型
- [ ] 未知类型抛异常并被捕获
