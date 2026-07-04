# 任务: 用 Lambda 自定义排序

## 需求背景

一个员工管理系统需要按多种规则排序员工数据。

## 需求描述

定义 `struct Employee { std::string name; int age; double salary; };`

实现三个排序:
1. 按 salary 降序
2. 按 age 升序，同 age 按 name 字母序
3. 按 name 长度升序

全部使用 `std::sort` + lambda，直接在调用处写 lambda。

## 验收标准
- [ ] 三种排序都使用 lambda
- [ ] 排序结果正确
