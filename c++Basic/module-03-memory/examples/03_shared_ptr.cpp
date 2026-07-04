// 模块 2 示例: shared_ptr 共享所有权
// 相关规范: Core Guidelines R.20, R.21, R.22

#include <iostream>
#include <memory>
#include <vector>
#include <string>

struct CacheEntry {
    std::string key;
    std::string value;
    CacheEntry(std::string k, std::string v)
        : key(std::move(k)), value(std::move(v)) {
        std::cout << "  [构造 CacheEntry] " << key << '\n';
    }
    ~CacheEntry() {
        std::cout << "  [析构 CacheEntry] " << key << '\n';
    }
};

void sharedExample() {
    // ✅ 正确: make_shared 一次分配控制块+对象，效率优于 shared_ptr(new T) — Core Guidelines R.22
    auto entry = std::make_shared<CacheEntry>("config", "value123");
    std::cout << "引用计数: " << entry.use_count() << '\n';

    {
        auto entry2 = entry;
        std::cout << "引用计数: " << entry.use_count() << '\n';

        std::vector<std::shared_ptr<CacheEntry>> cache;
        cache.push_back(entry);
        std::cout << "引用计数: " << entry.use_count() << '\n';
    }
    std::cout << "引用计数: " << entry.use_count() << '\n';
}

struct Node {
    std::string name;
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> prev;

    explicit Node(std::string n) : name(std::move(n)) {
        std::cout << "  [构造 Node] " << name << '\n';
    }
    ~Node() {
        std::cout << "  [析构 Node] " << name << '\n';
    }
};

void cycleExample() {
    std::cout << "--- 循环引用演示 (此例中节点不会被析构!) ---\n";
    // ✅ 正确: make_shared 创建 shared_ptr — Core Guidelines R.22
    auto a = std::make_shared<Node>("A");
    auto b = std::make_shared<Node>("B");
    a->next = b;   // ✅ 正确: 单向链接使用 shared_ptr 无问题
    // ❌ 错误: 双向 shared_ptr 形成循环引用，a/b 永远不会析构 — Core Guidelines R.22
    // 正确做法: prev 应该用 weak_ptr 打破循环
    b->prev = a;
}

int main() {
    sharedExample();
    cycleExample();
    std::cout << "cycleExample 结束 — 注意没有看到 Node 的析构消息\n";
    return 0;
}
