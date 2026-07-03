// 模块 2 示例: RAII (Resource Acquisition Is Initialization)
// 相关规范: Core Guidelines R.1, P.8, C.21
//
// RAII 是 C++ 最重要的设计哲学: 资源在构造时获取，在析构时释放。

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <memory>

void fileExample() {
    std::cout << "--- RAII 文件操作 ---\n";
    {
        // ✅ 正确: ofstream 构造时打开文件，离开作用域自动关闭 — Core Guidelines R.1
        std::ofstream file("/tmp/test_cpp.txt");
        if (file.is_open()) {
            file << "Hello from C++ RAII!\n";
        }
    }

    {
        std::ifstream file("/tmp/test_cpp.txt");
        std::string line;
        while (std::getline(file, line)) {
            std::cout << "  Read: " << line << '\n';
        }
    }
}

class ThreadSafeCounter {
public:
    void increment() {
        // ✅ 正确: lock_guard 构造时加锁，离开作用域自动解锁 — Core Guidelines CP.24
        std::lock_guard<std::mutex> lock(mutex_);
        ++count_;
    }
    int get() const {
        // ✅ 正确: lock_guard 保证读操作线程安全，异常安全 — Core Guidelines CP.24
        std::lock_guard<std::mutex> lock(mutex_);
        return count_;
    }
private:
    mutable std::mutex mutex_;
    int count_ = 0;
};

// ✅ 正确: RAII 封装类 — 构造时连接数据库，析构时断开 — Core Guidelines R.1, C.21
class DatabaseConnection {
public:
    // ✅ 正确: 构造函数获取资源（数据库连接）— Core Guidelines R.1
    DatabaseConnection(const std::string& url) : url_(url) {
        std::cout << "  [连接数据库] " << url_ << '\n';
    }
    // ✅ 正确: 析构函数自动释放资源（断开连接）— Core Guidelines R.1
    ~DatabaseConnection() {
        std::cout << "  [断开数据库] " << url_ << '\n';
    }
    void query(const std::string& sql) {
        std::cout << "  [执行查询] " << sql << '\n';
    }
    // ✅ 设计选择: 禁用拷贝，每个连接由唯一对象持有 — Core Guidelines C.21
    // 若需共享连接，应使用 shared_ptr，而非拷贝 DatabaseConnection
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;
private:
    std::string url_;
};

void raiiCustomExample() {
    std::cout << "--- 自定义 RAII ---\n";
    // ✅ 正确: db 是栈上对象，离开作用域自动调用析构函数断开连接 — Core Guidelines R.1
    DatabaseConnection db("mysql://localhost:3306/mydb");
    db.query("SELECT * FROM users");
}

int main() {
    fileExample();

    ThreadSafeCounter counter;
    counter.increment();
    counter.increment();
    std::cout << "Counter: " << counter.get() << '\n';

    raiiCustomExample();

    return 0;
}
