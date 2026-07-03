// 模块 9 示例: mutex + lock_guard
// 类似 Java synchronized
// 相关规范: Core Guidelines CP.20, CP.22

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>

class BankAccount {
public:
    explicit BankAccount(std::string owner, int initialBalance)
        : owner_(std::move(owner)), balance_(initialBalance) {}

    // ✅ 线程安全的转账
    // Core Guideline CP.22: 永远不要在持有锁时调用未知代码
    void transferTo(BankAccount& other, int amount) {
        if (this == &other) return;  // 自转账

        // ✅ 同时锁定两个 mutex，避免死锁
        std::lock(mutex_, other.mutex_);
        std::lock_guard<std::mutex> lg1(mutex_, std::adopt_lock);
        std::lock_guard<std::mutex> lg2(other.mutex_, std::adopt_lock);

        if (balance_ >= amount) {
            balance_ -= amount;
            other.balance_ += amount;
        }
    }

    int balance() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return balance_;
    }

private:
    std::string owner_;
    int balance_;
    mutable std::mutex mutex_;
};

int main() {
    BankAccount alice("Alice", 1000);
    BankAccount bob("Bob", 500);

    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&alice, &bob]() {
            alice.transferTo(bob, 10);
        });
        threads.emplace_back([&bob, &alice]() {
            bob.transferTo(alice, 5);
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Alice: " << alice.balance() << '\n';
    std::cout << "Bob: " << bob.balance() << '\n';

    // 验证: total 不变
    std::cout << "Total: " << alice.balance() + bob.balance() << '\n';

    return 0;
}
