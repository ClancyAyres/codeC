// 模块 1 示例: static 的三种含义
// 相关规范: Core Guidelines SF.6; Google Style: Static and Global Variables
//
// Java 的 static 只有一种含义（类级别），C++ 有三种完全不同的用法。

#include <iostream>
#include <string>

// ===== 1. 文件作用域 static: 符号仅本文件可见 =====
// 类似 Java 的 package-private，但更严格: 其他 .cpp 完全不可见
static int globalCounter = 0;  // 只有这个 .cpp 文件能访问

// 不加 static 的全局变量可以被其他 .cpp 用 extern 访问
int sharedCounter = 100;

// ===== 2. 函数内 static: 多次调用保持值 =====
// Java 完全没有这个用法
int getNextId() {
    static int id = 0;  // ✅ 只在第一次调用时初始化
    return ++id;        // 之后每次调用 id 保持上次的值
}

// ===== 3. 类内 static: 所有实例共享 =====
// 和 Java 一样
class Database {
public:
    static int connectionCount;  // ⚠️ 类内只声明
    static const int maxConnections = 100;  // ✅ const 可以在类内初始化

    Database() { ++connectionCount; }
    ~Database() { --connectionCount; }
};

// ⚠️ 非 const static 成员必须在类外定义
int Database::connectionCount = 0;

int main() {
    std::cout << "=== 函数内 static ===\n";
    std::cout << "getNextId(): " << getNextId() << '\n';  // 1
    std::cout << "getNextId(): " << getNextId() << '\n';  // 2
    std::cout << "getNextId(): " << getNextId() << '\n';  // 3

    std::cout << "\n=== 类内 static ===\n";
    std::cout << "Max connections: " << Database::maxConnections << '\n';
    {
        Database db1;
        std::cout << "After db1: " << Database::connectionCount << '\n';
        {
            Database db2;
            std::cout << "After db2: " << Database::connectionCount << '\n';
        }
        std::cout << "After db2 destroyed: " << Database::connectionCount << '\n';
    }
    std::cout << "After db1 destroyed: " << Database::connectionCount << '\n';

    std::cout << "\n=== 文件作用域 static ===\n";
    std::cout << "globalCounter: " << globalCounter << '\n';
    std::cout << "sharedCounter: " << sharedCounter << '\n';

    return 0;
}
