# 习题 04：头文件重复包含

## 目标

理解头文件守卫（`#pragma once`）的作用，以及没有守卫时会发生的错误。

## 要求

1. 创建 `config.hpp`，定义 `const int VERSION = 1;`
2. 创建 `math.hpp`，包含 `config.hpp`，并声明 `add(int, int)`
3. 创建 `string_utils.hpp`，也包含 `config.hpp`，并声明 `to_upper(std::string)`
4. 创建 `main.cpp`，同时包含 `math.hpp` 和 `string_utils.hpp`

## 步骤

### 第一步：先不加守卫，观察错误

创建以上文件，但 `config.hpp` **不加** `#pragma once`。

编译：
```bash
g++ -std=c++17 main.cpp math.cpp string_utils.cpp -o test
```

预期结果：编译错误 —— `VERSION` 被重复定义。

### 第二步：加上守卫，修复问题

在 `config.hpp` 顶部添加 `#pragma once`（或其他头文件的 `#ifndef` 守卫）。

重新编译，应该成功。

## 为什么？

`#include` 是文本替换。`main.cpp` 的预处理过程：

```
main.cpp
  → #include "math.hpp"
    → #include "config.hpp"       ← VERSION 第一次定义
    → add() 声明
  → #include "string_utils.hpp"
    → #include "config.hpp"       ← VERSION 第二次定义 ← 错误！
    → to_upper() 声明
```

加上 `#pragma once` 后，预处理遇到第二次 `#include "config.hpp"` 时会跳过。

## 提示

- 每个 `.hpp` 文件都应该有头文件守卫
- 推荐使用 `#pragma once`（简洁），或 `#ifndef`/`#define`/`#endif`（标准）
- 即使在 `.cpp` 文件中被多次间接包含，守卫也能保护
