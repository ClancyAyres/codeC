// Example 01: Exception-Safe Stack — 基本/强/nothrow 三版本对比
// 目的：展示异常安全三等级在同一个数据结构中的实现差异

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cassert>

// ── 版本1：基本保证 — 不变量保持，但状态可能部分改变 ─────────
template<typename T>
class StackBasic {
    std::vector<T> data;
public:
    bool empty() const noexcept { return data.empty(); }
    size_t size() const noexcept { return data.size(); }

    void push(const T& value) {
        data.push_back(value);  // 可能抛 bad_alloc → data 不变 → 基本保证
    }

    void pop() {
        if (data.empty()) throw std::underflow_error("pop from empty stack");
        data.pop_back();        // noexcept（对标准容器）
    }

    const T& top() const {
        if (data.empty()) throw std::underflow_error("top on empty stack");
        return data.back();
    }
};

// ── 版本2：强保证 — copy-and-swap ─────────────────────────────
template<typename T>
class StackStrong {
    std::vector<T> data;
public:
    bool empty() const noexcept { return data.empty(); }
    size_t size() const noexcept { return data.size(); }
    const T& top() const { return data.back(); }

    void push(const T& value) {
        auto copy = data;            // 可能抛 — 但 data 未变
        copy.push_back(value);       // 可能抛 — 但 data 未变
        data.swap(copy);             // noexcept — 成功交换
    }
    // 强保证：push 要么完全完成，要么 data 毫发无损

    void pop() {
        if (data.empty()) throw std::underflow_error("pop from empty stack");
        data.pop_back();
    }
};

// ── 版本3：nothrow — 用 noexcept 标记所有不抛的操作 ────────────
template<typename T>
class StackNothrow {
    std::vector<T> data;
public:
    bool empty() const noexcept { return data.empty(); }
    size_t size() const noexcept { return data.size(); }
    void clear() noexcept { data.clear(); }

    void push(const T& value) {
        data.push_back(value);
    }

    void pop() {
        if (data.empty()) throw std::underflow_error("pop from empty stack");
        data.pop_back();
    }

    const T& top() const { return data.back(); }

    // swap 显式标记 noexcept
    void swap(StackNothrow& other) noexcept {
        data.swap(other.data);
    }
};

// ── 演示：包含可能抛异常的元素的栈 ─────────────────────────────

struct ThrowingCopy {
    int id;
    bool throwOnCopy;

    explicit ThrowingCopy(int id, bool throwOnCopy = false)
        : id(id), throwOnCopy(throwOnCopy) {}

    ThrowingCopy(const ThrowingCopy& other)
        : id(other.id), throwOnCopy(other.throwOnCopy)
    {
        if (throwOnCopy)
            throw std::runtime_error("Copy failed for " + std::to_string(id));
    }

    // move 也标记 noexcept（vector 扩容时的关键优化）
    ThrowingCopy(ThrowingCopy&&) noexcept = default;
};

int main() {
    // ── 基本保证验证 ──────────────────────────────────────────
    std::cout << "=== 异常安全三等级演示 ===\n\n";

    {
        StackBasic<ThrowingCopy> stack;
        stack.push(ThrowingCopy(1));
        stack.push(ThrowingCopy(2));

        std::cout << "[基本保证] 原始大小: " << stack.size() << '\n';
        try {
            stack.push(ThrowingCopy(3, true));  // 拷贝会抛异常
        } catch (const std::exception& e) {
            std::cout << "  异常: " << e.what() << '\n';
            std::cout << "  栈大小: " << stack.size()
                      << " (不变，基本保证)\n";
        }
    }

    // ── 强保证验证 ────────────────────────────────────────────
    std::cout << '\n';
    {
        StackStrong<ThrowingCopy> stack;
        stack.push(ThrowingCopy(1));
        stack.push(ThrowingCopy(2));

        std::cout << "[强保证] 原始大小: " << stack.size() << '\n';
        auto originalTop = stack.top().id;

        try {
            stack.push(ThrowingCopy(3, true));
        } catch (const std::exception& e) {
            std::cout << "  异常: " << e.what() << '\n';
            std::cout << "  栈大小: " << stack.size()
                      << " (不变，强保证)\n";
            assert(stack.top().id == originalTop);
            std::cout << "  栈顶元素: " << stack.top().id
                      << " (不变，强保证)\n";
        }
    }

    // ── noexcept swap 演示 ─────────────────────────────────────
    std::cout << '\n';
    {
        StackNothrow<ThrowingCopy> s1, s2;
        s1.push(ThrowingCopy(10));
        s1.push(ThrowingCopy(20));
        s2.push(ThrowingCopy(30));

        std::cout << "[nothrow swap] 交换前: s1=" << s1.size()
                  << " s2=" << s2.size() << '\n';

        s1.swap(s2);  // noexcept 保证

        std::cout << "  交换后: s1=" << s1.size()
                  << " s2=" << s2.size() << '\n';

        static_assert(noexcept(std::declval<StackNothrow<ThrowingCopy>&>()
                               .swap(std::declval<StackNothrow<ThrowingCopy>&>())),
                      "swap 应是 noexcept");
        std::cout << "  static_assert 通过: swap 确实是 noexcept\n";
    }

    std::cout << "\n所有测试通过。\n";
    return 0;
}
