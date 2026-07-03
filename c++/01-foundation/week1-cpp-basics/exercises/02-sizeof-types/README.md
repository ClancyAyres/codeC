# 习题 02：打印所有基本类型的 sizeof

## 目标

编写程序打印 C++ 中所有基本类型和定长类型占用的字节数。

## 要求

1. 打印以下内置类型的 `sizeof`：
   - `bool`, `char`, `short`, `int`, `long`, `long long`
   - `float`, `double`, `long double`
   - 指针（`void*`）

2. 打印以下定长类型的 `sizeof`：
   - `int8_t`, `uint8_t`
   - `int16_t`, `uint16_t`
   - `int32_t`, `uint32_t`
   - `int64_t`, `uint64_t`
   - `size_t`, `ptrdiff_t`

3. 额外挑战：打印 `int` 和 `unsigned int` 的最大值和最小值（使用 `std::numeric_limits`）

## 提示

- 需要包含 `<cstdint>` 和 `<iostream>`
- `sizeof` 返回 `size_t` 类型
- 使用 `std::numeric_limits<T>::max()` 和 `std::numeric_limits<T>::min()` 获取范围

## 期望输出参考

```
bool:           1 字节
char:           1 字节
short:          2 字节
int:            4 字节
long:           8 字节
long long:      8 字节
float:          4 字节
double:         8 字节
long double:    16 字节
指针:           8 字节
int8_t:         1 字节
...
int 最大值:     2147483647
int 最小值:     -2147483648
```
