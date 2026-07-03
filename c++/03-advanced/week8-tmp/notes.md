# Week 8 知识点笔记：模板元编程入门

## 一、constexpr 函数与变量

### 1.1 constexpr 函数（C++11/14/17）

```cpp
// C++11: 只能有一条 return 语句
constexpr int square(int n) { return n * n; }

// C++14: 允许循环、分支、局部变量
constexpr int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i) result *= i;
    return result;
}
```

constexpr 函数**可能**在编译时求值：当参数是常量表达式时，结果在编译时确定。也可以在运行时调用。

### 1.2 constexpr 变量

```cpp
constexpr int answer = 42;          // 必须在编译时确定
constexpr int area = square(6);     // square(6) 编译时计算 → 36
constexpr int fib10 = fib(10);      // 模板元编程也能做到
```

constexpr 变量强制编译时求值：如果初始化表达式不是编译时常量，编译报错。

### 1.3 constexpr vs const

|          | const                  | constexpr                |
|----------|------------------------|--------------------------|
| 含义      | 值不可修改              | 编译时常量表达式         |
| 初始化时刻 | 运行时或编译时         | 必须编译时               |
| 能否运行时 | 可以（运行时初始化）   | 可以（constexpr 函数也可运行时调用） |

### 1.4 if constexpr（C++17）

```cpp
template<typename T>
auto getValue(T t) {
    if constexpr (std::is_pointer_v<T>) {
        return *t;       // 仅当 T 是指针时编译此分支
    } else {
        return t;
    }
}
```

- 条件在编译时求值，不成立的分支**完全不编译**
- 避免了 SFINAE 的繁琐写法，让代码更简洁

## 二、type_traits（类型特征）

### 2.1 常用 trait

```cpp
#include <type_traits>

std::is_integral_v<T>        // T 是整数类型？
std::is_floating_point_v<T>  // T 是浮点类型？
std::is_pointer_v<T>         // T 是指针？
std::is_reference_v<T>       // T 是引用？
std::is_same_v<T, U>         // T 和 U 是同一类型？
std::is_convertible_v<From, To>  // From 可隐式转换为 To？
std::is_base_of_v<Base, Derived> // Base 是 Derived 的基类？
std::is_class_v<T>           // T 是 class/struct？
```

### 2.2 类型转换 trait

```cpp
std::remove_const_t<T>          // const T → T
std::remove_reference_t<T>      // T& → T, T&& → T
std::add_pointer_t<T>           // T → T*
std::decay_t<T>                 // 数组→指针, 函数→函数指针, 去掉 cv 和引用
std::conditional_t<B, T, F>     // B?T:F (编译时三元)
std::enable_if_t<B, T>          // B 为 true 时是 T，否则替换失败
```

### 2.3 void_t 与 SFINAE 检测

```cpp
// void_t: C++17 提供的便捷类型别名
template<typename...>
using void_t = void;

// 检测某类型是否有 .size() 成员
template<typename, typename = void>
struct has_size : std::false_type {};

template<typename T>
struct has_size<T, std::void_t<decltype(std::declval<T>().size())>> : std::true_type {};

template<typename T>
inline constexpr bool has_size_v = has_size<T>::value;
```

## 三、SFINAE 与 enable_if

### 3.1 SFINAE 原理

**S**ubstitution **F**ailure **I**s **N**ot **A**n **E**rror

模板参数替换时，如果某候选产生了无效类型/表达式，该候选被**静默丢弃**，而非编译错误。这是 enable_if 的基石。

### 3.2 enable_if 用法

```cpp
// 方式一：作为返回类型
template<typename T>
typename std::enable_if<std::is_integral_v<T>, void>::type
process(T val) { /* 整数版本 */ }

// 方式二：作为额外模板参数（默认 void）
template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
void process(T val) { /* 整数版本 */ }

// 方式三：C++17 if constexpr（最简洁）
template<typename T>
void process(T val) {
    if constexpr (std::is_integral_v<T>) { /* 整数 */ }
    else if constexpr (std::is_floating_point_v<T>) { /* 浮点 */ }
}
```

### 3.3 enable_if vs if constexpr

|           | enable_if                      | if constexpr              |
|-----------|--------------------------------|---------------------------|
| 机制       | 排除整个函数重载               | 在函数体内部条件编译        |
| 语法复杂度  | 复杂                           | 简洁                      |
| 适用场景   | 选择不同重载（不同函数签名）    | 同一函数内不同行为          |
| C++ 版本  | C++11                          | C++17                     |

## 四、变参模板（Variadic Templates）

### 4.1 基本语法

```cpp
// Args 是模板参数包，args 是函数参数包
template<typename... Args>
void print(Args... args) {
    // 使用 pack...
}
```

### 4.2 递归展开（C++11 传统方式）

```cpp
// 基础情况：空包
void print() { std::cout << '\n'; }

// 递归情况：取第一个，递归剩余
template<typename T, typename... Args>
void print(T first, Args... rest) {
    std::cout << first << ' ';
    print(rest...);
}
```

### 4.3 折叠表达式（Fold Expressions，C++17）

```cpp
// 一元右折叠：(pack op ...)
template<typename... Args>
auto sum(Args... args) { return (args + ...); }           // args1 + (args2 + (args3 + argsn))

// 一元左折叠：(... op pack)
template<typename... Args>
auto sum(Args... args) { return (... + args); }           // ((args1 + args2) + args3) + argsn

// 二元右折叠：(pack op ... op init)
template<typename... Args>
void print(Args... args) { ((std::cout << args << ' '), ...); }

// 二元左折叠：(init op ... op pack)
template<typename... Args>
bool all(Args... args) { return (true && ... && args); }
```

### 4.4 参数包展开位置

```cpp
// 函数调用中展开
f(args...);

// 初始化列表中展开（C++11 惯用技巧）
std::vector<int> v{args...};

// 继承列表中展开
template<typename... Bases>
class Derived : public Bases... {};

// sizeof... 运算符
template<typename... Args>
void foo(Args... args) {
    std::cout << sizeof...(Args) << " types, "
              << sizeof...(args) << " arguments\n";
}
```

## 五、编译期类型列表

### 5.1 递归类型列表（类模板）

```cpp
// 空列表 — 递归终止
struct NullType {};

// 列表节点
template<typename H, typename T>
struct TypeList {
    using Head = H;
    using Tail = T;
};

// 使用
using MyList = TypeList<int, TypeList<double, TypeList<char, NullType>>>;

// 编译时获取长度
template<typename List>
struct Length;
template<>
struct Length<NullType> { static constexpr size_t value = 0; };
template<typename H, typename T>
struct Length<TypeList<H, T>> { static constexpr size_t value = 1 + Length<T>::value; };
```

### 5.2 编译时类型操作

```cpp
// 获取第 N 个类型
template<typename List, size_t N>
struct TypeAt;
template<typename H, typename T>
struct TypeAt<TypeList<H, T>, 0> { using type = H; };
template<typename H, typename T, size_t N>
struct TypeAt<TypeList<H, T>, N> { using type = typename TypeAt<T, N - 1>::type; };

// 在末尾追加
template<typename List, typename New>
struct Append;
template<>
struct Append<NullType, NullType> { using type = NullType; };
template<typename T>
struct Append<NullType, T> { using type = TypeList<T, NullType>; };
template<typename H, typename T, typename New>
struct Append<TypeList<H, T>, New> {
    using type = TypeList<H, typename Append<T, New>::type>;
};
```

## 六、常见陷阱

### 6.1 constexpr 不等于编译时

```cpp
constexpr int f(int x) { return x * 2; }
int n;
std::cin >> n;
int result = f(n);  // 运行时调用（合法）
int arr[f(n)];      // 编译错误：n 不是常量表达式
```

### 6.2 模板实例化深度

递归模板元编程受编译器 `-ftemplate-depth` 限制（默认约 900 层）。constexpr 循环没有此限制，优先用 constexpr 函数代替递归模板元编程。

### 6.3 enable_if 断言陷阱

```cpp
template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
void f(T) {}  // 有问题！第二个 f 的默认参数无法区分重载
```

应使用返回类型 enable_if，或使用额外的非类型参数：
```cpp
template<typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
void f(T) {}  // 正确
```

### 6.4 折叠表达式的空包

对于一元折叠，如果参数包为空，只有 `&&`（true）、`||`（false）、`,`（void()）有默认值。`+` 等运算符空包是编译错误，需用二元折叠提供初始值。
