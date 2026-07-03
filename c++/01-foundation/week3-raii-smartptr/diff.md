# Week 3：RAII 与智能指针 — Java vs C++ 差异对照

## 一、try-with-resources vs RAII

### 1.1 核心概念对照

| 概念 | Java | C++ |
|------|------|-----|
| 资源管理范式 | try-with-resources (Java 7+) | RAII (Resource Acquisition Is Initialization) |
| 触发时机 | 显式声明，离开 try 块时调用 close() | 对象离开作用域时自动调用析构函数 |
| 接口要求 | 实现 `AutoCloseable` 接口 | 实现析构函数 `~T()` |
| 异常安全 | try 块内异常会触发 close() | 栈展开时自动调用析构函数 |
| 语法复杂度 | 需要 try 块包裹 | 无需额外语法，声明即管理 |

### 1.2 代码对照

```java
// Java: try-with-resources
try (FileInputStream fis = new FileInputStream("data.txt");
     BufferedReader br = new BufferedReader(new InputStreamReader(fis))) {
    String line = br.readLine();
    System.out.println(line);
} // fis.close() 和 br.close() 自动调用，即使发生异常
```

```cpp
// C++: RAII — 无需 try 块，声明即管理
void readFile() {
    std::ifstream file("data.txt");  // 构造时打开
    std::string line;
    std::getline(file, line);        // 使用
    std::cout << line << '\n';
} // file 离开作用域，析构函数自动关闭文件，即使发生异常
```

### 1.3 自定义 RAII 类

```java
// Java: 实现 AutoCloseable
class DatabaseConnection implements AutoCloseable {
    public DatabaseConnection() {
        System.out.println("Database opened");
    }
    @Override
    public void close() {
        System.out.println("Database closed");
    }
}

// 使用
try (DatabaseConnection db = new DatabaseConnection()) {
    db.query("SELECT ...");
} // db.close() 自动调用
```

```cpp
// C++: 利用析构函数
class DatabaseConnection {
public:
    DatabaseConnection() { std::cout << "Database opened\n"; }
    ~DatabaseConnection() { std::cout << "Database closed\n"; }
    void query(const std::string& sql) { /* ... */ }
};

// 使用
void doWork() {
    DatabaseConnection db;  // 构造时打开
    db.query("SELECT ...");
} // db 离开作用域，析构函数自动关闭
```

## 二、finalize() vs 析构函数

### 2.1 对照表

| 维度 | Java `finalize()` | C++ 析构函数 `~T()` |
|------|-------------------|---------------------|
| 调用时机 | GC 回收对象时调用（不确定） | 对象生命周期结束时确定调用 |
| 调用顺序 | 不确定 | 构造顺序的逆序（栈上对象） |
| 是否可靠 | 不可靠（Java 9 已废弃，Java 18+ 建议用 Cleaner） | 完全可靠，语义保证 |
| 资源释放 | 不应依赖 finalize 释放关键资源 | 析构函数是释放资源的唯一正确位置 |
| 性能影响 | finalize 对象回收慢 | 无额外开销 |

### 2.2 代码对照

```java
// Java: finalize 不可靠
class Resource {
    protected void finalize() {
        // 可能永远不被调用，不应依赖它释放关键资源
        System.out.println("finalize called — maybe never");
    }
}
```

```cpp
// C++: 析构函数确定调用
class Resource {
public:
    Resource() { std::cout << "acquired\n"; }
    ~Resource() {
        std::cout << "released\n";  // 保证调用
    }
};
```

## 三、GC vs 确定性释放

### 3.1 核心差异

| 维度 | Java GC | C++ 析构函数 |
|------|---------|-------------|
| 内存回收 | GC 自动回收不可达对象 | `delete` 或智能指针显式释放 |
| 非内存资源 | 必须用 try-with-resources | 析构函数天然支持（RAII） |
| 释放时机 | 不确定（GC 决定） | 确定（离开作用域或 delete 时） |
| 循环引用 | GC 可处理（可达性分析） | `shared_ptr` 循环引用导致泄漏，需 `weak_ptr` |

### 3.2 互斥锁示例

```java
// Java: 必须手动 unlock
Lock lock = new ReentrantLock();
lock.lock();
try {
    // 临界区
} finally {
    lock.unlock();  // 必须手动释放！
}
```

```cpp
// C++: RAII 自动管理
{
    std::lock_guard<std::mutex> guard(mtx);  // 构造时加锁
    // 临界区
} // guard 析构自动解锁，即使异常发生
```

## 四、Cloneable.clone() vs 拷贝构造函数

### 4.1 对照表

| 维度 | Java `Cloneable` + `clone()` | C++ 拷贝构造函数 |
|------|------------------------------|------------------|
| 接口 | 实现标记接口 `Cloneable`，重写 `clone()` | 定义 `T(const T&)` |
| 返回值 | `Object`（需强制转型） | `T`（类型安全） |
| 深浅拷贝 | 默认浅拷贝，深拷贝需手动实现 | 完全由开发者控制 |
| 异常 | `clone()` 抛出 `CloneNotSupportedException` | 不抛异常（通常标记 `noexcept`） |
| 语义 | 约定模糊，实现不一致 | 语言级别定义，语义明确 |

### 4.2 代码对照

```java
// Java: clone
class Dog implements Cloneable {
    String name;
    Dog(String name) { this.name = name; }

    @Override
    public Dog clone() {
        try {
            return (Dog) super.clone();  // 浅拷贝
        } catch (CloneNotSupportedException e) {
            throw new RuntimeException(e);
        }
    }
}

Dog d1 = new Dog("Buddy");
Dog d2 = d1.clone();          // 需要显式调用，需要强制转型
```

```cpp
// C++: 拷贝构造
class Dog {
    std::string name;
public:
    Dog(const std::string& n) : name(n) {}
    Dog(const Dog& other) : name(other.name) {}  // 拷贝构造
};

Dog d1("Buddy");
Dog d2 = d1;                  // 自动调用拷贝构造
Dog d3(d1);                   // 等价写法
```

## 五、移动语义（C++ 独有）

Java 没有移动语义的概念，因为所有对象变量都是引用：

```java
// Java: 始终是引用语义
Dog d1 = new Dog("Buddy");
Dog d2 = d1;                  // d2 和 d1 指向同一个对象
// 没有"转移所有权"的概念，GC 管理一切
```

```cpp
// C++: 移动语义
std::unique_ptr<Dog> p1 = std::make_unique<Dog>("Buddy");
std::unique_ptr<Dog> p2 = std::move(p1);  // 所有权转移
// p1 现在是 nullptr，p2 拥有 Dog 对象
```

## 六、快速参考卡

| 场景 | Java 做法 | C++ 做法 |
|------|----------|---------|
| 文件操作 | `try (FileInputStream fis = ...)` | `std::ifstream file("...");` |
| 数据库连接 | `try (Connection conn = ...)` | `DatabaseConnection conn;` 或 RAII 包装 |
| 锁管理 | `lock.lock(); try { ... } finally { lock.unlock(); }` | `std::lock_guard<std::mutex> guard(mtx);` |
| 深拷贝 | 实现 `Cloneable`，重写 `clone()` | 实现拷贝构造函数 `T(const T&)` |
| 所有权转移 | 无此概念 | `std::move()` 或 `std::unique_ptr` |
| 共享所有权 | 所有引用都是共享的（GC 管理） | `std::shared_ptr` |
| 避免循环引用 | GC 自动处理 | `std::weak_ptr` 打破 shared_ptr 循环 |
| 资源获取即初始化 | try-with-resources | RAII（构造获取，析构释放） |
