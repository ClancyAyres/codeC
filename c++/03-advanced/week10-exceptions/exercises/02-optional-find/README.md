# 习题 02：Optional Find — 用 std::optional 实现键值查找

## 目标

用 `std::optional` 替代哨兵值和输出参数，实现安全的查找和取值。

## 要求

完成以下函数：

### `findByName`
- 在 `std::vector<User>` 中按名字查找
- 找到返回 `User`，未找到返回 `std::nullopt`

### `findEmail`
- 链式查找：先找用户，再获取 email
- 无用户 → `nullopt`
- 有用户无 email → 有值的 `optional`（但其中的 string 无意义，表示 email 字段为空）

### `getOrDefault`
- 用 `value_or` 实现

### `safeGet`
- 用 `opt.value()` 而非 `*opt`
- 空时抛 `std::bad_optional_access`

### `ConfigLoader::getConfig`
- 延迟初始化模式
- 用 `cache_.emplace(...)` 原地构造

## 提示

- `std::optional<T>` 有 `has_value()`, `value()`, `value_or()`, `operator*`, `operator->`
- `opt.emplace(args...)` 原地构造 T，避免额外的拷贝/移动
- `operator*` 在空时是未定义行为，`value()` 在空时抛异常
- 链式查找：先查用户 → 有用户再看 email → 层层判断

## 预期输出

```
[findByName] 找到 Bob (id=2)
[findByName] Diana 未找到，返回 nullopt
[findEmail] Alice email: alice@example.com
[findEmail] Bob email: 无邮箱
[findEmail] Diana email: 无用户
[getOrDefault] 通过
[safeGet] value() 正确抛出 bad_optional_access
[延迟初始化] 通过 (42)

所有测试通过。
```
