// Exercise 04: Exception Safety Audit — 完整解法

#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <cassert>

// ── 问题1 修复: 用 unique_ptr 替代裸指针 ──────────────────────

class FileProcessorFixed {
    std::unique_ptr<std::ostream> log;
    std::unique_ptr<std::string> buffer;

public:
    FileProcessorFixed(const std::string& logPath, const std::string& data)
        : log(std::make_unique<std::ofstream>(logPath))
        , buffer(std::make_unique<std::string>(data))
    {
        // 如果 buffer 的 make_unique 抛异常：
        // log 的 unique_ptr 已完全构造 → 析构时会关闭/释放
        // buffer 的 unique_ptr 未构造 → 无需清理
    }
    // 无需手写析构

    void process() {
        *buffer += "_processed";
        *log << "Processed: " << *buffer << '\n';
    }

    const std::string& getBuffer() const { return *buffer; }
};

// ── 问题2 修复: noexcept move ────────────────────────────────

struct Resource {
    int* data;
    size_t size;

    Resource(size_t n) : data(new int[n]), size(n) {}

    Resource(Resource&& other) noexcept    // 标记 noexcept
        : data(other.data)
        , size(other.size)
    {
        other.data = nullptr;
        other.size = 0;
    }

    ~Resource() { delete[] data; }

    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;
    Resource& operator=(Resource&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }
};

static_assert(std::is_nothrow_move_constructible_v<Resource>,
              "Resource move 构造应为 noexcept");

// ── 问题3 修复: RAII 锁包装 ──────────────────────────────────

class LockGuard {
    bool* locked;
public:
    explicit LockGuard(bool& lock) : locked(&lock) {
        *locked = true;  // lock
    }
    ~LockGuard() {
        *locked = false; // unlock（析构保证调用）
    }
    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
};

class Counter {
    int value = 0;
    bool locked = false;

public:
    void increment(int amount) {
        LockGuard guard(locked);  // RAII: 构造 lock，析构 unlock
        value += amount;
        // 即使 value += amount 抛异常，guard 析构也保证解锁
    }

    int get() const { return value; }
};

// ── 问题4 修复: 析构不抛异常 ──────────────────────────────────

class DatabaseConnectionFixed {
    bool connected = true;
public:
    void close() {
        if (!connected) throw std::logic_error("already closed");
        connected = false;
    }

    ~DatabaseConnectionFixed() noexcept {  // 显式 noexcept（其实默认就是）
        try {
            close();
        } catch (...) {
            // 析构函数中吞掉所有异常
            // 在实际代码中应该记录日志
        }
    }
};

// ── 问题5 修复: 返回 unique_ptr ───────────────────────────────

struct Widget {
    int id;
    explicit Widget(int id) : id(id) {}
    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;
};

std::unique_ptr<Widget> createWidgetGood(int id) {
    return std::make_unique<Widget>(id);
    // 如果 make_unique 失败抛异常，unique_ptr 自动处理
}

// ── main ──────────────────────────────────────────────────────

int main() {
    {
        FileProcessorFixed fp("test_audit.log", "hello");
        fp.process();
        assert(fp.getBuffer() == "hello_processed");
        std::cout << "[构造 SAFETY] 通过\n";
    }

    {
        std::vector<Resource> v;
        v.push_back(Resource(42));  // noexcept move 让 vector 放心使用 move
        std::cout << "[noexcept MOVE] 通过 (push_back 成功)\n";
    }

    {
        Counter c;
        c.increment(5);
        assert(c.get() == 5);
        std::cout << "[RAII LOCK] 通过\n";
    }

    {
        DatabaseConnectionFixed conn;
        std::cout << "[析构 NEXCEPT] 通过 (析构未 terminate)\n";
    }

    {
        auto w = createWidgetGood(42);
        assert(w->id == 42);
        std::cout << "[unique_ptr] Widget(" << w->id << ")\n";
    }

    std::cout << "\n所有审计项通过。\n";
    return 0;
}
