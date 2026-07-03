# 任务: 实现 swap 函数与字符串处理

## 需求背景

在一个数据处理模块中，需要对数据进行交换和格式化。Java 中你直接修改对象引用即可，但 C++ 需要区分值、引用和指针。

## 需求描述

在 `01_swap_ref.cpp` 中实现三个函数:

1. `swapByValue(int a, int b)` — 按值传递版本。试着实现它，然后在 main 中观察: 外部变量是否被交换了？
2. `swapByRef(int& a, int& b)` — 按引用传递。交换两个 int。
3. `appendSuffix(std::string& str, const std::string& suffix)` — 使用引用在字符串末尾追加后缀。

## 编码规范检查清单

- [ ] 只读参数是否用了 `const`？
- [ ] 需要修改的参数是否用了引用 `&`？
- [ ] 没有 `using namespace std;` 在全局作用域

## 验收标准

- [ ] `swapByRef` 能正确交换 main 中的两个变量
- [ ] `swapByValue` 不能交换外部变量（让学生自己发现差异）
- [ ] `appendSuffix` 直接修改传入的字符串
- [ ] 编译无警告
