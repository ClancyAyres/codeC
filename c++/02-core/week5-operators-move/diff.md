# Week 5：运算符重载与移动语义 — Java vs C++ 差异对照

## 一、运算符重载

### 1.1 核心概念对照

| 概念 | Java | C++ |
|------|------|-----|
| 运算符重载 | 不支持（语言设计者认为易滥用） | 完全支持，除少数运算符外皆可重载 |
| 相等比较 | `equals()` 方法 | `operator==` |
| 字符串表示 | `toString()` 方法 | `operator<<` 或自定义方法 |
| 加法/拼接 | 数值用 `+`，字符串用 `+`（编译器特殊处理） | 任何类型可重载 `operator+` |
| 下标访问 | `list.get(i)` / `array[i]` | `operator[]` |
| 函数调用 | 无对应 | `operator()`（函子） |
| 赋值语义 | 引用赋值（不可重载） | 可重载 `operator=`（拷贝/移动赋值） |

### 1.2 不可重载的运算符

| 运算符 | 说明 |
|--------|------|
| `::` | 作用域解析 |
| `.` | 成员访问 |
| `.*` | 成员指针访问 |
| `?:` | 三元条件 |
| `sizeof` | 编译时大小 |
| `typeid` | 运行时类型信息 |
| `static_cast` / `dynamic_cast` / `const_cast` / `reinterpret_cast` | 类型转换 |

### 1.3 代码对照

```java
// Java: 方法调用替代运算符
public class Complex {
    private double real, imag;

    public Complex(double r, double i) { real = r; imag = i; }

    public Complex add(Complex other) {
        return new Complex(real + other.real, imag + other.imag);
    }

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof Complex)) return false;
        Complex c = (Complex) obj;
        return real == c.real && imag == c.imag;
    }

    @Override
    public String toString() {
        return real + " + " + imag + "i";
    }

    // 使用：c1.add(c2) 而非 c1 + c2
}
```

```cpp
// C++: 运算符重载，自然语法
class Complex {
    double real_, imag_;
public:
    Complex(double r, double i) : real_(r), imag_(i) {}

    Complex operator+(const Complex& other) const {
        return Complex(real_ + other.real_, imag_ + other.imag_);
    }

    bool operator==(const Complex& other) const {
        return real_ == other.real_ && imag_ == other.imag_;
    }

    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        return os << c.real_ << " + " << c.imag_ << "i";
    }

    // 使用：c1 + c2, c1 == c2, cout << c1
};
```

## 二、移动语义 — C++11 最重要的特性

### 2.1 概念对照

| 概念 | Java | C++ |
|------|------|-----|
| 移动语义 | 不存在（所有对象引用，无需移动） | C++11 引入，通过 `T&&` 右值引用实现 |
| 资源转移 | GC 自动管理，无资源转移概念 | `std::move` 语义转移（不移动数据本身） |
| 拷贝 vs 移动 | 只有引用拷贝（浅拷贝） | 拷贝构造/赋值 vs 移动构造/赋值 |
| 临时对象优化 | JIT 可能优化 | 移动语义 + RVO/NRVO |
| 所有权转移 | 不存在 | `std::unique_ptr` 独占所有权 |

### 2.2 左值与右值

```cpp
int a = 42;           // a 是左值（有名字、可取地址）
int& lref = a;        // 左值引用：绑定到左值
int&& rref = 42;      // 右值引用：绑定到右值（临时对象、字面量）
int&& rref2 = a + 1;  // 右值引用：绑定到表达式结果（临时值）

// std::move：将左值转为右值引用（语义标记，不移动数据本身）
int&& rref3 = std::move(a);  // a 现在可被移动

// 关键认知：std::move 不移动任何东西，只做类型转换
```

### 2.3 移动构造/赋值 vs 拷贝构造/赋值

```cpp
class Buffer {
    char* data_;
    size_t size_;
public:
    // 拷贝构造：深拷贝，昂贵
    Buffer(const Buffer& other)
        : data_(new char[other.size_])
        , size_(other.size_) {
        std::copy(other.data_, other.data_ + size_, data_);
    }

    // 移动构造：转移所有权，O(1)
    Buffer(Buffer&& other) noexcept
        : data_(std::exchange(other.data_, nullptr))
        , size_(std::exchange(other.size_, 0)) {
    }

    // 拷贝赋值
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new char[size_];
            std::copy(other.data_, other.data_ + size_, data_);
        }
        return *this;
    }

    // 移动赋值
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = std::exchange(other.data_, nullptr);
            size_ = std::exchange(other.size_, 0);
        }
        return *this;
    }

    ~Buffer() { delete[] data_; }
};
```

### 2.4 移动语义的性能意义

```cpp
// 没有移动语义：vector 扩容时拷贝所有元素
std::vector<std::string> v;
v.push_back(std::string(1000, 'x'));  // C++03: 拷贝 1000 个字符
v.push_back(std::string(1000, 'x'));  // 扩容：拷贝第一个字符串的 1000 字符

// 有移动语义：vector 扩容时移动所有元素（O(1) 每个）
std::vector<std::string> v;
v.push_back(std::string(1000, 'x'));  // C++11: 直接构造
v.push_back(std::string(1000, 'x'));  // 扩容：移动第一个字符串（交换指针）
```

## 三、完美转发

### 3.1 概念

| 概念 | Java | C++ |
|------|------|-----|
| 完美转发 | 不存在（无值类别概念） | `std::forward<T>(arg)` 保持参数值类别 |
| 万能引用 | 无 | `T&&` 在模板推导上下文中是万能引用 |
| 引用折叠 | 无 | `T& &` → `T&`, `T&& &` → `T&`, `T& &&` → `T&`, `T&& &&` → `T&&` |

### 3.2 代码对照

```cpp
// 万能引用：T&& 在模板推导上下文中可绑定左值或右值
template<typename T>
void wrapper(T&& arg) {          // T&& 是万能引用（不是右值引用）
    // std::forward 保持 arg 的值类别
    process(std::forward<T>(arg));
}

// 使用
int x = 42;
wrapper(x);       // T = int&,  arg 是左值引用, forward 传左值
wrapper(42);      // T = int,   arg 是右值引用, forward 传右值
wrapper(std::move(x));  // T = int, arg 是右值引用, forward 传右值

// 典型应用：工厂函数
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
```

## 四、=default 和 =delete

### 4.1 对照表

| 目的 | Java | C++ |
|------|------|-----|
| 使用编译器默认实现 | 自动（无需声明） | `= default` |
| 禁止某个操作 | 不提供该方法即可 | `= delete` |
| 禁止拷贝 | 无原生支持（可用 `CloneNotSupportedException`） | `= delete` 拷贝构造/赋值 |
| 只允许移动 | 无对应概念 | 拷贝 `= delete`，移动 `= default` |

### 4.2 代码对照

```cpp
// 只允许移动的类
class MoveOnly {
public:
    MoveOnly() = default;
    MoveOnly(MoveOnly&&) = default;            // 允许移动
    MoveOnly& operator=(MoveOnly&&) = default;
    MoveOnly(const MoveOnly&) = delete;         // 禁止拷贝
    MoveOnly& operator=(const MoveOnly&) = delete;
};
```

## 五、快速参考卡

| 场景 | Java 做法 | C++ 做法 |
|------|----------|---------|
| 相等比较 | `obj.equals(other)` | `operator==` |
| 字符串表示 | `obj.toString()` | `operator<<` 或自定义 |
| 加法 | `c1.add(c2)` | `operator+` |
| 下标访问 | `list.get(i)` | `operator[]` |
| 函数对象 | Lambda / 匿名类 | Lambda / `operator()` |
| 禁止拷贝 | 不提供 clone | `= delete` |
| 资源转移 | 不需要（GC） | `std::move` + 移动语义 |
| 工厂转发 | 无对应 | `std::forward` |
| 临时对象优化 | JIT | 移动语义 |
| 深拷贝 | `clone()` | 拷贝构造 |
