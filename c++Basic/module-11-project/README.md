# Task Manager — 综合项目

这个 CLI 任务管理器串联了课程全部 10 个模块的知识点:

| 模块 | 在本项目中的应用 |
|------|-----------------|
| 1 基础语法 | 引用、const、auto、range-for |
| 2 内存管理 | unique_ptr 管理任务对象、RAII 文件操作 |
| 3 OOP | 抽象基类 Command、继承多态、Pimpl 隐藏实现 |
| 4 值语义 | 运算符重载、三五法则 |
| 5 STL | vector/map 存储任务、algorithm 排序查找 |
| 6 模板 | 泛型命令解析 |
| 7 现代 C++ | lambda、optional、string_view |
| 8 错误处理 | 异常处理、RAII 保证文件安全 |
| 9 并发 | (可选扩展: 异步保存) |

## 使用方式

编译后运行:
```
./task_manager
> add 完成C++学习计划
> add 写周报
> list
> done 1
> list
> save
> load
> quit
```

支持命令: add, list, done, undo, remove, save, load, help, quit
