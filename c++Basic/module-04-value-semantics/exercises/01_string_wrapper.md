# 任务: 实现 StringWrapper 类

## 需求背景

需要封装一个动态字符串类，支持拷贝、移动、比较和拼接操作。

## 需求描述

实现 `StringWrapper` 类:
1. 构造函数 `StringWrapper(const char* str)` 和 `StringWrapper(const std::string& str)`
2. 拷贝构造、拷贝赋值（三五法则）
3. 移动构造、移动赋值
4. `operator+` 拼接
5. `operator==` 比较内容相等
6. `operator<<` 输出流
7. `length()` 返回长度

## 验收标准

- [ ] 拷贝后两个对象独立
- [ ] 移动后原对象处于安全状态
- [ ] 拼接返回新对象
- [ ] 能正确放入 vector
