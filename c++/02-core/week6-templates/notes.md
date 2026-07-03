# Week 6 知识点笔记：模板基础

## 一、函数模板

### 1.1 基本语法

```cpp
template<typename T>
T max(T a, T b) {
    return a > b ? a : b;
}
```

- `typename T`：`T` 是模板参数，可用 `class` 替代（等价）
- 编译器不会为未使用的模板生成代码

### 1.2 类型推导

```cpp
max(3, 5);        // T 推导为 int
max(3.14, 2.72);  // T 推导为 double
max(3, 3.14);     // 编译错误！T 推导冲突（int vs double）
max<double>(3, 3.14);  // 显式指定 T = double，int 隐式转换
```

编译器从实参推导模板参数类型。如果推导冲突，需显式指定。

### 1.3 显式实例化

```cpp
template<typename T>
T max(T a, T b) { return a > b ? a : b; }

// 显式实例化：强制编译器生成特定版本
template int max<int>(int, int);
template double max<double>(double, double);
```

显式实例化可用于将模板实现放在 .cpp 文件中（限制可用的类型集合）。

### 1.4 函数模板重载

```cpp
// 模板版本
template<typename T>
T max(T a, T b) { return a > b ? a : b; }

// 针对 const char* 的特化版本（重载，非特化）
const char* max(const char* a, const char* b) {
    return std::strcmp(a, b) > 0 ? a : b;
}
```

非模板函数优先于模板函数匹配。

## 二、类模板

### 2.1 基本语法

```cpp
template<typename T>
class Vector {
    T* data_;
    size_t size_;
    size_t capacity_;
public:
    Vector();
    ~Vector();
    void push_back(const T& value);
    T& operator[](size_t index);
    size_t size() const;
    size_t capacity() const;
};
```

### 2.2 成员函数定义

**方式一：类内定义**

```cpp
template<typename T>
class Vector {
public:
    size_t size() const { return size_; }  // 类内定义，隐式内联
};
```

**方式二：类外定义（同在头文件中）**

```cpp
template<typename T>
class Vector {
public:
    void push_back(const T& value);  // 声明
};

template<typename T>                  // 需要重复模板声明
void Vector<T>::push_back(const T& value) {  // Vector<T> 不是 Vector
    if (size_ >= capacity_) { /* reallocate */ }
    data_[size_++] = value;
}
```

- 类外定义必须在同一个翻译单元中可见（通常放在同一个 .hpp 文件）
- 每个成员函数定义前必须重复 `template<typename T>`
- 类名必须写成 `Vector<T>`，不是 `Vector`

### 2.3 模板代码组织

```
方案1（推荐）：全部放在头文件中
  vector.hpp — 包含所有声明和定义

方案2：显式实例化
  vector.hpp — 只有声明
  vector.cpp — 定义 + 显式实例化
  // vector.cpp 末尾
  template class Vector<int>;
  template class Vector<double>;
  // 缺点：只能使用显式实例化的类型

方案3：.tpp / .impl.hpp 模式
  vector.hpp — 声明
  vector.tpp — 定义
  // vector.hpp 末尾
  #include "vector.tpp"
```

**推荐方案1**用于学习和大多数项目。大型项目可能用显式实例化控制编译时间。

## 三、模板特化

### 3.1 全特化（Full Specialization）

```cpp
// 通用模板
template<typename T>
class TypeInfo {
public:
    static const char* name() { return "unknown"; }
};

// 全特化：针对 int
template<>
class TypeInfo<int> {
public:
    static const char* name() { return "int"; }
};

// 全特化：针对 double
template<>
class TypeInfo<double> {
public:
    static const char* name() { return "double"; }
};
```

### 3.2 偏特化（Partial Specialization）

```cpp
// 通用模板
template<typename T, typename U>
class Pair {
    T first;
    U second;
};

// 偏特化：两个类型相同时
template<typename T>
class Pair<T, T> {  // 匹配 Pair<int, int> 等
    T values[2];
};

// 偏特化：第二个参数为指针时
template<typename T, typename U>
class Pair<T, U*> {  // 匹配 Pair<int, double*> 等
    T first;
    U* second;
};
```

**规则：**
- 函数模板只能全特化，不能偏特化
- 类模板既可以全特化也可以偏特化
- 偏特化只匹配更特殊的情况，通用模板是回退选择

### 3.3 全特化 vs 偏特化对比

| | 全特化 | 偏特化 |
|---|--------|--------|
| 语法 | `template<>` | `template<typename T>` + 部分参数指定 |
| 函数模板 | 支持 | 不支持（用重载替代） |
| 类模板 | 支持 | 支持 |
| 匹配 | 所有参数已确定 | 部分参数仍为变量 |

## 四、非类型模板参数

### 4.1 语法

```cpp
template<typename T, int N>       // N 是编译时常量
class Array {
    T data_[N];                   // 栈上分配，大小编译时确定
public:
    T& operator[](int i) { return data_[i]; }
    constexpr int size() const { return N; }
};

Array<int, 5> arr;                // 5 个 int 的数组
Array<double, 100> big;           // 100 个 double 的数组
```

### 4.2 允许的非类型参数

| 类型 | 示例 |
|------|------|
| 整数类型 | `int`, `size_t`, `char`, `bool` |
| 枚举类型 | `enum Color { Red, Green }` |
| 指针 | `template<int* p>`（需静态存储期） |
| 引用 | `template<int& r>`（需静态存储期） |
| `std::nullptr_t` | `template<std::nullptr_t p>` |
| C++20: 浮点 | `template<double d>` |
| C++20: 类类型 | 满足特定条件的字面类型 |

### 4.3 编译时计算

```cpp
template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

// Factorial<5>::value == 120  （编译时计算）
```

## 五、typename vs class

```cpp
template<typename T>  // 推荐：语义清晰
template<class T>     // 等价：历史原因保留
```

两者在模板参数声明中**完全等价**。推荐使用 `typename`（语义更清晰）。

`typename` 还有第二种用途 — 标识依赖类型：

```cpp
template<typename T>
void foo() {
    typename T::value_type x;  // 告诉编译器 T::value_type 是类型
    // 不加 typename → 编译器默认 T::value_type 是静态成员/值
}
```

## 六、C++20 Concepts 简介

Concepts 为模板参数提供编译时约束，在实例化前产生清晰的错误消息。

```cpp
#include <concepts>

// 约束 T 必须支持 < 比较
template<typename T>
requires std::totally_ordered<T>
T max(T a, T b) {
    return a > b ? a : b;
}

// 简洁语法
template<std::totally_ordered T>
T max(T a, T b) {
    return a > b ? a : b;
}

// 使用 auto 的缩写
std::totally_ordered auto max(std::totally_ordered auto a,
                              std::totally_ordered auto b) {
    return a > b ? a : b;
}

// 自定义 Concept
template<typename T>
concept Printable = requires(T a) {
    { std::cout << a } -> std::same_as<std::ostream&>;
};
```

**优势：**
- 错误消息更短、更清晰（在调用点报错，而非实例化深处）
- 重载决议更精确
- 代码意图更明确

## 七、模板最佳实践

1. **模板代码放在头文件中** — 除非使用显式实例化
2. **`typename` 优先于 `class`** — 语义更清晰
3. **非类型参数尽量用 `size_t`** — 避免负数语义问题
4. **模板函数内用 `T{}` 而非 `T()`** — 统一初始化避免 most vexing parse
5. **避免过度模板化** — 不是所有代码都需要模板，有时普通函数更清晰
6. **C++20 项目中优先使用 Concepts** — 改善错误消息和接口文档
7. **注意代码膨胀** — 每个实例化产生独立代码，过多实例化会增加二进制大小
