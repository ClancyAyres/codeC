# 习题 02：设计不可拷贝类

## 目标

设计一个 `NonCopyable` 基类和一个使用它的 `UniqueResource` 类，使用 `= delete` 禁止拷贝。

## 要求

1. 创建 `NonCopyable` 基类：
   - 拷贝构造函数 = delete
   - 拷贝赋值运算符 = delete
   - 允许默认构造、移动构造、移动赋值
   - protected 析构函数（防止通过基类指针删除派生类对象）

2. 创建 `UniqueResource` 类继承 `NonCopyable`：
   - 持有一个 `int*` 资源（用 new 分配）
   - 构造函数接收 int 值，用 new 分配并存储
   - 析构函数 delete 资源
   - 提供 `int value() const` 获取资源值
   - 提供 `void setValue(int v)` 修改资源值
   - 允许移动，禁止拷贝

## 提示

- 使用 `= delete` 显式删除拷贝操作
- 移动后源对象的资源指针应设为 `nullptr`
- 析构时检查指针是否为空

## 预期输出

```
Creating r1 with value 42
  r1.value() = 42
Moving r1 to r2
  r1 still valid? no (nullptr)
  r2.value() = 42
r2.setValue(100)
  r2.value() = 100
Destroying resource 100
```
