# 习题 03：makeVector — 变参模板创建 vector

## 目标

实现一个 `makeVector` 变参函数模板，接收任意数量不同类型的参数，统一转换为 `std::string` 后放入 `std::vector<std::string>`。

## 要求

```cpp
template<typename... Args>
std::vector<std::string> makeVector(Args... args) {
    // TODO: 使用折叠表达式将每个元素转为 std::string
    //   整数/浮点 → std::to_string
    //   std::string → 不加转换
    //   const char*  → std::string
    //   其他 → "<?>"
    // TODO: 返回 std::vector<std::string>
}
```

额外：实现 `makeSet`，返回 `std::set<std::string>`（自动去重排序）。

## 预期行为

```
makeVector(1, 2.5, "hello", std::string("world"))
  → ["1", "2.500000", "hello", "world"]

makeSet(3, 1, 2, 3, 1)  → ["1", "2", "3"]  (sorted, unique)
```

## 提示

- 用辅助函数 `toString(T)` 做类型分发（`if constexpr`）
- 折叠表达式：在 lambda/函数体中逐个 `push_back`
- 依赖 `<vector>`, `<string>`, `<set>`, `<type_traits>`
