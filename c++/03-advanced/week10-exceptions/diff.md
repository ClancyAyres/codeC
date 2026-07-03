# Week 10：异常安全与错误处理 — Java vs C++ 差异对照

## 一、checked exception — Java 有，C++ 没有

```java
// Java — checked exception，调用者必须处理
import java.io.*;

public void readConfig(String path) throws IOException {  // 声明可能抛出的 checked exception
    try (BufferedReader br = new BufferedReader(new FileReader(path))) {
        String line = br.readLine();
        // ...
    }
}
// 调用者必须 catch IOException 或继续声明 throws
```

```cpp
// C++ — 没有 checked exception，异常规范已废弃
#include <fstream>
#include <string>

void readConfig(const std::string& path) {
    std::ifstream file(path);
    if (!file) throw std::runtime_error("Cannot open: " + path);
    std::string line;
    std::getline(file, line);
    // ...
}
// 调用者可以完全不 catch — 异常沿调用栈向上传播，直到被 catch 或 terminate
```

| 特性 | Java checked exception | C++ |
|------|----------------------|-----|
| 存在性 | 有，编译器强制处理 | 无，历史上 `throw()` 和 `noexcept(false)` 是另的事 |
| 声明方式 | `throws IOException` | 无（`throw(X)` 已废弃，`noexcept` 是保证不抛） |
| 编译时检查 | 不处理则编译错误 | 无检查，完全运行时 |
| 理念 | 强制错误处理 | 认为 checked exception 弊大于利 |

## 二、finally → RAII

```java
// Java — finally 块手动清理
Connection conn = null;
try {
    conn = DriverManager.getConnection(url);
    PreparedStatement stmt = conn.prepareStatement(sql);
    try {
        ResultSet rs = stmt.executeQuery();
        try {
            // 使用 rs ...
        } finally {
            rs.close();     // 每一层都要 finally 关闭
        }
    } finally {
        stmt.close();
    }
} finally {
    if (conn != null) conn.close();
}

// Java 7+ try-with-resources 改善了许多
try (Connection conn = DriverManager.getConnection(url);
     PreparedStatement stmt = conn.prepareStatement(sql);
     ResultSet rs = stmt.executeQuery()) {
    // 使用 rs ...
}
```

```cpp
// C++ — RAII，析构函数自动清理，无需 finally
#include <fstream>
#include <string>

void process() {
    std::ifstream file("data.txt");     // 构造 → 打开
    std::string line;
    std::getline(file, line);

    // 离开作用域 → file 析构 → 自动关闭
    // 无论正常返回还是异常抛出，析构都保证执行
}

// 管理动态内存 / 锁 同理
void work() {
    auto ptr = std::make_unique<Resource>();
    std::lock_guard<std::mutex> lock(mtx);

    // ... 可能抛异常的操作 ...

    // ptr 和 lock 析构自动清理，无需 finally
}
```

| 方面 | Java finally / try-with-resources | C++ RAII |
|------|----------------------------------|----------|
| 原理 | 显式在 finally 块中清理 | 析构函数自动清理 |
| 适用范围 | 实现 AutoCloseable 的类 | 所有有析构函数的类型 |
| 嵌套清理 | try-with-resources 简化 | 无需任何额外代码 |
| 忘记清理 | 编译器提醒（try-with-resources）或泄露 | 自动清理，不可能忘记 |

## 三、Optional → std::optional

```java
// Java — Optional (Java 8+)，主要用于返回值
import java.util.Optional;

Optional<String> findUser(int id) {
    if (id > 0) return Optional.of("user_" + id);
    return Optional.empty();
}

// 使用
String name = findUser(42).orElse("unknown");
findUser(1).ifPresent(System.out::println);
```

```cpp
// C++ — std::optional (C++17)
#include <optional>
#include <string>

std::optional<std::string> findUser(int id) {
    if (id > 0) return "user_" + std::to_string(id);
    return std::nullopt;
}

// 使用
std::string name = findUser(42).value_or("unknown");
if (auto user = findUser(1)) {
    std::cout << *user << '\n';
}
```

| 操作 | Java Optional | C++ std::optional |
|------|-------------|-------------------|
| 空值 | `Optional.empty()` | `std::nullopt` 或 `{}` |
| 检查有值 | `isPresent()` | `has_value()` 或 `operator bool` |
| 获取值 | `get()`（空时 NoSuchElementException） | `value()`（空时 bad_optional_access） |
| 解引用 | 无 | `operator*` / `operator->`（空时 UB） |
| 默认值 | `orElse(T)` | `value_or(T)` |
| 条件执行 | `ifPresent(Consumer)` | 无直接等价，用 `if (opt)` |

## 四、异常规范 — 方向相反

```java
// Java — 声明"会抛出什么"
public void risky() throws IOException, SQLException {
    // 可能抛 IOException 或 SQLException
}

// 没有声明 → 不抛 checked exception（但还可以抛 RuntimeException）
```

```cpp
// C++ — 声明"保证不抛出什么"
void safe() noexcept {        // C++11+，保证不抛
    // 如果内部抛了 → std::terminate
}

void risky() noexcept(false) {  // 等同于不写 noexcept，可能抛
    throw std::runtime_error("oops");
}

// 历史上的 throw() 动态异常规范（已废弃）:
void oldStyle() throw(std::bad_alloc);  // C++17 移除，不要用！
```

| 方面 | Java throws | C++ noexcept |
|------|------------|-------------|
| 语义 | "我可能抛出这些" | "我保证不抛出" |
| 编译器行为 | 强制调用者处理 | 编译器可做优化（不生成栈展开代码） |
| 违反后果 | 不处理 → 编译错误 | 运行时抛 → `std::terminate()` |
| 默认 | 没有声明 = 没有 checked exception | 没有标注 = 可能抛 |

## 五、异常类型体系

```java
// Java — 严格继承树
// Throwable
//   ├── Error（不应该 catch）
//   └── Exception
//         ├── RuntimeException（unchecked）
//         └── ...（checked）
```

```cpp
// C++ — 没有特权的根类型
#include <stdexcept>

// std::exception 是建议的基类，但不强制
throw std::runtime_error("msg");
throw std::logic_error("msg");
throw std::out_of_range("msg");

// 可以抛任何类型（int, string, 自定义类...）
// 但强烈建议继承 std::exception
throw 42;  // 合法，但极差实践

// catch 所有标准异常
catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
}

// catch 所有异常（包括非 std::exception）
catch (...) {
    // 无法获取异常信息
}
```

## 六、快速参考卡

| 场景 | Java 做法 | C++ 做法 |
|------|----------|---------|
| 抛异常 | `throw new Exception("msg")` | `throw std::runtime_error("msg")` |
| checked exception | `throws IOException` | 无（不存在这个概念） |
| 保证不抛 | 无直接等价 | `noexcept` |
| finally 清理 | `try {} finally {}` | RAII（析构函数自动清理） |
| try-with-resources | `try (Resource r = ...) {}` | RAII（任何有析构的类） |
| Optional | `Optional<T>` | `std::optional<T>`（C++17） |
| tagged union | 无内置 | `std::variant<A, B, C>`（C++17） |
| 错误码+值返回 | 自建 Result 类 | `std::expected<T, E>`（C++23）/ `std::variant<T, Error>` |
| catch-all | `catch (Exception e)` | `catch (const std::exception& e)` |
| catch literally everything | `catch (Throwable t)` | `catch (...)` |
| 异常基类 | `Throwable` / `Exception` | `std::exception`（建议使用，非强制） |
| 栈展开 | 自动 | 自动（RAII 析构保证调用） |
