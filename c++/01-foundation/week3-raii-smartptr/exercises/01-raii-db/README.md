# 习题 01：RAII 数据库连接

## 目标

实现一个 `DatabaseConnection` 类，使用 RAII 模式自动管理"数据库连接"的打开和关闭。

## 要求

实现以下类：

```cpp
class DatabaseConnection {
    std::string dbName_;
    bool connected_;
public:
    // 构造函数：打开连接
    explicit DatabaseConnection(const std::string& dbName);

    // 析构函数：关闭连接
    ~DatabaseConnection();

    // 禁止拷贝（数据库连接通常不可拷贝）
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;

    // 允许移动
    DatabaseConnection(DatabaseConnection&& other) noexcept;
    DatabaseConnection& operator=(DatabaseConnection&& other) noexcept;

    // 查询接口
    bool isConnected() const;
    const std::string& dbName() const;
    void execute(const std::string& sql);
};
```

## 行为要求

- 构造函数输出 `[RAII] Database '<name>' opened`
- 析构函数输出 `[RAII] Database '<name>' closed`
- 移动构造/赋值后，源对象的 `connected_` 变为 `false`
- `execute()` 输出 `[SQL] <sql>`，仅在 `connected_` 为 `true` 时有效
- 析构函数检查：如果 `connected_` 为 `true` 才输出关闭信息

## 提示

- 用 `std::exchange` 实现移动语义
- 移动构造函数中源对象的 `connected_` 应设为 `false`
- 移动赋值中先检查自赋值，再关闭当前连接（如果已打开），最后接管源对象的资源

## 预期输出

```
[RAII] Database 'mydb' opened
[SQL] SELECT * FROM users
[RAII] Database 'mydb' closed
```
