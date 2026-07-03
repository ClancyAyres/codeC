# Week 7：STL 容器与算法 — Java vs C++ 差异对照

## 一、容器映射

| Java | C++ | 关键差异 |
|------|-----|---------|
| `ArrayList<T>` | `std::vector<T>` | vector 在末尾增删 O(1) 均摊，连续内存 |
| `LinkedList<T>` | `std::list<T>` | 双向链表，无随机访问 |
| `ArrayDeque<T>` | `std::deque<T>` | 双端队列，分段连续内存 |
| `HashMap<K,V>` | `std::unordered_map<K,V>` | 均基于哈希表，C++ 桶遍历需 `begin(i)` |
| `TreeMap<K,V>` | `std::map<K,V>` | 红黑树，有序，O(log n) |
| `HashSet<T>` | `std::unordered_set<T>` | 基于哈希表 |
| `TreeSet<T>` | `std::set<T>` | 红黑树，有序 |
| `Stack<T>` | `std::stack<T>` | 容器适配器，默认底层 deque |
| `Queue<T>` | `std::queue<T>` | 容器适配器，默认底层 deque |
| `PriorityQueue<T>` | `std::priority_queue<T>` | 默认大顶堆（Java 默认小顶堆！） |

## 二、String 差异

| 方面 | Java `String` | C++ `std::string` |
|------|--------------|-------------------|
| 可变性 | 不可变（Immutable） | 可变（Mutable） |
| 字符串池 | 有 intern pool | 无内置 intern pool |
| 拼接开销 | `StringBuilder` 优化 | `+=` 直接修改，或 `std::ostringstream` |
| 字面量 | `"hello"` 是 `String` 对象 | `"hello"` 是 `const char[6]`，隐式转换 |
| 只读视图 | 无 | `std::string_view`（C++17） |
| 比较 | `.equals()` 比较值，`==` 比较引用 | `==` 直接比较值 |
| 空值表示 | `null` 可赋值给 String | `std::string` 不能为 null；用 `std::optional<std::string>` |
| 小字符串优化 | 无 | SSO：短字符串不分配堆内存 |

## 三、Stream API → `<algorithm>` + Lambda

```java
// Java Stream API
List<String> result = names.stream()
    .filter(s -> s.length() > 3)
    .map(String::toUpperCase)
    .sorted()
    .collect(Collectors.toList());
```

```cpp
// C++ <algorithm> + lambda
std::vector<std::string> result;
std::copy_if(names.begin(), names.end(), std::back_inserter(result),
    [](const auto& s) { return s.length() > 3; });
std::transform(result.begin(), result.end(), result.begin(),
    [](auto s) {
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        return s;
    });
std::sort(result.begin(), result.end());
```

C++ 迭代器 + `<algorithm>` 等价于 Java Stream，但：
- **更灵活**：迭代器范围不限于容器
- **更显式**：需要指定输出位置（`back_inserter`）
- **零开销**：lambda 可内联，无虚函数调用
- C++20 `<ranges>` 提供管道语法接近 Java Stream

## 四、迭代器差异

| Java `Iterator<T>` | C++ Iterator |
|-------------------|-------------|
| `hasNext()` / `next()` | `it != end()` / `*it++` |
| 单向 | 5 种分类（Input、Forward、Bidirectional、RandomAccess、Contiguous） |
| 基于对象（接口） | 基于概念（编译时 duck typing） |
| 无指针语义 | 行为模拟指针（`*it` 解引用，`it->` 成员访问） |
| 无范围语法 | C++11 range-for：`for (auto& x : container)` |

## 五、Lambda 表达式

```java
// Java lambda
(s) -> s.length()
(String s) -> { return s.length(); }
```

```cpp
// C++ lambda
[](const std::string& s) { return s.length(); }
[](const std::string& s) -> size_t { return s.length(); }
```

| 特性 | Java | C++ |
|------|------|-----|
| 捕获外部变量 | 自动（effectively final） | 显式指定：`[=]` 值捕获，`[&]` 引用捕获 |
| 可变 lambda | 不支持修改捕获变量 | `[=]() mutable { x++; }` |
| 泛型 lambda | 不支持 | `[](auto x) { return x * 2; }`（C++14） |
| 立即执行 | 不支持 | `[&]{ /* code */ }();` |
| 返回类型推导 | 自动 | `auto` 推导或显式 `-> RetType` |

## 六、快速参考卡

| 场景 | Java 做法 | C++ 做法 |
|------|----------|---------|
| 动态数组 | `ArrayList<T>` | `std::vector<T>` |
| 键值映射 | `HashMap<K,V>` | `std::unordered_map<K,V>` |
| 有序映射 | `TreeMap<K,V>` | `std::map<K,V>` |
| 过滤 | `.stream().filter(...)` | `std::copy_if` |
| 转换 | `.stream().map(...)` | `std::transform` |
| 排序 | `Collections.sort(list, comp)` | `std::sort(begin, end, comp)` |
| 求和 | `.stream().reduce(0, Integer::sum)` | `std::accumulate(begin, end, 0)` |
| 删除 | `.removeIf(...)` | `std::remove_if` + `erase` (erase-remove idiom) |
| 查找 | `.contains(obj)` | `std::find(begin, end, val) != end` |
| 子串 | `str.substring(i, j)` (新对象) | `std::string_view(str).substr(i, n)` (零拷贝) |
