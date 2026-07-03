# 任务: 用 variant 实现配置值解析器

## 需求背景

配置系统需要支持多种值类型（int, double, string, bool）。

## 需求描述

用 `std::variant<int, double, std::string, bool>` 实现 `ConfigValue`:
- `parse(const std::string& input)` 解析字符串返回 ConfigValue
- 用 `std::visit` 实现 `print(const ConfigValue&)` 输出
- 如果输入是 "true"/"false" → bool
- 如果输入是纯数字（有小数点） → double
- 如果输入是纯数字（无小数点） → int
- 否则 → string
