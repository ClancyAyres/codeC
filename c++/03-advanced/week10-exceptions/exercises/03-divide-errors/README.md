# 习题 03：Divide Errors — 三种错误处理方式对比

## 目标

用异常、`std::optional`、`std::variant` 三种方式实现除法错误处理。

## 要求

实现以下四个版本的除法函数：

### `divideThrow` — 异常版本
- `b == 0` → 抛 `std::invalid_argument`
- 结果非有限（NaN / Inf）→ 抛 `std::overflow_error`
- 正常计算并返回

### `divideOptional` — optional 版本
- 任何错误 → 返回 `std::nullopt`
- 正常 → 返回 `a / b` 包裹在 optional 中

### `divideVariant` — variant + 错误字符串
- `DivideResult = std::variant<double, std::string>`
- 错误 → 返回字符串错误信息
- 正常 → 返回结算结果

### `divideWithCode` — variant + error_code
- `CodedResult = std::variant<double, std::error_code>`
- `b == 0` → `std::make_error_code(std::errc::invalid_argument)`
- 非有限 → `std::make_error_code(std::errc::result_out_of_range)`

## 提示

- 使用 `std::isfinite(x)` 检查结果是否为有限值（排除 NaN 和 Inf）
- `std::holds_alternative<T>(variant)` 检查 variant 当前持有类型
- `std::get<T>(variant)` 提取值（类型不匹配时抛异常）
- 用 `0.0 / 0.0` 可得到 NaN
- variant 返回错误时，直接 `return std::string("error msg")` 或 `return std::make_error_code(...)`

## 预期输出

```
[divideThrow] 通过
[divideOptional] 通过
[divideVariant] 通过
[divideWithCode] 通过

所有测试通过。
```
