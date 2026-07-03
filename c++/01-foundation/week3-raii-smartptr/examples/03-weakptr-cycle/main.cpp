#include <iostream>
#include <memory>
#include <string>

struct BadNode {
    std::string name;
    std::shared_ptr<BadNode> next;
    std::shared_ptr<BadNode> prev;     // BUG: 强引用导致循环引用

    explicit BadNode(const std::string& n) : name(n) {
        std::cout << "  BadNode(" << name << ") created\n";
    }
    ~BadNode() {
        std::cout << "  BadNode(" << name << ") destroyed\n";
    }
};

struct GoodNode {
    std::string name;
    std::shared_ptr<GoodNode> next;
    std::weak_ptr<GoodNode> prev;       // FIX: 弱引用打破循环

    explicit GoodNode(const std::string& n) : name(n) {
        std::cout << "  GoodNode(" << name << ") created\n";
    }
    ~GoodNode() {
        std::cout << "  GoodNode(" << name << ") destroyed\n";
    }

    std::shared_ptr<GoodNode> getPrev() const {
        return prev.lock();              // 需要时提升为 shared_ptr
    }
};

void badDoublyLinkedList() {
    std::cout << "\n=== Bad: shared_ptr for both next and prev ===\n";
    std::cout << "Creating A <-> B ...\n";
    {
        auto a = std::make_shared<BadNode>("A");
        auto b = std::make_shared<BadNode>("B");

        a->next = b;    // b 引用计数: 1 → 2
        b->prev = a;    // a 引用计数: 1 → 2

        std::cout << "  Leaving scope...\n";
        // a 局部变量销毁: a 引用计数 2 → 1 (b->prev 仍持有)
        // b 局部变量销毁: b 引用计数 2 → 1 (a->next 仍持有)
    }
    std::cout << "  After scope — DESTRUCTORS NEVER CALLED! (memory leak)\n";
}

void goodDoublyLinkedList() {
    std::cout << "\n=== Good: weak_ptr for prev ===\n";
    std::cout << "Creating A <-> B ...\n";
    {
        auto a = std::make_shared<GoodNode>("A");
        auto b = std::make_shared<GoodNode>("B");

        a->next = b;    // b 引用计数: 1 → 2
        b->prev = a;    // a 引用计数: 1 (weak_ptr 不增加)

        std::cout << "  a.use_count() = " << a.use_count() << '\n';
        std::cout << "  b.use_count() = " << b.use_count() << '\n';

        std::cout << "  Leaving scope...\n";
        // a 局部变量销毁: a 引用计数 1 → 0 → GoodNode("A") destroyed
        // b 局部变量销毁: b 引用计数 1 → 0 → GoodNode("B") destroyed
    }
    std::cout << "  After scope — both destroyed correctly!\n";
}

void weakPtrDemo() {
    std::cout << "\n=== weak_ptr usage demo ===\n";
    std::shared_ptr<int> sp = std::make_shared<int>(42);
    std::weak_ptr<int> wp = sp;

    std::cout << "  sp.use_count() = " << sp.use_count() << '\n';
    std::cout << "  wp.expired() = " << wp.expired() << '\n';

    if (auto locked = wp.lock()) {
        std::cout << "  wp.lock() succeeded, value = " << *locked << '\n';
    }

    std::cout << "  Resetting sp...\n";
    sp.reset();
    std::cout << "  wp.expired() = " << wp.expired() << '\n';

    if (auto locked = wp.lock()) {
        std::cout << "  Should not reach here\n";
    } else {
        std::cout << "  wp.lock() returned nullptr (object gone)\n";
    }
}

int main() {
    badDoublyLinkedList();
    goodDoublyLinkedList();
    weakPtrDemo();

    std::cout << "\nDone.\n";
    return 0;
}
