# 任务: 扩展 Task Manager

## 需求背景

综合项目已实现基础任务管理。请扩展以下功能。

## 需求描述

1. **优先级**: 每个 Task 增加 priority (low/medium/high)，list 按优先级排序
2. **编辑**: 实现 `edit <id> <new_title>` 命令修改任务标题
3. **搜索**: 实现 `search <keyword>` 命令，匹配任务标题
4. **统计**: 实现 `stats` 命令，显示 total/done/pending/完成率

## 验收标准

- [ ] 优先级排序正确 (high > medium > low)
- [ ] edit 修改成功后显示新标题
- [ ] search 支持部分匹配（不区分大小写）
- [ ] stats 显示正确统计信息
