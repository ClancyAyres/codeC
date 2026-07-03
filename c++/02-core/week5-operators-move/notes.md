# Week 5 知识点笔记：运算符重载与移动语义

## 一、运算符重载规则

### 1.1 可重载的运算符

| 类别 | 运算符 |
|------|--------|
| 算术 | `+` `-` `*` `/` `%` |
| 位运算 | `&` `\|` `^` `~` `<<` `>>` |
| 比较 | `==` `!=` `<` `>` `<=` `>=` `<=>`(C++20) |
| 逻辑 | `!` `&&` `\|\|` |
| 赋值 | `=` `+=` `-=` `*=` `/=` `%=` `&=` `\|=` `^=` `<<=` `>>=` |
| 自增/自减 | `++` `--`（前置/后置） |
| 下标 | `[]` |
| 函数调用 | `()` |
| 成员访问 | `->` `->*` |
| 逗号 | `,` |
| 内存 | `new` `new[]` `delete` `delete[]` |
| 类型转换 | `operator T()` |

### 1.2 成员函数 vs 友元函数

```cpp
class Complex {
    double real_, imag_;
public:
    Complex(double r, double i) : real_(r), imag_(i) {}

    // 成员函数：左操作数是 Complex
    Complex operator+(const Complex& rhs) const {
        return Complex(real_ + rhs.real_, imag_ + rhs.imag_);
    }
    Complex operator+(double rhs) const {
        return Complex(real_ + rhs, imag_);
    }

    // 成员函数：需要 this
    Complex& operator+=(const Complex& rhs) {
        real_ += rhs.real_;
        imag_ += rhs.imag_;
        return *this;
    }

    // 友元函数：左操作数不是 Complex
    friend Complex operator+(double lhs, const Complex& rhs) {
        return Complex(lhs + rhs.real_, rhs.imag_);
    }

    // 友元函数：流输出（左操作数是 ostream，不是 Complex）
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        return os << c.real_ << " + " << c.imag_ << "i";
    }

    // 比较运算符通常用成员函数
    bool operator==(const Complex& rhs) const {
        return real_ == rhs.real_ && imag_ == rhs.imag_;
    }

    bool operator!=(const Complex& rhs) const {
        return !(*this == rhs);  // 用 == 实现 !=
    }
};
```

### 1.3 成员 vs 友元选择指南

| 运算符 | 推荐形式 | 原因 |
|--------|---------|------|
| `=` `+=` `-=` `*=` `/=` | 成员 | 必须修改 this |
| `[]` `()` `->` | 成员 | C++ 要求必须是成员 |
| `++` `--`（前置/后置） | 成员 | 修改自身状态 |
| `+` `-` `*` `/` | 友元（或成员） | 友元支持隐式转换两侧；成员只转换右侧 |
| `==` `!=` `<` `>` | 成员 | 通常对称即可，成员足够 |
| `<<` `>>` | 友元 | 左操作数是 `std::ostream`/`std::istream` |

### 1.4 前置/后置自增自减

```cpp
class Counter {
    int value_ = 0;
public:
    // 前置 ++：先自增，返回引用
    Counter& operator++() {
        ++value_;
        return *this;
    }

    // 后置 ++：返回旧值（int 参数是哑元，仅用于区分）
    Counter operator++(int) {
        Counter old = *this;
        ++value_;
        return old;
    }
};

// 使用
Counter c;
++c;   // 调用前置 ++
c++;   // 调用后置 ++（int 参数由编译器传入 0）
```

## 二、移动语义深入

### 2.1 值类别速查

```
            表达式
           /      \
      左值          右值
   (有标识、      (可移动)
    可取地址)      /     \
            纯右值      亡值
          (临时对象、  (std::move结果、
           字面量)      函数返回右值引用)
```

### 2.2 移动语义的 noxcept 重要性

```cpp
class Buffer {
public:
    // 移动构造标记 noexcept — 允许 vector 在扩容时使用移动而非拷贝
    Buffer(Buffer&& other) noexcept
        : data_(std::exchange(other.data_, nullptr))
        , size_(std::exchange(other.size_, 0)) {
    }

    // 如果移动构造不声明 noexcept，vector 扩容时会退化为拷贝！
    // 原因：vector 需要强异常安全保证，拷贝失败可回滚，移动失败不可回滚
};

static_assert(std::is_nothrow_move_constructible_v<Buffer>);
```

### 2.3 std::exchange 模式

```cpp
// 移动构造/赋值的惯用写法
class Widget {
    int* ptr_ = nullptr;
    size_t size_ = 0;
public:
    // 移动构造
    Widget(Widget&& other) noexcept
        : ptr_(std::exchange(other.ptr_, nullptr))
        , size_(std::exchange(other.size_, 0)) {
    }

    // 移动赋值
    Widget& operator=(Widget&& other) noexcept {
        delete[] ptr_;  // 释放自身资源
        ptr_ = std::exchange(other.ptr_, nullptr);
        size_ = std::exchange(other.size_, 0);
        return *this;
    }
};
```

### 2.4 移动后的对象状态

```cpp
void moveSemantics() {
    std::string s1 = "hello";
    std::string s2 = std::move(s1);  // s1 被移动

    // s1 处于"有效但未指定"状态
    // - 可以安全销毁
    // - 可以重新赋值：s1 = "world";
    // - 可以调用无前置条件的成员：s1.empty(), s1.size()
    // - 不应假设其内容

    s1.clear();     // 安全：无前置条件
    s1 = "new";     // 安全：重新赋值
    // std::cout << s1[0];  // 未定义：移动后内容未指定
}
```

### 2.5 自动移动规则

编译器在以下情况自动移动（无需显式 std::move）：

```cpp
Widget createWidget() {
    Widget w;
    return w;  // 自动移动（NRVO 更可能直接原地构造）
}

Widget createFromTemp() {
    return Widget();  // 自动移动
}

// 不要写 return std::move(w); — 这会阻止 RVO/NRVO 优化
```

## 三、完美转发深入

### 3.1 万能引用与引用折叠

```cpp
template<typename T>
void func(T&& arg) {  // 万能引用（非右值引用！）
    // arg 的类型取决于调用方式
}

int x = 42;
const int cx = 42;

func(x);        // T = int&        → T&& = int& &&  → int&
func(cx);       // T = const int&  → T&& = const int& && → const int&
func(42);       // T = int         → T&& = int&&
func(std::move(x)); // T = int     → T&& = int&&
```

引用折叠规则：

| 模板推导 | 结果 |
|---------|------|
| `T& &` | `T&` |
| `T& &&` | `T&` |
| `T&& &` | `T&` |
| `T&& &&` | `T&&` |

只有右值引用的右值引用折叠为右值引用，其余都是左值引用。

### 3.2 std::forward vs std::move

```cpp
// std::move: 无条件转为右值引用
template<typename T>
constexpr std::remove_reference_t<T>&& move(T&& t) noexcept {
    return static_cast<std::remove_reference_t<T>&&>(t);
}

// std::forward: 条件性转发（保持值类别）
// 左值版本
template<typename T>
constexpr T&& forward(std::remove_reference_t<T>& t) noexcept {
    return static_cast<T&&>(t);
}
// 右值版本
template<typename T>
constexpr T&& forward(std::remove_reference_t<T>&& t) noexcept {
    return static_cast<T&&>(t);
}

// 使用原则：
// - std::move:   "我要放弃这个值，你可以拿走它的资源"
// - std::forward: "我转发这个参数，保持它原本的值类别"
```

### 3.3 完美转发工厂函数

```cpp
template<typename T, typename... Args>
std::unique_ptr<T> makeUnique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// 使用示例
struct Widget {
    Widget(int a, std::string b, double c)
        : a_(a), b_(std::move(b)), c_(c) {}
    int a_;
    std::string b_;
    double c_;
};

auto w1 = makeUnique<Widget>(42, "hello", 3.14);
// "hello" 作为右值转发，Widget 构造时用移动
// 42, 3.14 是纯右值，同样被转发

std::string name = "world";
auto w2 = makeUnique<Widget>(1, name, 2.71);
// name 作为左值转发，Widget 构造时用拷贝
```

## 四、=default 与 =delete

### 4.1 特殊成员函数一览

```cpp
class Example {
public:
    Example();                           // 默认构造
    ~Example();                          // 析构
    Example(const Example&);             // 拷贝构造
    Example& operator=(const Example&);  // 拷贝赋值
    Example(Example&&);                  // 移动构造
    Example& operator=(Example&&);       // 移动赋值
};
```

编译器自动生成规则：声明了任何构造函数 → 不生成默认构造；声明了拷贝/移动/析构之一 → 不生成移动操作；声明了移动操作或析构 → 弃用拷贝生成。

### 4.2 使用场景

```cpp
// 场景1：只允许移动
class MoveOnly {
public:
    MoveOnly() = default;
    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
};

// 场景2：禁止堆分配（=delete operator new）
class StackOnly {
public:
    void* operator new(size_t) = delete;
    void* operator new[](size_t) = delete;
};

// 场景3：禁止特定类型转换
void foo(int x);
void foo(double) = delete;  // 禁止隐式转换

foo(42);     // OK
foo(3.14);   // 编译错误：已删除
```

## 五、三/五法则

### 5.1 法则总结

| 法则 | 内容 |
|------|------|
| 三法则 | 如果需要自定义析构、拷贝构造、拷贝赋值中任一个，很可能三者都需要 |
| 五法则 | 三法则 + 移动构造 + 移动赋值 |
| 零法则 | 如果不需要自定义任何特殊成员函数，就不要定义任何 |

```cpp
// 五法则示例：管理资源的类
class Resource {
    int* data_;
    size_t size_;
public:
    Resource(size_t n) : data_(new int[n]), size_(n) {}
    ~Resource() { delete[] data_; }

    Resource(const Resource& other)
        : data_(new int[other.size_]), size_(other.size_) {
        std::copy(other.data_, other.data_ + size_, data_);
    }

    Resource& operator=(const Resource& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new int[size_];
            std::copy(other.data_, other.data_ + size_, data_);
        }
        return *this;
    }

    Resource(Resource&& other) noexcept
        : data_(std::exchange(other.data_, nullptr))
        , size_(std::exchange(other.size_, 0)) {}

    Resource& operator=(Resource&& other) noexcept {
        delete[] data_;
        data_ = std::exchange(other.data_, nullptr);
        size_ = std::exchange(other.size_, 0);
        return *this;
    }
};

// 零法则示例：不管理资源，不定义任何特殊成员
class Point {
    double x_, y_;  // 编译器生成所有特殊成员，正确且高效
public:
    Point(double x, double y) : x_(x), y_(y) {}
};
```

## 六、运算符重载最佳实践

1. **保持语义一致**：`+` 不应修改操作数，`+=` 应修改左操作数
2. **用成员实现复合赋值**：`+=` 用成员，`+` 用友元并委托给 `+=`
3. **对称运算符用友元**：支持隐式转换两侧
4. **`<<` `>>` 必须用友元**：左操作数是流对象
5. **`==` 和 `!=` 成对出现**：用 `==` 实现 `!=`
6. **标记移动操作为 noexcept**：让标准库容器使用移动优化
7. **移动后对象应保持可析构状态**：至少将指针置为 nullptr
