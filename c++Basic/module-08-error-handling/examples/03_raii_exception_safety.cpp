// 模块 8 示例: RAII 异常安全
// Java 用 try-finally，C++ 用 RAII

#include <iostream>
#include <string>
#include <stdexcept>

// RAII 资源管理器: 即使异常也能正确清理
class ScopedOperation {
public:
    explicit ScopedOperation(const std::string& name)
        : name_(name) {
        std::cout << "[START] " << name_ << '\n';
    }

    ~ScopedOperation() {
        std::cout << "[END]   " << name_ << " (always called, even on exception)\n";
    }

    ScopedOperation(const ScopedOperation&) = delete;
    ScopedOperation& operator=(const ScopedOperation&) = delete;

private:
    std::string name_;
};

// ⚠️ 危险: 手动清理，异常不安全
void manualCleanupBad() {
    int* data = new int[100];
    // ... 做一些可能抛异常的操作 ...
    // riskyOperation();  // 如果这里抛异常...
    delete[] data;  // ...这行不会执行 → 内存泄漏!
}

// ✅ RAII: 自动清理，异常安全
void raiiCleanupGood() {
    ScopedOperation guard("Safe operation");

    // 即使 riskyOperation 抛异常，guard 的析构也会执行
    // riskyOperation();
}

int main() {
    std::cout << "=== RAII 异常安全演示 ===\n";

    try {
        raiiCleanupGood();
        std::cout << "Success path\n\n";

        throw std::runtime_error("Simulated error");
    }
    catch (const std::exception& e) {
        std::cout << "\nCaught: " << e.what() << '\n';
        std::cout << "But [END] was still printed above — RAII worked!\n";
    }

    return 0;
}
