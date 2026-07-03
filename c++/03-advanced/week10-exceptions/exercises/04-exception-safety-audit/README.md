# 习题 04：Exception Safety Audit — 找出并修复异常不安全代码

## 目标

审查代码，修复 5 个异常安全问题。

## 问题清单

### 问题1: 构造函数中手动 new 导致泄露
`FileProcessor` 的构造函数：如果第二个 `new` 抛异常，第一个 `new` 的结果泄露。
**修复**：用 `std::unique_ptr` 替代所有裸指针。

### 问题2: 非 noexcept move 构造
`Resource(Resource&&)` 未标记 `noexcept`，导致 `vector` 扩容时不敢用 move。
**修复**：标记为 `noexcept`，并添加 `static_assert` 验证。

### 问题3: 非 RAII 锁
`Counter::increment` 手动 lock/unlock，如果 `value += amount` 抛异常则永远不解锁。
**修复**：实现 `LockGuard` RAII 包装类。

### 问题4: 析构函数抛异常
`DatabaseConnection::~DatabaseConnection` 调用可能抛异常的 `close()`，导致 `std::terminate`。
**修复**：在析构中用 try/catch(...) 吞掉异常。

### 问题5: 返回裸指针
`createWidgetBad` 返回 `Widget*`，调用者可能忘记 `delete`。
**修复**：返回 `std::unique_ptr<Widget>`。

## 提示

- `std::unique_ptr` 在异常中自动析构，是 RAII 的核心工具
- `noexcept` 标记让 `std::vector` 能用 move 扩容
- 析构函数默认 `noexcept` — 内部抛异常 = 直接 `std::terminate`
- 如果析构函数必须调用可能失败的操作，用 `try { ... } catch(...) {}` 吞掉
- 工厂函数返回 `std::unique_ptr<T>` 是 C++ 惯例

## 预期输出

```
[构造 SAFETY] 通过
[noexcept MOVE] 通过
[RAII LOCK] 通过
[析构 NEXCEPT] 通过
[unique_ptr] Widget(42)

所有审计项通过。
```
