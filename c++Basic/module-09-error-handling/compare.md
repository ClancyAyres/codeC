# 模块 8: 错误处理 — Java vs C++

## 核心差异

| 维度 | Java | C++ |
|------|------|-----|
| 检查异常 | checked exception（编译器强制处理） | 没有 checked exception |
| 异常规范 | `throws IOException` | `noexcept`（C++11，承诺不抛异常） |
| finally | `finally` 块 | RAII（析构函数自动清理） |
| try-with-resources | `try (Resource r = ...) { }` | RAII + 栈对象 |
| 错误码 | 不常用 | `std::error_code` 在某些团队替代异常 |

## 知识点: 异常

| 场景 | Java | C++ | 踩坑提醒 |
|------|------|-----|----------|
| 抛异常 | `throw new Exception()` | `throw std::runtime_error("msg")` | ⚠️ C++ 可以抛任意类型，但推荐标准异常 |
| 捕获 | `catch(Exception e)` | `catch(const std::exception& e)` | ⚠️ C++ 按值捕获会切片，必须用引用 |
| 重抛 | `throw e;` vs `throw;` | `throw;` (保留原始异常) | ⚠️ `throw e;` 在 C++ 中也会切片 |
| 析构中抛异常 | 不适用 | ⚠️ 绝对是禁忌！ | 析构函数抛异常 → `std::terminate` |

## 知识点: noexcept

| 场景 | C++ |
|------|-----|
| 标记不抛异常 | `void foo() noexcept;` |
| 条件 noexcept | `void swap(T& a, T& b) noexcept(noexcept(a.swap(b)));` |
| 检查 noexcept | `noexcept(foo())` — 编译期 bool |
| 移动构造应标记 noexcept | 这样 vector 扩容时用移动而非拷贝 |

## 知识点: RAII 与异常安全

RAII 是 C++ 异常安全的基础。确保资源在异常时正确释放。

### 相关编码规范

| 规范来源 | 编号 | 规则 |
|----------|------|------|
| C++ Core Guidelines | E.2 | 用异常来表明"不能完成给定任务" |
| C++ Core Guidelines | E.12 | 如果不可能从异常中恢复则不抛异常 |
| C++ Core Guidelines | E.16 | 用析构函数来做异常安全保证 |
| C++ Core Guidelines | E.17 | 不要在析构函数中抛出异常 |
| Google Style Guide | - | 适当使用异常，但不要过度 |

## 常见 Bug

- [ ] 按值捕获异常导致切片
- [ ] 析构函数中抛异常
- [ ] `throw new std::exception()` — 应该 throw by value
- [ ] 忘记 `noexcept` 导致 vector 使用拷贝而非移动
