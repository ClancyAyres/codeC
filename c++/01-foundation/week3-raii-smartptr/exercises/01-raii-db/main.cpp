#include <iostream>
#include <string>
#include <utility>

class DatabaseConnection {
    std::string dbName_;
    bool connected_;

public:
    // TODO: 实现构造函数 — 输出打开信息，设置 connected_ = true
    explicit DatabaseConnection(const std::string& dbName)
        : dbName_(dbName), connected_(true) {
        // TODO: 输出 "[RAII] Database '<name>' opened"
    }

    // TODO: 实现析构函数 — 如果 connected_，输出关闭信息
    ~DatabaseConnection() {
        // TODO: if (connected_) 输出 "[RAII] Database '<name>' closed"
    }

    // 禁止拷贝
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;

    // TODO: 实现移动构造函数 — 接管资源，源对象 connected_ 变为 false
    DatabaseConnection(DatabaseConnection&& other) noexcept
        : dbName_(other.dbName_), connected_(other.connected_) {
        // TODO: 将源对象的 connected_ 设为 false
    }

    // TODO: 实现移动赋值运算符 — 检查自赋值，关闭当前连接，接管资源
    DatabaseConnection& operator=(DatabaseConnection&& other) noexcept {
        // TODO: 自赋值检查
        // TODO: 如果当前已连接，先"关闭"（输出信息）
        // TODO: 接管源对象的 dbName_ 和 connected_
        // TODO: 源对象 connected_ 设为 false
        return *this;
    }

    bool isConnected() const { return connected_; }
    const std::string& dbName() const { return dbName_; }

    void execute(const std::string& sql) {
        if (connected_) {
            std::cout << "[SQL] " << sql << '\n';
        }
    }
};

int main() {
    // 测试 1：基本 RAII
    std::cout << "=== Test 1: Basic RAII ===\n";
    {
        DatabaseConnection db("mydb");
        std::cout << "  connected: " << db.isConnected() << " (expect 1)\n";
        db.execute("SELECT * FROM users");
    }
    std::cout << "  db should be closed now\n\n";

    // 测试 2：移动构造
    std::cout << "=== Test 2: Move constructor ===\n";
    {
        DatabaseConnection db1("source_db");
        DatabaseConnection db2 = std::move(db1);
        std::cout << "  db1.connected: " << db1.isConnected() << " (expect 0)\n";
        std::cout << "  db2.connected: " << db2.isConnected() << " (expect 1)\n";
        std::cout << "  db2.name: " << db2.dbName() << " (expect source_db)\n";
    }
    std::cout << '\n';

    // 测试 3：移动赋值
    std::cout << "=== Test 3: Move assignment ===\n";
    {
        DatabaseConnection db1("first_db");
        DatabaseConnection db2("second_db");
        std::cout << "  Before move-assign:\n";
        db2 = std::move(db1);
        std::cout << "  db1.connected: " << db1.isConnected() << " (expect 0)\n";
        std::cout << "  db2.connected: " << db2.isConnected() << " (expect 1)\n";
        std::cout << "  db2.name: " << db2.dbName() << " (expect first_db)\n";
    }
    std::cout << '\n';

    // 测试 4：移动后执行查询
    std::cout << "=== Test 4: Execute after move ===\n";
    {
        DatabaseConnection db1("moved_db");
        DatabaseConnection db2 = std::move(db1);
        db2.execute("INSERT INTO logs VALUES (1, 'hello')");
        // db1.execute("should not print");
    }
    std::cout << '\n';

    std::cout << "All tests done.\n";
    return 0;
}
