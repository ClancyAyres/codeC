# Week 10 知识点笔记：异常安全与错误处理

## 一、异常安全三等级

### 1.1 基本保证（Basic Guarantee）

不变量保持，无资源泄露。但对象状态可能改变（"有效但未指定"的状态）。

```cpp
class StringList {
    std::vector<std::string> data;
public:
    void append(const std::string& s) {
        data.push_back(s);  // push_back 可能抛 bad_alloc
    }
    // 如果 push_back 抛异常：
    // - vector 保持原有状态（不变）
    // - 无资源泄露
    // - 这就是基本保证
};
```

### 1.2 强保证（Strong Guarantee）

操作要么完全成功，要么完全不改变任何状态（"all or nothing"）。

```cpp
class StringList {
    std::vector<std::string> data;
public:
    void append(const std::string& s) {
        // 先构造副本，成功才交换
        auto copy = data;             // 可能抛 — 但 data 未变
        copy.push_back(s);            // 可能抛 — 但 data 未变
        data.swap(copy);              // 不抛异常
    }
    // 强保证：如果失败，data 保持原样
};
```

### 1.3 不抛异常保证（Nothrow Guarantee）

操作保证不抛异常。

```cpp
struct Connection {
    ~Connection() noexcept {   // 析构函数默认 noexcept
        close();               // close() 内部必须不抛
    }
};

void swap(StringList& a, StringList& b) noexcept {
    a.data.swap(b.data);      // vector::swap 是 noexcept
}

// noexcept 标记的函数：
// - 析构函数（默认）
// - swap 函数
// - move 构造函数（推荐）
// - 简单 getter/setter
```

### 1.4 等级选择指南

| 等级 | 何时使用 | 示例 |
|------|---------|------|
| Nothrow | 析构函数、swap、move、资源释放 | `~Resource()`, `swap()`, `lock.unlock()` |
| Strong | 可能失败但应原子化的操作 | 插入、删除、赋值 |
| Basic | 最低要求 — 适用即可 | 复杂交互式操作 |

## 二、noexcept 详解

### 2.1 语法与语义

```cpp
// C++11
void f1() noexcept;           // 保证不抛
void f2() noexcept(true);    // 保证不抛（等价）
void f3() noexcept(false);   // 可能抛（默认行为）
void f4();                    // 可能抛（默认行为）

// C++17：noexcept 是函数类型的一部分
void (*fp1)() noexcept = f1;            // ok
void (*fp2)() = f1;                     // C++17 起允许（隐式转换）
void (*fp3)() noexcept = f4;            // 错误！
```

### 2.2 noexcept 运算符

```cpp
// 编译时检查表达式是否 noexcept
constexpr bool b1 = noexcept(f1());          // true
constexpr bool b2 = noexcept(std::vector<int>{});  // false（构造可能分配内存）

// 用于条件 noexcept
template<typename T>
void swap_wrapper(T& a, T& b)
    noexcept(noexcept(swap(a, b)))   // 只有 T 的 swap 是 noexcept 才是
{
    swap(a, b);
}
```

### 2.3 编译器优化

```cpp
std::vector<MyClass> v;

// 如果 MyClass 的 move 构造是 noexcept：
// vector 扩容时使用 move（更高效）
// 否则：vector 扩容时使用 copy（更安全，因为强保证需要 copy 回滚）

class MyClass {
public:
    MyClass(MyClass&&) noexcept = default;  // 标记 noexcept 让 vector 用 move 扩容
    //    ^^^^^^^^ 重要！
};
```

### 2.4 隐式 noexcept

```cpp
// 编译器自动标记为 noexcept 的：
// - 析构函数（C++11 起默认）
// - 编译器生成的默认构造、拷贝构造、移动构造
//   （如果调用基类和成员的对应操作都是 noexcept）
// - delete 运算符（大部分重载）
```

## 三、RAII 与异常

### 3.1 基本 RAII 模式

```cpp
#include <memory>
#include <fstream>

void process() {
    // 所有资源在异常中也保证释放
    auto ptr = std::make_unique<int[]>(100);
    std::ifstream file("data.txt");
    std::lock_guard<std::mutex> lock(mtx);

    // 可能抛异常的操作
    do_work();

    // 无论 do_work 是否抛异常：
    // 1. lock 析构 → 解锁
    // 2. file 析构 → 关闭文件
    // 3. ptr 析构 → 释放内存
}
```

### 3.2 两阶段构造（避免构造函数抛异常后资源泄露）

```cpp
class Database {
    Connection* conn = nullptr;
    Statement* stmt = nullptr;

public:
    Database(const std::string& url) {
        conn = new Connection(url);      // 如果抛异常，ok
        try {
            stmt = new Statement(*conn); // 如果抛异常，conn 泄露！
        } catch (...) {
            delete conn;
            throw;  // 重新抛出
        }
    }

    ~Database() {
        delete stmt;
        delete conn;
    }
};

// 更好的写法：用 unique_ptr 管理，让 RAII 处理
class Database {
    std::unique_ptr<Connection> conn;
    std::unique_ptr<Statement> stmt;

public:
    Database(const std::string& url)
        : conn(std::make_unique<Connection>(url))
        , stmt(std::make_unique<Statement>(*conn))
    {
        // 如果 stmt 构造失败：
        // conn 的 unique_ptr 已完全构造，会析构 → 自动 delete
        // stmt 的 unique_ptr 尚未构造，不需要清理
    }
    // 析构函数自动清理，不需手写
};
```

### 3.3 RAII lock wrapper

```cpp
std::mutex mtx;
std::vector<int> data;

// 基本保证：lock_guard 析构时一定解锁
void add(int v) {
    std::lock_guard<std::mutex> lock(mtx);
    data.push_back(v);     // 即使抛异常，lock 析构解锁
}

// 强保证：先改临时副本，再交换
void add_strong(int v) {
    std::lock_guard<std::mutex> lock(mtx);
    auto copy = data;           // 快照
    copy.push_back(v);          // 在副本上操作，失败不影响原 data
    data.swap(copy);            // noexcept 交换
}
```

## 四、std::optional

### 4.1 基本用法

```cpp
#include <optional>
#include <string>

std::optional<int> parse(const std::string& s) {
    try {
        return std::stoi(s);
    } catch (...) {
        return std::nullopt;   // 表示"无值"
    }
}

int main() {
    auto a = parse("42");
    auto b = parse("abc");

    // 检查是否有值
    if (a) {                    // 或 a.has_value()
        std::cout << "a = " << *a << '\n';    // 42
        std::cout << "a = " << a.value() << '\n';  // 42，空时抛 bad_optional_access
    }

    // 提供默认值
    int result = b.value_or(-1);  // -1
    std::cout << "b = " << result << '\n';
}
```

### 4.2 常用操作

```cpp
std::optional<int> opt;

// 构造
opt = 42;                    // 赋值有值
opt = std::nullopt;          // 变为无值
opt = std::make_optional(42);
opt.emplace(42);             // 原地构造

// 检查
bool has = opt.has_value();
bool has2 = static_cast<bool>(opt);
if (opt) { /* ... */ }

// 获取值
int v1 = *opt;               // 未检查 — 无值时 UB
int v2 = opt.value();        // 无值时抛 std::bad_optional_access
int v3 = opt.value_or(-1);   // 安全默认值

// 重置
opt.reset();                 // 变为无值，析构持有的对象
```

### 4.3 常见使用场景

```cpp
// 场景1: 替代哨兵值
// int find_index(...) { return -1; }  ← 魔法值
std::optional<size_t> find_index(const std::vector<int>& v, int target) {
    for (size_t i = 0; i < v.size(); ++i)
        if (v[i] == target) return i;
    return std::nullopt;
}

// 场景2: 延迟初始化
class Widget {
    std::optional<Expensive> cache;
public:
    const Expensive& get() {
        if (!cache) cache.emplace(/* ... */);
        return *cache;
    }
};

// 场景3: 可能失败的操作
std::optional<User> findUser(int id) {
    if (id <= 0) return std::nullopt;
    return User{id, "name"};
}
```

## 五、std::variant

### 5.1 基本用法

```cpp
#include <variant>
#include <string>
#include <iostream>

// variant 是类型安全的 union — 同一时刻持有其中一种类型
std::variant<int, std::string, double> v;

v = 42;
v = "hello";
v = 3.14;

// 访问当前持有的值
std::visit([](const auto& val) {
    std::cout << val << '\n';
}, v);

// 检查持有类型
if (std::holds_alternative<int>(v)) {
    int i = std::get<int>(v);        // 类型不匹配时抛 std::bad_variant_access
}

// 使用 get_if（不抛异常版）
if (int* p = std::get_if<int>(&v)) {
    std::cout << "int: " << *p << '\n';
}

// 使用 index() 获取类型索引
size_t idx = v.index();  // 0 = int, 1 = string, 2 = double
```

### 5.2 std::visit 模式

```cpp
std::variant<int, double, std::string> v = "hello";

// visit 自动分发到正确的类型
std::visit([](const auto& arg) {
    using T = std::decay_t<decltype(arg)>;
    if constexpr (std::is_same_v<T, int>)
        std::cout << "int: " << arg << '\n';
    else if constexpr (std::is_same_v<T, double>)
        std::cout << "double: " << arg << '\n';
    else if constexpr (std::is_same_v<T, std::string>)
        std::cout << "string: " << arg << '\n';
}, v);

// 使用 overloaded 模式（更优雅）
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// C++17 需要显式推导指引，C++20 可省略
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

std::visit(overloaded{
    [](int i)          { std::cout << "int: " << i << '\n'; },
    [](double d)       { std::cout << "double: " << d << '\n'; },
    [](std::string s)  { std::cout << "string: " << s << '\n'; },
}, v);
```

### 5.3 variant 作为 Result 类型

```cpp
#include <variant>
#include <string>
#include <system_error>

// 简易 Result 类型 — 返回值或错误码
template<typename T>
using Result = std::variant<T, std::error_code>;

Result<int> safe_divide(int a, int b) {
    if (b == 0)
        return std::make_error_code(std::errc::invalid_argument);
    return a / b;
}

void use() {
    auto result = safe_divide(10, 0);
    std::visit(overloaded{
        [](int value) { std::cout << "Result: " << value << '\n'; },
        [](std::error_code ec) { std::cout << "Error: " << ec.message() << '\n'; },
    }, result);
}
```

## 六、错误码模式

### 6.1 错误码 vs 异常选择

| 方面 | 错误码 | 异常 |
|------|-------|------|
| 性能（无错误路径） | 需要检查返回值 | 零开销 |
| 调用者未知错误 | 必须逐层传递 | 自动传播 |
| 不忽略 | 易忽略返回值 | 不 catch 则 terminate |
| 大型代码库 | 显式可见 | 隐式传播 |
| 实时系统 | 通常禁用 | 难以保证实时性 |

### 6.2 std::error_code（C++11）

```cpp
#include <system_error>
#include <iostream>

std::error_code open_file(const std::string& path) {
    int fd = ::open(path.c_str(), O_RDONLY);
    if (fd < 0)
        return {errno, std::generic_category()};
    ::close(fd);
    return {};  // 默认构造表示成功
}

void demo() {
    auto ec = open_file("/nonexistent");
    if (ec) {  // operator bool — 有错误则为 true
        std::cout << "Error: " << ec.message() << '\n';
        std::cout << "Code: " << ec.value() << '\n';
        std::cout << "Category: " << ec.category().name() << '\n';
    }
}
```

### 6.3 函数式错误处理

```cpp
// 返回 variant<T, Error> 的函数式风格
template<typename T, typename E>
using Result = std::variant<T, E>;

Result<int, std::string> parse_int(const std::string& s) {
    try {
        size_t pos;
        int v = std::stoi(s, &pos);
        if (pos != s.size())
            return "trailing characters: " + s.substr(pos);
        return v;
    } catch (const std::exception& e) {
        return std::string("parse error: ") + e.what();
    }
}
```

## 七、构造函数失败

### 7.1 构造函数中抛异常

```cpp
class Resource {
    std::unique_ptr<Connection> conn;
public:
    Resource(const std::string& url)
        : conn(std::make_unique<Connection>(url))
    {
        // 如果 Connection 构造抛异常：
        // conn 的 unique_ptr 尚未构造 → 无需清理
    }

    // 构造失败 → 对象未被构造 → 析构函数不被调用
    // → 但已完全构造的成员/基类的析构函数会被调用
};
```

### 7.2 构造失败规则

```cpp
void f() {
    Resource* p = new Resource("db://...");
    // 如果 Resource 构造抛异常：
    // - new 表达式保证释放分配的内存
    // - p 不会被赋值（保持原值）
    // - 异常继续向上传播

    auto q = std::make_unique<Resource>("db://...");
    // 如果构造抛异常：
    // - smart pointer 未构造
    // - 无清理负担
}
```

## 八、最佳实践

1. **始终用 RAII 管理资源** — 不要手写 delete/close/unlock
2. **析构函数绝不抛异常** — 默认 noexcept，内部用 try/catch(...) 吞掉
3. **move 构造函数标记 noexcept** — 让 vector 等容器在扩容时能用 move
4. **优先用基本保证，关键操作做到强保证**
5. **用 catch(const std::exception& e) 而非 catch(...)** — 除非你真的要吞一切
6. **optional/variant 优先于哨兵值和输出参数**
7. **构造失败直接抛异常** — 不要用 init()/isValid() 两阶段模式（来自更早时代的)
8. **不要 catch 然后什么也不做** — 要么处理，要么传播
9. **noexcept 用于明确不抛的操作** — 帮助编译器优化，帮助调用者推理
10. **异常安全是组合的** — 用 RAII 组件组合的系统自然具备异常安全
