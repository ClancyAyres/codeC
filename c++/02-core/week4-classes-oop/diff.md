# Week 4：类与面向对象 — Java vs C++ 差异对照

## 一、无统一根类 Object

### 1.1 核心概念对照

| 概念 | Java | C++ |
|------|------|-----|
| 根类 | 所有类隐式继承 `java.lang.Object` | 无统一根类，类之间完全独立 |
| 通用方法 | `toString()`, `equals()`, `hashCode()`, `getClass()` | 无内置通用方法，按需自定义 |
| 类型擦除 | 通过 `Object` 实现泛型容器 | 模板（编译时生成），`void*`（不推荐） |
| 运行时类型信息 | `instanceof`, `getClass()` | `typeid`, `dynamic_cast` |

### 1.2 代码对照

```java
// Java: 所有类都有 Object 的方法
class Dog {
    String name;
    Dog(String name) { this.name = name; }

    @Override
    public String toString() { return "Dog: " + name; }

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof Dog)) return false;
        Dog other = (Dog) obj;
        return name.equals(other.name);
    }

    @Override
    public int hashCode() { return name.hashCode(); }
}
```

```cpp
// C++: 没有统一根类，方法完全自定义
class Dog {
    std::string name_;
public:
    Dog(std::string name) : name_(std::move(name)) {}

    // 自定义输出
    friend std::ostream& operator<<(std::ostream& os, const Dog& d) {
        return os << "Dog: " << d.name_;
    }

    // 自定义相等比较
    bool operator==(const Dog& other) const {
        return name_ == other.name_;
    }
};

// 如果需要放入无序容器，单独特化 std::hash
namespace std {
    template<> struct hash<Dog> {
        size_t operator()(const Dog& d) const {
            return hash<string>()(d.name_);  // 需声明 friend
        }
    };
}
```

## 二、无 interface 关键字 — 纯虚类

### 2.1 对照表

| 维度 | Java `interface` | C++ 纯虚类 |
|------|-----------------|-----------|
| 关键字 | `interface` | `class` + 纯虚函数 `= 0` |
| 多实现 | `class D implements A, B` | `class D : public A, public B` |
| 默认方法 | Java 8+ 支持 `default` 方法 | 普通虚函数可提供默认实现 |
| 常量 | `static final` 字段 | `static const` 成员 |
| 实例化 | 不可实例化 | 不可实例化（有纯虚函数） |

### 2.2 代码对照

```java
// Java: interface
interface Drawable {
    void draw();
    default void debug() { System.out.println("Drawable"); }
}

interface Clickable {
    void click();
}

class Button implements Drawable, Clickable {
    @Override public void draw() { /* ... */ }
    @Override public void click() { /* ... */ }
}
```

```cpp
// C++: 纯虚类模拟 interface
class Drawable {
public:
    virtual ~Drawable() = default;
    virtual void draw() = 0;
    virtual void debug() { std::cout << "Drawable\n"; }
};

class Clickable {
public:
    virtual ~Clickable() = default;
    virtual void click() = 0;
};

class Button : public Drawable, public Clickable {
public:
    void draw() override { /* ... */ }
    void click() override { /* ... */ }
};
```

## 三、值语义 vs 引用语义

### 3.1 核心差异

| 维度 | Java | C++ |
|------|------|-----|
| 对象变量 | 始终是引用（指针） | 可以是值、引用、指针 |
| 赋值语义 | 复制引用，指向同一对象 | 默认复制整个对象（拷贝构造） |
| 函数传参 | 始终传引用值 | 按需选择：值传递、引用传递、指针传递 |
| 多态 | 自动（所有变量是引用） | 需要引用或指针才能多态 |
| 对象切片 | 不存在 | 值传递派生类会切片 |

### 3.2 代码对照

```java
// Java: 始终是引用语义
Dog d1 = new Dog("Buddy");
Dog d2 = d1;                // d2 和 d1 指向同一对象
d2.setName("Max");
System.out.println(d1.getName()); // "Max" — d1 也被修改了
```

```cpp
// C++: 值语义
Dog d1("Buddy");
Dog d2 = d1;                // 拷贝构造，d2 是独立对象
d2.setName("Max");
std::cout << d1.getName();  // "Buddy" — d1 不受影响

// C++: 需要引用语义时显式使用指针或引用
Dog* p1 = new Dog("Buddy");
Dog* p2 = p1;               // p2 和 p1 指向同一对象
p2->setName("Max");
std::cout << p1->getName(); // "Max"
delete p1;
```

## 四、多重继承

### 4.1 对照表

| 维度 | Java | C++ |
|------|------|-----|
| 类的多重继承 | 不支持（单继承） | 支持 |
| 接口多继承 | 支持（`implements A, B`） | 对应纯虚类多继承 |
| 菱形问题 | 不存在（接口无状态） | 存在，需 `virtual` 继承解决 |
| 解决方式 | 接口方法冲突需手动重写 | `virtual` 继承共享基类子对象 |

### 4.2 代码对照

```java
// Java: 接口多继承，无菱形问题
interface A { default void foo() { System.out.println("A"); } }
interface B extends A { default void foo() { System.out.println("B"); } }
interface C extends A { default void foo() { System.out.println("C"); } }
// class D implements B, C { }  // 编译错误：foo() 冲突
class D implements B, C {
    @Override public void foo() { B.super.foo(); }  // 必须手动解决
}
```

```cpp
// C++: 类多继承，菱形问题用 virtual 解决
class A {
public:
    int value = 10;
    virtual ~A() = default;
};

class B : virtual public A { };  // virtual 继承
class C : virtual public A { };  // virtual 继承
class D : public B, public C { };
// D 中只有一个 A 子对象，value 无歧义
```

## 五、访问控制差异

### 5.1 对照表

| Java | C++ | 含义 |
|------|-----|------|
| `public` | `public` | 所有代码可访问 |
| `protected` | `protected` | 子类可访问（Java 还允许同包访问） |
| （包私有，默认） | 无对应 | Java 独有：同一包内可访问 |
| `private` | `private` | 只有本类可访问 |
| 无 | `friend` | 授权特定函数/类访问私有成员 |

### 5.2 继承中的访问控制（C++ 独有）

| 继承方式 | 基类 public → 派生类 | 基类 protected → 派生类 | 基类 private → 派生类 |
|---------|---------------------|------------------------|----------------------|
| `public` 继承 | public | protected | 不可访问 |
| `protected` 继承 | protected | protected | 不可访问 |
| `private` 继承 | private | private | 不可访问 |

Java 没有继承访问修饰符的概念。

## 六、final 关键字差异

| 维度 | Java `final` | C++ `final` |
|------|-------------|------------|
| 禁止继承 | `final class X` | `class X final` |
| 禁止重写 | `final void method()` | `void method() final` |
| 禁止赋值 | `final int x = 5;` | `const int x = 5;` |

## 七、快速参考卡

| 场景 | Java 做法 | C++ 做法 |
|------|----------|---------|
| 定义接口 | `interface Drawable { void draw(); }` | `class Drawable { virtual void draw() = 0; };` |
| 实现接口 | `class Button implements Drawable` | `class Button : public Drawable` |
| 多态调用 | 自动（引用语义） | 需要指针或引用 |
| 禁止继承 | `final class Util` | `class Util final` |
| 禁止重写 | `final void method()` | `void method() final` |
| 抽象类 | `abstract class Base` | `class Base { virtual void f() = 0; };` |
| 根类方法 | 继承自 `Object` | 自定义（无统一根类） |
| 类型转换 | `(SubType) obj` | `static_cast`, `dynamic_cast` |
| 菱形继承 | 不存在（接口无状态） | `virtual` 继承解决 |
| 私有成员访问 | 无 `friend` 概念 | `friend` 函数/类 |
