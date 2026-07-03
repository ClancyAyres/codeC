// Example 03: Initializer List — const/引用成员必须使用初始化列表
// Java 中 final 字段可以在声明时或构造函数体内赋值，C++ const 成员必须用初始化列表

#include <iostream>
#include <string>

// ============================================================
// 类 A: 有 const 成员和引用成员 — 必须使用初始化列表
// ============================================================
class Config {
    const std::string appName_;   // const 成员
    const int maxRetries_;        // const 成员
    std::string& logRef_;         // 引用成员
    int counter_;                 // 普通成员（可选初始化列表）

public:
    Config(const std::string& name, int retries, std::string& logTarget)
        : appName_(name)          // 必须：const 成员
        , maxRetries_(retries)    // 必须：const 成员
        , logRef_(logTarget)      // 必须：引用成员
        , counter_(0)             // 推荐：比函数体内赋值高效
    {
        // 以下代码如果取消注释会编译失败：
        // appName_ = name;        // 错误：const 成员不能赋值
        // maxRetries_ = retries;  // 错误：const 成员不能赋值
        // logRef_ = logTarget;    // 错误：引用不能重新绑定
    }

    void print() const {
        std::cout << "  appName: " << appName_ << '\n'
                  << "  maxRetries: " << maxRetries_ << '\n'
                  << "  logRef: " << logRef_ << '\n'
                  << "  counter: " << counter_ << '\n';
    }

    void increment() { ++counter_; }
};

// ============================================================
// 演示：如果忘记初始化列表，const/引用成员会导致编译错误
// 取消下面的注释会导致编译失败
// ============================================================

/*
class BrokenConfig {
    const int id_;        // 未初始化 — 编译错误
    std::string& ref_;    // 未初始化 — 编译错误
public:
    BrokenConfig() {       // 没有初始化列表 — 编译错误
        // id_ = 0;       // 已经太晚了
    }
};
*/

// ============================================================
// 类 B: 基类没有默认构造函数 — 必须使用初始化列表
// ============================================================
class Database {
    std::string url_;
public:
    explicit Database(const std::string& url) : url_(url) {
        std::cout << "  Database connected to: " << url_ << '\n';
    }
    // 没有默认构造函数！
};

class UserService : public Database {
    std::string table_;
public:
    UserService(const std::string& url, const std::string& table)
        : Database(url)         // 必须：调用基类构造函数
        , table_(table) {
        std::cout << "  UserService using table: " << table_ << '\n';
    }
};

// ============================================================
// 演示初始化顺序陷阱
// ============================================================
class OrderTrap {
    int a_;
    int b_;
public:
    // 初始化列表是 b 在前，但 a 声明在前，所以 a 先初始化
    OrderTrap(int x) : b_(x), a_(b_ + 1) {
        std::cout << "  OrderTrap: a=" << a_ << " (should be x+1), b="
                  << b_ << " (should be x)\n";
        std::cout << "  WARNING: a_ uses uninitialized b_! Undefined behavior.\n";
    }
};

int main() {
    std::cout << "=== Config with const and reference members ===\n";
    std::string logBuffer = "[initial log]";
    Config cfg("MyApp", 3, logBuffer);
    cfg.print();

    std::cout << "\n=== After modifying logBuffer ===\n";
    logBuffer = "[updated log message]";
    cfg.print();  // logRef_ 反映外部变化

    std::cout << "\n=== UserService (base class needs init list) ===\n";
    UserService service("postgresql://localhost:5432/mydb", "users");

    std::cout << "\n=== Initialization order trap ===\n";
    OrderTrap t(5);

    return 0;
}
