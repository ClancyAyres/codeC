# 模块 5: STL 容器与算法 — Java vs C++

## 容器对应表

| Java | C++ | 说明 |
|------|-----|------|
| `ArrayList<T>` | `std::vector<T>` | 动态数组 |
| `LinkedList<T>` | `std::list<T>` | 双向链表 |
| `HashMap<K,V>` | `std::unordered_map<K,V>` | 哈希表 |
| `TreeMap<K,V>` | `std::map<K,V>` | 红黑树，有序 |
| `HashSet<T>` | `std::unordered_set<T>` | 哈希集合 |
| `TreeSet<T>` | `std::set<T>` | 有序集合 |
| `Stream<T>` | `<algorithm>` + range-for | C++20 ranges 更接近 |
| `Optional<T>` | `std::optional<T>` (C++17) | 可空值 |
| `String` | `std::string` / `std::string_view` | string_view 不持有数据 |
| `PriorityQueue<T>` | `std::priority_queue<T>` | 堆 |
| `Deque<T>` | `std::deque<T>` | 双端队列 |
| `Stack<T>` | `std::stack<T>` | 栈 |

## 关键差异

| 场景 | Java | C++ | 踩坑提醒 |
|------|------|-----|----------|
| 删除元素 | `list.remove(i)` | `vec.erase(vec.begin()+i)` | ⚠️ 删除后迭代器失效 |
| 插入 | `list.add(i, elem)` | `vec.insert(vec.begin()+i, elem)` | ⚠️ 可能在 O(n) |
| map 访问不存在的 key | 返回 null | `map[key]` 会插入默认值！ | ⚠️ 用 find() 或 at() 代替 |
| 迭代 | `for-each` / `iterator()` | range-for / 迭代器 | ⚠️ 不可在迭代中修改容器 |
| 排序 | `Collections.sort()` | `std::sort(vec.begin(), vec.end())` | ⚠️ map/set 是自动排序的 |

## 常见 Bug

- [ ] `map[key]` 检查存在性 — key 不存在时会插入并返回默认值
- [ ] 遍历中删除元素导致迭代器失效
- [ ] 在 vector 中间频繁插入/删除 — 考虑用 list/deque
- [ ] `for (auto item : vec)` 忘记 `&` 导致拷贝
- [ ] string 拼接使用 `+` 循环导致 O(n^2) — 用 `ostringstream` 或 `reserve`
