# 习题 02：TypeDispatch — 编译时分发处理函数

## 目标

实现一个 `TypeDispatch<T>` 模板，根据类型特征输出不同描述。

## 要求

```cpp
template<typename T>
struct TypeDispatch {
    static void describe() {
        // TODO: 用 if constexpr 区分以下类型：
        //   整数类型   → "integral"
        //   浮点类型   → "floating-point"
        //   指针类型   → "pointer"（打印指向类型名称）
        //   std::string → "std::string"
        //   其他        → "unknown"
    }
};
```

额外：实现 `Traits` 结构，包含 `isIntegral`, `isFloating`, `isPointer`, `isString` 等 `static constexpr bool`。

## 预期行为

```
TypeDispatch<int>::describe()          → Type: integral
TypeDispatch<double>::describe()       → Type: floating-point
TypeDispatch<int*>::describe()         → Type: pointer
TypeDispatch<std::string>::describe()  → Type: std::string
TypeDispatch<char>::describe()         → Type: integral (char is integral)
TypeDispatch<float>::describe()        → Type: floating-point
Traits<int>::isIntegral    → true
Traits<int>::isFloating    → false
Traits<double>::isIntegral → false
Traits<double>::isFloating → true
```

## 提示

- 使用 `<type_traits>` 中的 `std::is_integral_v<T>`, `std::is_floating_point_v<T>`, `std::is_pointer_v<T>`, `std::is_same_v<T, std::string>`
- `if constexpr` 分支按顺序匹配
- Traits 结构直接用 `static constexpr bool` 成员 + type_traits
