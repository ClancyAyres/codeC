# 任务: 用 enum class 实现 HTTP 状态码处理

## 需求背景

HTTP 库需要处理状态码。用 `enum class` 实现类型安全的状态码检查。

## 需求描述

1. 定义 `enum class HttpStatus`，包含 `OK(200)`, `NotFound(404)`, `InternalError(500)`
2. `int toCode(HttpStatus status)` — 转为数值
3. `std::string toMessage(HttpStatus status)` — 转为描述
4. `bool isSuccess(HttpStatus status)` — 检查是否成功 (2xx)

## 验收标准

- [ ] 枚举值正确映射到 HTTP 状态码
- [ ] 类型安全，不能和 int 混用
