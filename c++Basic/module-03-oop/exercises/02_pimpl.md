# 任务: 实现 Pimpl 模式

## 需求背景

库的公共 API 头文件需要隐藏实现细节，减少编译依赖。Pimpl (Pointer to Implementation) 是 C++ 最常用的封装模式。

## 需求描述

实现 `ConfigManager` 类，用 Pimpl 模式隐藏实现细节:

1. 公共头文件只暴露 `ConfigManager` 的接口
2. 实现细节（如读取文件、解析格式等）放在实现中
3. 接口: `get(const std::string& key)` 返回配置值
4. 接口: `set(const std::string& key, const std::string& value)` 设置配置

## 编码规范检查清单

- [ ] 是否用 unique_ptr 持有 impl？
- [ ] 析构函数是否在实现中定义？
- [ ] 是否显式声明了拷贝/移动？

## 验收标准

- [ ] 公共接口不包含任何实现细节
- [ ] 编译通过
