# 任务: 修复有问题的代码

## 需求背景

代码审查时发现一段有内存泄漏和悬垂指针问题的代码，需要你修复。这是一个典型的"接手遗留代码"场景。

## 需求描述

`02_fix_memory_leak.cpp` 中的代码有以下已知问题:

1. 内存泄漏: `processData` 中 `new` 的对象在异常路径没有 delete
2. 悬垂指针: `getCachedValue` 返回了局部变量指针
3. 使用了裸 new/delete（应该用智能指针）
4. 没有 RAII 管理资源

修复这些问题，使代码:
- 没有内存泄漏
- 没有悬垂指针
- 符合 C++ Core Guidelines 和 Google Style Guide
- 行为保持等价（不能改变功能逻辑）

## 验收标准

- [ ] 所有 new 替换为 make_unique 或 make_shared
- [ ] 不再有裸 delete
- [ ] getCachedValue 不再返回悬垂指针
- [ ] 代码编译无警告
