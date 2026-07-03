// Exercise 04: Exception Safety Audit — 找出并修复异常不安全代码
// TODO: 审查以下代码，找出所有异常安全问题并修复

#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <cassert>

// ── 问题1: 构造函数 — 手动 new 导致泄露 ──────────────────────

class FileProcessor {
    std::ostream* log;
    std::string* buffer;

public:
    FileProcessor(const std::string& logPath, const std::string& data)
        : log(new std::ofstream(logPath))
        , buffer(new std::string(data))
    {}

    ~FileProcessor() {
        delete log;
        delete buffer;
    }

    void process() {
        *buffer += "_processed";
        *log << "Processed: " << *buffer << '\n';
    }

    const std::string& getBuffer() const { return *buffer; }
};

// TODO: 修复 FileProcessor
// 问题：如果 buffer(new std::string(data)) 抛异常，log 泄露
// 修复：用 std::unique_ptr 替代裸指针
// class FileProcessorFixed { ... };

// ── 问题2: 非 noexcept move 构造 ─────────────────────────────

struct Resource {
    int* data;
    size_t size;

    Resource(size_t n) : data(new int[n]), size(n) {}

    // TODO: 实现 noexcept move 构造函数
    // - 标记为 noexcept
    // - 从 other 窃取资源
    // - 将 other 置于有效但未指定状态（data=nullptr, size=0）
    Resource(Resource&& other)
        : data(other.data)
        , size(other.size)
    {
        other.data = nullptr;
        other.size = 0;
    }

    ~Resource() { delete[] data; }

    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;
};

// TODO: 验证 move 构造是否为 noexcept
// static_assert(..., "move 构造应为 noexcept");

// ── 问题3: 非 RAII 锁 ────────────────────────────────────────

class Counter {
    int value = 0;
    // 模拟锁（C++ 中不用 volatile 做同步，这里仅演示 RAII 模式）
    bool locked = false;

    void lock() { locked = true; }
    void unlock() { locked = false; }

public:
    // TODO: 实现 increment — 即使操作抛异常也要保证解锁
    // 提示：实现一个 LockGuard 类（RAII），或直接用类似模式
    void increment(int amount) {
        lock();
        value += amount;
        unlock();
    }

    int get() const { return value; }
};

// TODO: 实现 LockGuard RAII 包装类
// class LockGuard { ... };

// ── 问题4: 析构函数抛异常 ────────────────────────────────────

class DatabaseConnection {
    bool connected = true;
public:
    void close() {
        if (!connected) throw std::logic_error("already closed");
        connected = false;
    }

    // TODO: 修复析构函数 — 不能抛异常
    ~DatabaseConnection() {
        close();  // 如果抛异常 → std::terminate!
    }
};

// ── 问题5: 返回裸指针的工厂函数 ─────────────────────────────

// 问题：返回裸指针，调用者可能忘记 delete

struct Widget {
    int id;
    explicit Widget(int id) : id(id) {}
    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;
};

// TODO: 将 Bad 改为 Good
// 修复：返回 std::unique_ptr
Widget* createWidgetBad(int id) {
    return new Widget(id);
}
// TODO: std::unique_ptr<Widget> createWidgetGood(int id) { ... }

// ── main ──────────────────────────────────────────────────────

int main() {
    // 测试1: RAII 构造
    {
        // TODO: 使用修复后的 FileProcessorFixed
        // FileProcessorFixed fp("log.txt", "hello");
        std::cout << "[构造 SAFETY] 待修复\n";
    }

    // 测试2: noexcept move
    {
        // TODO: static_assert 验证
        // std::vector<Resource> v;
        // v.push_back(Resource(42));  // 需要 noexcept move
        std::cout << "[noexcept MOVE] 待修复\n";
    }

    // 测试3: RAII 锁
    {
        Counter c;
        c.increment(5);
        assert(c.get() == 5);
        std::cout << "[RAII LOCK] 通过\n";
    }

    // 测试4: 析构不抛
    {
        // TODO: 修复析构函数
        // DatabaseConnection conn;
        std::cout << "[析构 NEXCEPT] 待修复\n";
    }

    // 测试5: unique_ptr 返回
    {
        // TODO: auto w = createWidgetGood(42);
        // std::cout << "[unique_ptr] Widget(" << w->id << ")\n";
        std::cout << "[unique_ptr] 待修复\n";
    }

    std::cout << "\n审查完成。\n";
    return 0;
}
