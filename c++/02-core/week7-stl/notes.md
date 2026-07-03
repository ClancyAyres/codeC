# Week 7 知识点笔记：STL 容器与算法

## 一、序列容器

### 1.1 `std::vector<T>` — 动态数组

```cpp
#include <vector>
std::vector<int> v = {1, 2, 3};

v.push_back(4);        // 末尾添加
v.pop_back();          // 末尾删除
v[0] = 10;             // 随机访问 O(1)
v.at(0) = 10;          // 带边界检查的访问
v.size();              // 元素个数
v.capacity();          // 已分配容量
v.reserve(100);        // 预留容量，避免重复分配
v.shrink_to_fit();     // 释放多余容量
v.insert(v.begin(), 0); // 在指定位置插入
v.erase(v.begin());    // 删除指定位置元素
```

- **内存连续性**：元素在堆上连续存储，缓存友好
- **扩容策略**：通常 1.5x 或 2x 增长
- **迭代器失效**：重新分配后所有迭代器失效
- **适用场景**：需要随机访问、尾部增删、遍历

### 1.2 `std::deque<T>` — 双端队列

```cpp
#include <deque>
std::deque<int> dq;

dq.push_back(1);       // 尾部 O(1)
dq.push_front(2);      // 头部 O(1)
dq.pop_back();         // 尾部 O(1)
dq.pop_front();        // 头部 O(1)
dq[0];                 // 随机访问 O(1)（略慢于 vector）
```

- **分段连续内存**：多个固定大小块
- **优势**：两端操作 O(1)，不重新分配导致元素移动
- **劣势**：比 vector 略慢的随机访问，内存开销更大
- **适用场景**：需要两端操作的队列

### 1.3 `std::list<T>` — 双向链表

```cpp
#include <list>
std::list<int> lst = {1, 2, 3};

lst.push_back(4);
lst.push_front(0);
lst.insert(it, 5);     // O(1) 在指定位置插入
lst.erase(it);         // O(1) 删除指定位置
// lst[0];              // 不支持随机访问！
```

- **节点独立分配**：每个节点单独分配，非连续内存
- **插入/删除不使迭代器失效**（被删除元素除外）
- **劣势**：缓存不友好，内存开销大（每节点两个指针）
- **适用场景**：频繁中间插入/删除，且元素很大

### 1.4 `std::array<T, N>` — 固定大小数组

```cpp
#include <array>
std::array<int, 5> arr = {1, 2, 3, 4, 5};

arr[0] = 10;
arr.size();            // 编译时常量 5
arr.fill(0);           // 所有元素填充为 0
```

- **栈上分配**：无堆内存，大小编译时确定
- **包装 C 数组**：提供 STL 接口（`begin()`, `end()`, `size()`）
- **适用场景**：固定大小小数组，替代 `int arr[5]`

### 1.5 `std::forward_list<T>` — 单向链表

```cpp
#include <forward_list>
std::forward_list<int> fl = {1, 2, 3};

fl.push_front(0);      // 头部 O(1)
// fl.push_back(...);   // 不支持！需遍历到末尾
fl.insert_after(it, 5); // 在指定位置后插入
```

- 比 `list` 更省内存（一个指针 vs 两个指针）
- 只能前向遍历

## 二、关联容器

### 2.1 有序容器（红黑树实现）

```cpp
#include <map>
#include <set>

std::map<std::string, int> scores;
scores["Alice"] = 95;       // operator[] 若 key 不存在则插入默认值
scores.insert({"Bob", 87});
scores.at("Alice");         // key 不存在则抛 std::out_of_range
scores.find("Bob");         // 返回迭代器，不存在返回 end()
scores.count("Charlie");    // 0 或 1（multimap 中可 >1）

std::set<int> s = {3, 1, 4, 1, 5};
// 自动排序去重 → {1, 3, 4, 5}
s.insert(9);
```

| 容器 | 特点 | 查找/插入/删除 |
|------|------|-------------|
| `std::map<K,V>` | 键有序，唯一键 | O(log n) |
| `std::multimap<K,V>` | 键有序，可重复键 | O(log n) |
| `std::set<T>` | 有序，唯一值 | O(log n) |
| `std::multiset<T>` | 有序，可重复值 | O(log n) |

### 2.2 无序容器（哈希表实现）

```cpp
#include <unordered_map>
#include <unordered_set>

std::unordered_map<std::string, int> cache;
cache["key"] = 42;
cache.find("key");
cache.bucket_count();      // 桶数量
cache.load_factor();       // 负载因子
cache.rehash(100);         // 设置桶数量
cache.reserve(100);        // 预留足够桶容纳 n 个元素
```

| 容器 | 特点 | 平均查找/插入/删除 |
|------|------|-----------------|
| `std::unordered_map<K,V>` | 哈希，唯一键 | O(1) |
| `std::unordered_multimap<K,V>` | 哈希，可重复键 | O(1) |
| `std::unordered_set<T>` | 哈希，唯一值 | O(1) |
| `std::unordered_multiset<T>` | 哈希，可重复值 | O(1) |

### 2.3 有序 vs 无序选择

- **需要有序遍历** → 有序容器
- **只需 O(1) 查找** → 无序容器
- **自定义键** → 有序需 `operator<`，无序需 `hash<K>` + `operator==`
- **内存** → 无序容器内存开销更大（桶数组）

## 三、容器适配器

```cpp
#include <stack>
#include <queue>

// stack：默认底层 deque，后进先出
std::stack<int> stk;
stk.push(1);
stk.top();              // 查看栈顶
stk.pop();              // 移除栈顶（无返回值！）

// queue：默认底层 deque，先进先出
std::queue<int> q;
q.push(1);
q.front();              // 查看队首
q.back();               // 查看队尾
q.pop();

// priority_queue：默认底层 vector，默认大顶堆
std::priority_queue<int> pq;
pq.push(3);
pq.push(1);
pq.push(4);
pq.top();               // 4（最大值）
// 小顶堆：std::priority_queue<int, vector<int>, greater<int>>
```

## 四、迭代器

### 4.1 基本使用

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};

// 传统迭代器
for (auto it = v.begin(); it != v.end(); ++it) {
    std::cout << *it << ' ';
}

// const 迭代器（只读）
for (auto it = v.cbegin(); it != v.cend(); ++it) {
    std::cout << *it << ' ';
}

// 反向迭代器
for (auto it = v.rbegin(); it != v.rend(); ++it) {
    std::cout << *it << ' ';
}

// C++11 range-for（最常用）
for (const auto& x : v) {
    std::cout << x << ' ';
}
```

### 4.2 迭代器类别

```
InputIterator → ForwardIterator → BidirectionalIterator → RandomAccessIterator → ContiguousIterator
```

| 类别 | 支持操作 | 容器示例 |
|------|---------|---------|
| Input | `*it`, `++it`, `==`, `!=` | `std::istream_iterator` |
| Forward | Input + 可多次遍历 | `std::forward_list` |
| Bidirectional | Forward + `--it` | `std::list`, `std::set`, `std::map` |
| RandomAccess | Bidirectional + `it+n`, `it-n`, `<`, `>` | `std::vector`, `std::deque`, `std::array` |
| Contiguous | RandomAccess + 元素连续存储 | `std::vector`, `std::array` |

### 4.3 迭代器失效

```cpp
std::vector<int> v = {1, 2, 3};
auto it = v.begin();
v.push_back(4);  // 可能触发重新分配 → it 失效！
// *it;           // 未定义行为
```

| 容器 | 失效规则 |
|------|---------|
| `vector` | 重新分配全部失效；插入前元素之前迭代器有效，之后失效 |
| `deque` | 两端操作不失效；中间插入全部失效 |
| `list` | 插入不失效；被删除元素失效 |
| `map/set` | 插入不失效；被删除元素失效 |
| `unordered_map/set` | 插入可能因 rehash 全部失效；被删除元素失效 |

## 五、`<algorithm>` 常用算法

### 5.1 非修改序列操作

```cpp
#include <algorithm>
#include <numeric>

auto it = std::find(v.begin(), v.end(), 42);       // 查找值
auto it = std::find_if(v.begin(), v.end(),         // 条件查找
    [](int x) { return x > 10; });

size_t n = std::count(v.begin(), v.end(), 0);      // 计数
size_t n = std::count_if(v.begin(), v.end(),       // 条件计数
    [](int x) { return x % 2 == 0; });

bool b = std::all_of(v.begin(), v.end(),           // 全部满足
    [](int x) { return x > 0; });
bool b = std::any_of(v.begin(), v.end(),           // 任一满足
    [](int x) { return x < 0; });
bool b = std::none_of(v.begin(), v.end(),          // 无一满足
    [](int x) { return x < 0; });

auto p = std::minmax_element(v.begin(), v.end());  // 同时找最大最小
int min = *p.first;
int max = *p.second;
```

### 5.2 修改序列操作

```cpp
// 复制
std::copy(src.begin(), src.end(), dst.begin());
std::copy_if(src.begin(), src.end(), std::back_inserter(dst),
    [](int x) { return x > 0; });

// 转换
std::transform(v.begin(), v.end(), v.begin(),
    [](int x) { return x * 2; });

// 填充
std::fill(v.begin(), v.end(), 0);
std::generate(v.begin(), v.end(), [n = 0]() mutable { return n++; });

// 替换
std::replace(v.begin(), v.end(), 0, -1);           // 0 替换为 -1
std::replace_if(v.begin(), v.end(),
    [](int x) { return x < 0; }, 0);               // 负数替换为 0

// 去重（需先排序）
std::sort(v.begin(), v.end());
auto last = std::unique(v.begin(), v.end());
v.erase(last, v.end());                             // 真正删除
```

### 5.3 排序与搜索

```cpp
std::sort(v.begin(), v.end());                     // 升序 O(n log n)
std::sort(v.begin(), v.end(), std::greater<>());   // 降序
std::stable_sort(v.begin(), v.end());              // 稳定排序

std::reverse(v.begin(), v.end());                  // 反转

// 二分搜索（需已排序）
bool found = std::binary_search(v.begin(), v.end(), 42);
auto it = std::lower_bound(v.begin(), v.end(), 42); // 第一个 >= 42
auto it = std::upper_bound(v.begin(), v.end(), 42); // 第一个 > 42
```

### 5.4 删除操作

```cpp
// Erase-Remove Idiom（C++20 前标准方式）
v.erase(std::remove(v.begin(), v.end(), 42), v.end());
v.erase(std::remove_if(v.begin(), v.end(),
    [](int x) { return x < 0; }), v.end());

// C++20: std::erase / std::erase_if（更简洁）
std::erase(v, 42);
std::erase_if(v, [](int x) { return x < 0; });
```

### 5.5 数值算法

```cpp
#include <numeric>

int sum = std::accumulate(v.begin(), v.end(), 0);          // 求和
int prod = std::accumulate(v.begin(), v.end(), 1,
    std::multiplies<>());                                  // 求积

std::vector<int> partial(v.size());
std::partial_sum(v.begin(), v.end(), partial.begin());     // 前缀和

std::adjacent_difference(v.begin(), v.end(), partial.begin()); // 相邻差
```

## 六、Lambda 表达式

### 6.1 语法

```cpp
// [capture](params) -> return_type { body }

auto f1 = []{ return 42; };                    // 无参数，无捕获
auto f2 = [](int x) { return x * 2; };        // 有参数
auto f3 = [](int x) -> double { return x/2.0; }; // 显式返回类型
```

### 6.2 捕获方式

```cpp
int a = 10, b = 20;

// 值捕获（只读）
auto f1 = [a, b] { return a + b; };

// 引用捕获（可修改外部变量）
auto f2 = [&a, &b] { a += b; };

// 全部值捕获
auto f3 = [=] { return a + b; };

// 全部引用捕获
auto f4 = [&] { a += b; };

// 混合捕获
auto f5 = [=, &b] { b += a; return a; };       // b 引用，其余值
auto f6 = [&, a] { return a + b; };            // a 值，其余引用

// mutable：允许修改值捕获的副本
auto f7 = [a]() mutable { a++; return a; };

// 移动捕获（C++14）
auto f8 = [ptr = std::make_unique<int>(42)] { return *ptr; };

// 初始化捕获（C++14）
auto f9 = [value = a * b] { return value; };

// 泛型 lambda（C++14）
auto f10 = [](auto x, auto y) { return x + y; };
```

### 6.3 Lambda 与函数对象

Lambda 本质是编译器生成的匿名函数对象：

```cpp
// 这两个等价
auto lambda = [n](int x) { return x + n; };

struct __anonymous {
    int n;
    __anonymous(int n) : n(n) {}
    auto operator()(int x) const { return x + n; }
};
```

## 七、`std::string_view`（C++17）

```cpp
#include <string_view>

// 不拥有数据的只读视图
std::string s = "Hello, World!";
std::string_view sv = s;                // 指向 s 的数据

sv = "Hello, World!"sv;                 // 直接指向字面量

auto sub = sv.substr(7, 5);             // "World" — 零拷贝！
// sub 仍然指向原始数据，只是偏移+长度

// 典型用法：函数参数
void process(std::string_view name) {   // 不拷贝，接受 string/char*/string_view
    if (name.starts_with("pref_")) { }  // C++20
    if (name.ends_with("_suf")) { }     // C++20
}

// 注意：string_view 不保证空终止符！
// sv.data() 可能不是 null-terminated
// 需要 C 字符串时用 std::string(sv).c_str()
```

| 特性 | `std::string` | `std::string_view` |
|------|-------------|-------------------|
| 数据所有权 | 拥有 | 不拥有（视图） |
| 内存分配 | 可能分配堆内存 | 无分配 |
| 可变性 | 可变 | 只读 |
| 空终止 | 保证（C++11 起） | 不保证 |
| 生命周期 | 自管理 | 依赖原始数据存活 |
| substr | 拷贝新字符串 | O(1) 零拷贝 |
| 适用 | 需要拥有/修改字符串 | 函数参数、解析、子串 |

## 八、最佳实践

1. **默认用 `std::vector`** — 缓存友好，覆盖大部分场景
2. **需要两端操作用 `std::deque`** — 比 list 缓存更友好
3. **需要频繁中间插入/删除用 `std::list`** — 仅在元素很大或迭代器稳定性重要时
4. **`std::array` 替代 C 数组** — 提供 STL 接口，不退化指针
5. **查找优先用 `unordered_map`** — 除非需要有序遍历
6. **函数参数用 `string_view`** — 避免不必要的字符串拷贝
7. **用 range-for 替代迭代器循环** — 更简洁安全
8. **使用 erase-remove idiom** — C++20 前删除元素的正确方式
9. **Lambda 捕获明确化** — 优先显式捕获，避免 `[=]` 意外拷贝大对象
10. **`reserve()` 预分配** — 已知大小时避免重复扩容
