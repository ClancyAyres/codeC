# 任务: 解析命令行参数

## 需求背景

需要实现一个命令行工具的参数解析功能，熟悉 main 函数签名和字符串处理。

## 需求描述

在 `01_parse_args.cpp` 中实现:

1. `main(int argc, char* argv[])` — 打印参数个数和每个参数
2. `bool hasFlag(int argc, char* argv[], const std::string& flag)` — 检查命令行中是否存在某个标志
3. `std::string getArg(int argc, char* argv[], const std::string& key)` — 获取 `--key=value` 形式的值

## 验收标准

- [ ] 正确打印参数个数和列表
- [ ] `--verbose` 标志能被检测到
- [ ] `--output=file.txt` 能解析出值
