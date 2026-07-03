# Week 4 知识点笔记：类与面向对象

## 一、构造函数类型

### 1.1 默认构造函数

```cpp
class Widget {
    int id_;
    std::string name_;
public:
    Widget() : id_(0), name_("default") {}  // 默认构造函数
};

Widget w;          // 调用默认构造
Widget w{};        // 值初始化（C++11）
```

注意：如果声明了任何构造函数，编译器不再生成默认构造函数。

### 1.2 参数化构造函数

```cpp
class Widget {
    int id_;
    std::string name_;
public:
    Widget(int id, const std::string& name)
        : id_(id), name_(name) {}

    // C++11: 委托构造函数
    Widget(int id) : Widget(id, "unnamed") {}
    Widget() : Widget(0) {}
};
```

### 1.3 拷贝构造函数

```cpp
class Widget {
    int* data_;
    size_t size_;
public:
    Widget(const Widget& other)
        : data_(new int[other.size_]),
          size_(other.size_) {
        std::copy(other.data_, other.data_ + size_, data_);
    }
};
```

调用时机：
- `Widget w2 = w1;` — 拷贝初始化
- `Widget w3(w1);` — 直接初始化
- 按值传递对象 — 函数参数
- 按值返回对象 — 函数返回值（可能被 RVO 优化掉）

### 1.4 移动构造函数

```cpp
class Widget {
    int* data_;
    size_t size_;
public:
    Widget(Widget&& other) noexcept
        : data_(std::exchange(other.data_, nullptr)),
          size_(std::exchange(other.size_, 0)) {
    }
};
```

### 1.5 委托构造函数

```cpp
class Point {
    double x_, y_;
public:
    Point(double x, double y) : x_(x), y_(y) {}
    Point(double v) : Point(v, v) {}       // 委托到 (x,y)
    Point() : Point(0.0, 0.0) {}            // 委托到 (v) → (x,y)
};
```

## 二、成员初始化列表

### 2.1 基本语法

```cpp
class Person {
    std::string name_;
    int age_;
    const int id_;           // const 成员
    std::string& ref_;       // 引用成员
public:
    Person(const std::string& name, int age, int id, std::string& ref)
        : name_(name)        // 直接构造，而非先默认构造再赋值
        , age_(age)
        , id_(id)            // const 成员：必须在初始化列表中初始化
        , ref_(ref)          // 引用成员：必须在初始化列表中初始化
    {
    }
};
```

### 2.2 必须使用初始化列表的情况

1. **const 成员变量** — 只能在初始化列表中初始化
2. **引用成员变量** — 引用必须在定义时绑定
3. **没有默认构造函数的成员对象** — 必须在初始化列表中显式构造
4. **基类没有默认构造函数** — 必须在初始化列表中调用基类构造函数
5. **成员变量名与参数名相同时** — 避免歧义

### 2.3 初始化列表 vs 函数体内赋值

```cpp
// 初始化列表：直接构造（效率高）
class Good {
    std::string s_;
public:
    Good(const std::string& s) : s_(s) {}
    // s_ 直接由 s 拷贝构造 — 一次 string 拷贝构造
};

// 函数体内赋值：先默认构造再赋值（效率低）
class Bad {
    std::string s_;
public:
    Bad(const std::string& s) {
        s_ = s;  // s_ 先默认构造（空串），再赋值 — 两次操作
    }
};
```

### 2.4 初始化顺序

成员变量按**声明顺序**初始化，而非初始化列表中的顺序：

```cpp
class Widget {
    int a_;
    int b_;
public:
    Widget(int x) : b_(x), a_(b_ + 1) {}  // 危险：a_ 先于 b_ 初始化！
    // a_ 使用未初始化的 b_ — 未定义行为
};
```

## 三、虚函数与多态

### 3.1 基本概念

```cpp
class Animal {
public:
    virtual ~Animal() = default;          // 虚析构 — 基类必须
    virtual void speak() const {          // 虚函数
        std::cout << "Animal sound\n";
    }
    void eat() const {                    // 非虚函数
        std::cout << "Animal eating\n";
    }
};

class Dog : public Animal {
public:
    void speak() const override {         // override: 明确表示重写
        std::cout << "Woof!\n";
    }
    void eat() const {                    // 隐藏（非重写）
        std::cout << "Dog eating\n";
    }
};

void demo() {
    Dog dog;
    Animal& ref = dog;
    ref.speak();  // "Woof!" — 动态绑定
    ref.eat();    // "Animal eating" — 静态绑定
}
```

### 3.2 虚函数表（vtable）

```
对象内存布局：
Dog 对象:
┌──────────────┐
│ vptr ────────│──→ Dog vtable:
├──────────────┤    ┌──────────────────┐
│ Dog 成员...  │    │ &Dog::speak()    │
└──────────────┘    │ &Animal::~Animal()│
                    └──────────────────┘
```

- 每个有虚函数的类有一个 vtable
- 每个对象有一个 vptr（虚表指针）
- 调用虚函数 = 通过 vptr → vtable → 函数地址（一次间接跳转）
- 开销：一个指针大小 + 一次间接寻址

### 3.3 override 和 final

```cpp
class Base {
public:
    virtual void f() const;
    virtual void g();
    virtual void h() final;   // 禁止子类重写
};

class Derived : public Base {
public:
    void f() const override;  // 明确重写，编译器检查签名匹配
    // void f() override;     // 编译错误：缺少 const
    void g() final;           // 重写并禁止进一步重写
    // void h() override;     // 编译错误：Base::h 是 final
};

class MoreDerived : public Derived {
public:
    // void g() override;     // 编译错误：Derived::g 是 final
};
```

### 3.4 纯虚函数与抽象类

```cpp
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;      // 纯虚函数
    virtual double perimeter() const = 0;  // 纯虚函数

    // 可以有非虚实现（通过 Shape::area() 调用）
};

// double Shape::area() const { return 0; }  // 可选的默认实现

class Circle : public Shape {
    double r_;
public:
    Circle(double r) : r_(r) {}
    double area() const override { return 3.14159 * r_ * r_; }
    double perimeter() const override { return 2 * 3.14159 * r_; }
};

// Shape s;  // 编译错误：不能实例化抽象类
std::unique_ptr<Shape> s = std::make_unique<Circle>(5.0);
```

## 四、对象切片（Object Slicing）

### 4.1 问题描述

```cpp
class Base {
protected:
    int baseData_ = 10;
public:
    virtual ~Base() = default;
    virtual void print() const {
        std::cout << "Base: " << baseData_ << '\n';
    }
};

class Derived : public Base {
    int extraData_ = 20;
public:
    void print() const override {
        std::cout << "Derived: base=" << baseData_
                  << " extra=" << extraData_ << '\n';
    }
};

// 危险：按值传递导致切片
void processByValue(Base b) {  // 拷贝构造 Base 部分
    b.print();  // 始终调用 Base::print()
}

// 安全：按引用传递
void processByRef(const Base& b) {
    b.print();  // 虚函数调用，正确多态
}

void demo() {
    Derived d;
    processByValue(d);  // 切片！extraData_ 丢失
    processByRef(d);    // 正确
}
```

### 4.2 切片发生的场景

- 按值传递派生类对象给基类参数
- 将派生类对象赋值给基类变量：`Base b = d;`
- 基类容器存储值：`std::vector<Base>` 存派生类

### 4.3 解决方案

- 使用引用或指针：`const Base&` 或 `Base*` 或 `std::unique_ptr<Base>`
- 使用 `std::vector<std::unique_ptr<Base>>` 替代 `std::vector<Base>`
- 基类的拷贝构造设为 `protected` 或 `= delete`

## 五、this 指针

```cpp
class Widget {
    int id_;
public:
    Widget(int id) : id_(id) {}

    Widget& setId(int id) {
        this->id_ = id;   // 显式使用 this 区分成员和参数
        return *this;      // 返回自身引用（链式调用）
    }

    Widget* getPtr() {
        return this;       // 返回自身指针
    }

    const Widget* getPtr() const {
        return this;       // const 成员函数中 this 是 const Widget*
    }
};
```

## 六、静态成员

```cpp
class Counter {
    static int count_;             // 静态成员变量声明
    static const int MAX = 100;    // const static 整型可以类内初始化
public:
    Counter() { ++count_; }
    ~Counter() { --count_; }

    static int getCount() {        // 静态成员函数
        return count_;
        // 不能访问 this（没有 this）
        // 不能访问非静态成员
    }

    static Counter* create() {     // 静态工厂方法
        return new Counter();
    }
};

int Counter::count_ = 0;           // 静态成员变量定义（在 .cpp 中）
```

## 七、friend

```cpp
class Widget {
    int secret_ = 42;

    // 友元函数：可以访问私有成员
    friend void debugPrint(const Widget& w);

    // 友元类：可以访问私有成员
    friend class WidgetTester;

    // 友元成员函数
    friend void OtherClass::inspect(const Widget& w);
public:
    int getSecret() const { return secret_; }
};

void debugPrint(const Widget& w) {
    std::cout << "Secret: " << w.secret_ << '\n';  // 可访问 private
}
```

**friend 的使用原则**：尽量少用，但运算符重载（`operator<<`）是合理的例外。

## 八、菱形继承与 virtual 继承

### 8.1 菱形问题

```
     A
    / \
   B   C
    \ /
     D
```

没有 virtual 继承时，D 包含两个 A 子对象：

```cpp
class A { public: int value = 10; };
class B : public A { };
class C : public A { };
class D : public B, public C { };

D d;
// d.value = 5;   // 歧义！是 B::A::value 还是 C::A::value？
d.B::value = 5;   // 必须显式指定路径
```

### 8.2 virtual 继承解决方案

```cpp
class A { public: int value = 10; };
class B : virtual public A { };    // virtual 继承
class C : virtual public A { };    // virtual 继承
class D : public B, public C { };

D d;
d.value = 5;   // 只有一个 A 子对象，无歧义
```

### 8.3 virtual 继承的代价

- 对象大小增加（额外的 vptr 或偏移量表）
- 访问基类成员多一次间接寻址
- 构造函数中 virtual 基类由最终派生类负责初始化

```cpp
class A {
    int x_;
public:
    A(int x) : x_(x) {}
};

class B : virtual public A {
public:
    B(int x) : A(x) {}  // 如果 B 不是最终类，此初始化被忽略
};

class C : virtual public A {
public:
    C(int x) : A(x) {}
};

class D : public B, public C {
public:
    D(int x) : A(x), B(x), C(x) {}  // D 负责初始化 A
};
```

## 九、组合优于继承

```cpp
// 继承方式：强耦合
class Bird {
public:
    virtual void fly() { /* ... */ }
};

class Penguin : public Bird {
public:
    void fly() override { throw std::runtime_error("Can't fly!"); }
    // 违反 Liskov 替换原则
};

// 组合方式：松耦合
class FlyBehavior {
public:
    virtual void fly() = 0;
    virtual ~FlyBehavior() = default;
};

class CanFly : public FlyBehavior {
public:
    void fly() override { std::cout << "Flying!\n"; }
};

class CannotFly : public FlyBehavior {
public:
    void fly() override { std::cout << "Cannot fly\n"; }
};

class Bird {
    std::unique_ptr<FlyBehavior> flyBehavior_;
public:
    Bird(std::unique_ptr<FlyBehavior> fb)
        : flyBehavior_(std::move(fb)) {}
    void performFly() { flyBehavior_->fly(); }
};

// Penguin penguin(std::make_unique<CannotFly>());
```
