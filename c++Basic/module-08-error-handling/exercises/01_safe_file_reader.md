# 任务: 安全的文件读取

## 需求背景

实现一个安全读取文件的函数，正确处理所有异常情况。

## 需求描述

实现 `std::string readFileSafe(const std::string& filename)`:
- 文件不存在 → 抛 `FileOpenError`（自定义异常）
- 文件为空 → 返回空字符串
- 读取失败 → 抛 `std::runtime_error`
- 用 RAII 确保文件正确关闭
- 正常情况返回文件全部内容

## 验收标准
- [ ] 读正常文件返回内容
- [ ] 不存在的文件抛 FileOpenError
- [ ] 空文件返回空字符串
- [ ] 异常安全（文件正确关闭）
