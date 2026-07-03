# Week 3 知识点笔记：RAII 与智能指针

## 一、RAII 原理

### 1.1 核心思想

RAII（Resource Acquisition Is Initialization）是 C++ 最重要的资源管理惯用法：

- **资源在构造函数中获取**（打开文件、分配内存、加锁等）
- **资源在析构函数中释放**（关闭文件、释放内存、解锁等）
- **利用栈对象的生命周期自动管理资源**（离开作用域 → 析构 → 释放）

```cpp
class FileHandle {
    FILE* file_;
public:
    FileHandle(const char* path, const char* mode)
        : file_(std::fopen(path, mode)) {
        if (!file_) throw std::runtime_error("Failed to open file");
    }

    ~FileHandle() {
        if (file_) std::fclose(file_);
    }

    // 禁止拷贝（或实现深拷贝/移动）
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    // 允许移动
    FileHandle(FileHandle&& other) noexcept : file_(other.file_) {
        other.file_ = nullptr;
    }

    FILE* get() const { return file_; }
};

void example() {
    FileHandle f("data.txt", "r");  // 构造时打开文件
    // 使用 f ...
} // f 析构，文件自动关闭 — 即使发生异常
```

### 1.2 RAII 的三个关键性质

1. **异常安全**：即使抛出异常，栈展开也会调用析构函数
2. **确定性**：资源在确定的时间点释放（离开作用域时）
3. **组合性**：RAII 对象作为成员变量，外层对象析构时自动释放

### 1.3 标准库中的 RAII 示例

```cpp
// 文件流
std::ifstream file("data.txt");   // 构造打开，析构关闭
std::ofstream out("output.txt");

// 锁管理
std::mutex mtx;
std::lock_guard<std::mutex> lock(mtx);    // 构造加锁，析构解锁
std::unique_lock<std::mutex> ulock(mtx);  // 更灵活（可手动 unlock/lock）

// 智能指针
auto p = std::make_unique<int>(42);       // 构造分配，析构 delete
auto sp = std::make_shared<std::string>("hello");

// 容器
std::vector<int> v(1000);                 // 构造分配，析构释放
std::string s = "hello";                  // 同上
```

## 二、unique_ptr — 独占所有权

### 2.1 基本用法

```cpp
#include <memory>

// 创建
auto p1 = std::make_unique<int>(42);           // C++14: 推荐
std::unique_ptr<int> p2(new int(10));           // C++11: 可用但不推荐

// 访问
std::cout << *p1 << '\n';                       // 解引用
std::cout << p1.get() << '\n';                  // 获取裸指针

// 释放所有权（返回裸指针，调用者负责 delete）
int* raw = p1.release();                        // p1 变为 nullptr
delete raw;

// 重置
p2.reset(new int(99));                          // 释放旧对象，接管新对象
p2.reset();                                     // 仅释放，p2 变为 nullptr

// 所有权转移（不可拷贝，只能移动）
auto p3 = std::move(p2);                        // p2 变为 nullptr
// auto p4 = p3;                                // 编译错误：unique_ptr 不可拷贝
```

### 2.2 工厂函数返回 unique_ptr

```cpp
class Animal {
public:
    virtual ~Animal() = default;
    virtual void speak() const = 0;
};

class Dog : public Animal {
public:
    void speak() const override { std::cout << "Woof!\n"; }
};

class Cat : public Animal {
public:
    void speak() const override { std::cout << "Meow!\n"; }
};

std::unique_ptr<Animal> createAnimal(const std::string& type) {
    if (type == "dog") return std::make_unique<Dog>();
    if (type == "cat") return std::make_unique<Cat>();
    return nullptr;
}

void demo() {
    auto animal = createAnimal("dog");
    animal->speak();  // Woof!
} // animal 离开作用域，Dog 对象自动 delete
```

### 2.3 作为函数参数

```cpp
// 接收所有权：按值传递 unique_ptr
void sink(std::unique_ptr<Animal> pet) {
    pet->speak();
} // pet 析构，Animal 被 delete

// 只使用不接管：传递裸指针或引用
void use(const Animal& pet) {
    pet.speak();
}

// 调用
auto dog = std::make_unique<Dog>();
use(*dog);                  // 传递引用，不转移所有权
sink(std::move(dog));       // 转移所有权，dog 变为 nullptr
```

### 2.4 自定义删除器

```cpp
// 对于 FILE* 等非 new 分配的资源
auto fileDeleter = [](FILE* f) {
    if (f) std::fclose(f);
};
std::unique_ptr<FILE, decltype(fileDeleter)> file(
    std::fopen("data.txt", "r"), fileDeleter);
```

## 三、shared_ptr — 共享所有权

### 3.1 基本用法

```cpp
#include <memory>

// 创建
auto sp1 = std::make_shared<int>(42);            // 推荐
std::shared_ptr<int> sp2(new int(10));           // 可用

// 引用计数
sp1.use_count();                                 // 当前引用计数

// 共享所有权（拷贝增加引用计数）
auto sp3 = sp1;                                  // use_count: 2
auto sp4(sp1);                                   // use_count: 3

// 重置
sp3.reset();                                     // use_count: 2
sp1.reset();                                     // use_count: 1
sp2 = sp4;                                       // sp2 原对象引用计数-1，sp4 的+1

// 当最后一个 shared_ptr 销毁时，对象被 delete
```

### 3.2 内部结构

```
shared_ptr<Dog> sp1 ──→ [控制块: ref=1, weak=0] ──→ [Dog 对象]
shared_ptr<Dog> sp2 ──→ [同一控制块: ref=2, weak=0] ──→ [同一 Dog 对象]
shared_ptr<Dog> sp3 ──→ [同一控制块: ref=3, weak=0] ──→ [同一 Dog 对象]
```

- **对象**：实际分配的资源
- **控制块**：存储引用计数（强引用 ref_count，弱引用 weak_count）和删除器
- `make_shared` 将对象和控制块分配在一起（一次分配，性能更好）

### 3.3 make_shared vs new shared_ptr

```cpp
// 方式 1：make_shared（推荐）
auto sp = std::make_shared<LargeObject>(arg1, arg2);
// 优点：一次内存分配（对象+控制块在一起），异常安全
// 缺点：对象和控制块绑定，weak_ptr 存在时内存不能释放

// 方式 2：new + shared_ptr
std::shared_ptr<LargeObject> sp2(new LargeObject(arg1, arg2));
// 优点：对象和控制块分离，对象可以单独释放
// 缺点：两次分配，可能的内存泄漏（如果在构造 shared_ptr 前抛异常）
```

## 四、weak_ptr — 打破循环引用

### 4.1 循环引用问题

```cpp
struct Node {
    std::string name;
    std::shared_ptr<Node> next;   // 强引用
    std::shared_ptr<Node> prev;   // 强引用 — 导致循环引用！

    Node(const std::string& n) : name(n) {
        std::cout << "Node(" << name << ") created\n";
    }
    ~Node() {
        std::cout << "Node(" << name << ") destroyed\n";
    }
};

void createCycle() {
    auto a = std::make_shared<Node>("A");
    auto b = std::make_shared<Node>("B");

    a->next = b;
    b->prev = a;  // 循环引用形成！

    // a 和 b 离开作用域时：
    // a 的引用计数 = 2 (a 自身 + b->prev)
    // b 的引用计数 = 2 (b 自身 + a->next)
    // 离开作用域后，局部变量 a 和 b 销毁，但引用计数各为 1
    // → Node("A") 和 Node("B") 的析构函数永远不会被调用！
    // → 内存泄漏！
}
```

### 4.2 使用 weak_ptr 修复

```cpp
struct Node {
    std::string name;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;     // 弱引用 — 不增加引用计数

    Node(const std::string& n) : name(n) {
        std::cout << "Node(" << name << ") created\n";
    }
    ~Node() {
        std::cout << "Node(" << name << ") destroyed\n";
    }
};

void noLeak() {
    auto a = std::make_shared<Node>("A");
    auto b = std::make_shared<Node>("B");

    a->next = b;                  // b 引用计数 = 2
    b->prev = a;                  // a 引用计数 = 1 (weak_ptr 不增加)

    // 离开作用域时：
    // a 的引用计数 = 1 → 析构时变为 0 → Node("A") destroyed
    // b 的引用计数 = 1 → 析构时变为 0 → Node("B") destroyed
    // → 全部正确释放！
}
```

### 4.3 使用 weak_ptr

```cpp
void useWeakPtr() {
    auto sp = std::make_shared<int>(42);
    std::weak_ptr<int> wp = sp;    // 不增加引用计数

    // 检查对象是否仍然存在
    if (auto locked = wp.lock()) { // lock() 返回 shared_ptr
        std::cout << *locked << '\n'; // 42
    } // locked 离开作用域，引用计数-1

    sp.reset();                     // 释放对象
    // wp 现在是悬垂的

    if (auto locked = wp.lock()) {  // lock() 返回空 shared_ptr
        // 不会执行
    } else {
        std::cout << "Object already destroyed\n";
    }

    // 检查是否过期
    std::cout << "expired: " << wp.expired() << '\n'; // true
}
```

### 4.4 weak_ptr 使用场景

| 场景 | 说明 |
|------|------|
| 双向链表 | prev 用 weak_ptr 打破循环 |
| 父子关系 | 子对象持有父对象的 weak_ptr |
| 观察者模式 | 被观察者不控制观察者的生命周期 |
| 缓存 | 允许缓存的对象在外部释放后被回收 |

## 五、规则五（Rule of Five）

### 5.1 定义

如果一个类需要自定义以下五个函数中的任何一个，通常需要自定义全部五个：

1. **析构函数** `~T()`
2. **拷贝构造函数** `T(const T&)`
3. **拷贝赋值运算符** `T& operator=(const T&)`
4. **移动构造函数** `T(T&&) noexcept`
5. **移动赋值运算符** `T& operator=(T&&) noexcept`

### 5.2 完整实现示例

```cpp
#include <iostream>
#include <utility>   // std::exchange, std::move

class Buffer {
    size_t size_;
    int* data_;

public:
    // 构造函数
    explicit Buffer(size_t size)
        : size_(size), data_(new int[size]{}) {
        std::cout << "Buffer(size_t) constructor\n";
    }

    // 1. 析构函数
    ~Buffer() {
        std::cout << "~Buffer() destructor\n";
        delete[] data_;
    }

    // 2. 拷贝构造函数
    Buffer(const Buffer& other)
        : size_(other.size_), data_(new int[other.size_]) {
        std::cout << "Buffer(const Buffer&) copy constructor\n";
        std::copy(other.data_, other.data_ + size_, data_);
    }

    // 3. 拷贝赋值运算符
    Buffer& operator=(const Buffer& other) {
        std::cout << "operator=(const Buffer&) copy assignment\n";
        if (this == &other) return *this;  // 自赋值检查

        int* newData = new int[other.size_];
        std::copy(other.data_, other.data_ + other.size_, newData);

        delete[] data_;
        data_ = newData;
        size_ = other.size_;
        return *this;
    }

    // 4. 移动构造函数
    Buffer(Buffer&& other) noexcept
        : size_(std::exchange(other.size_, 0)),
          data_(std::exchange(other.data_, nullptr)) {
        std::cout << "Buffer(Buffer&&) move constructor\n";
    }

    // 5. 移动赋值运算符
    Buffer& operator=(Buffer&& other) noexcept {
        std::cout << "operator=(Buffer&&) move assignment\n";
        if (this == &other) return *this;

        delete[] data_;
        data_ = std::exchange(other.data_, nullptr);
        size_ = std::exchange(other.size_, 0);
        return *this;
    }

    // 访问器
    size_t size() const { return size_; }
    int* data() const { return data_; }
    int& operator[](size_t i) { return data_[i]; }
    const int& operator[](size_t i) const { return data_[i]; }
};
```

### 5.3 规则零（Rule of Zero）

如果类的成员变量都是 RAII 类型（如 `std::vector`、`std::string`、`std::unique_ptr`），则**不需要**自定义任何特殊成员函数：

```cpp
class Good {
    std::vector<int> data_;          // RAII 类型
    std::string name_;               // RAII 类型
    std::unique_ptr<Helper> helper_; // RAII 类型
public:
    Good() = default;
    // 编译器生成的析构、拷贝、移动都正确！
};
```

### 5.4 决策表

| 场景 | 规则 |
|------|------|
| 成员都是 RAII 类型 | 规则零：不定义任何特殊成员函数 |
| 管理原始资源（new/malloc/句柄） | 规则五：全部自定义 |
| 只可移动不可拷贝 | 定义移动操作，拷贝操作 `= delete` |
| 不可移动不可拷贝 | 全部 `= delete` |

## 六、智能指针选择指南

```
是否需要共享所有权？
├── 否 → unique_ptr
│   ├── 不需要自定义删除器 → make_unique
│   └── 需要自定义删除器 → unique_ptr<T, Deleter>
│
└── 是 → shared_ptr
    ├── 可能出现循环引用 → weak_ptr 配合
    └── 不会循环 → make_shared
```

| 指针类型 | 所有权 | 拷贝 | 典型场景 |
|---------|--------|------|---------|
| `unique_ptr` | 独占 | 不可拷贝，可移动 | 工厂函数、容器元素、PIMPL |
| `shared_ptr` | 共享 | 可拷贝（引用计数） | 共享缓存、异步回调 |
| `weak_ptr` | 观察 | 可拷贝 | 打破循环、观察者、缓存 |
| 裸指针 `T*` | 无 | 可拷贝 | 非所有权的函数参数 |

## 七、常见陷阱

### 7.1 不要用裸指针构造多个 shared_ptr

```cpp
// 危险！
int* raw = new int(42);
std::shared_ptr<int> sp1(raw);
std::shared_ptr<int> sp2(raw);  // 独立的控制块！双重释放！

// 正确：始终从已有的 shared_ptr 拷贝
auto sp1 = std::make_shared<int>(42);
auto sp2 = sp1;                  // 共享同一个控制块
```

### 7.2 不要在函数参数中混合 new 和 shared_ptr

```cpp
// 危险：如果 g() 抛异常，new 的对象泄漏
process(std::shared_ptr<int>(new int(42)), g());

// 正确：使用 make_shared 或分步
process(std::make_shared<int>(42), g());
```

### 7.3 enable_shared_from_this

```cpp
class Widget : public std::enable_shared_from_this<Widget> {
public:
    std::shared_ptr<Widget> getShared() {
        return shared_from_this();  // 安全地返回自身的 shared_ptr
    }
};

// 使用
auto w = std::make_shared<Widget>();
auto w2 = w->getShared();  // 共享同一个控制块
```

### 7.4 不要在栈上创建 shared_ptr 管理的对象

```cpp
int stackVar = 42;
// std::shared_ptr<int> sp(&stackVar);  // 危险！栈变量不能被 delete

// 正确：始终管理堆上的对象
auto sp = std::make_shared<int>(42);
```
