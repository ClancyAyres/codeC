# 习题 01：字符串函数（指针算术实现）

## 目标

使用纯指针算术（不使用 `[]`）实现三个 C 风格字符串函数。

## 要求

实现以下函数，**只能使用指针算术（`*p`, `p++`, `p--` 等），不允许使用 `[]`**：

```cpp
// 计算字符串长度（不包含 '\0'）
size_t my_strlen(const char* str);

// 复制 src 到 dest（包括 '\0'），返回 dest
char* my_strcpy(char* dest, const char* src);

// 比较 s1 和 s2，返回负数/s1<s2, 0/s1==s2, 正数/s1>s2
int my_strcmp(const char* s1, const char* s2);
```

## 提示

- 字符串以 `'\0'` 结尾
- `while (*p != '\0')` 等价于 `while (*p)`
- 指针加减会按照类型大小自动调整步长
- `my_strcmp` 逐字符比较，直到找到差异或遇到 `'\0'`
