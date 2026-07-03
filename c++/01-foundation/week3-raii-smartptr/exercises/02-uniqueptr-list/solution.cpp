#include <iostream>
#include <memory>
#include <stdexcept>

struct Node {
    int value;
    std::unique_ptr<Node> next;

    explicit Node(int val) : value(val), next(nullptr) {}
};

class IntList {
    std::unique_ptr<Node> head_;
    Node* tail_ = nullptr;

public:
    IntList() = default;
    ~IntList() = default;

    void push_back(int value) {
        auto newNode = std::make_unique<Node>(value);
        Node* rawPtr = newNode.get();
        if (!head_) {
            head_ = std::move(newNode);
            tail_ = head_.get();
        } else {
            tail_->next = std::move(newNode);
            tail_ = rawPtr;
        }
    }

    void push_front(int value) {
        auto newNode = std::make_unique<Node>(value);
        if (!head_) {
            head_ = std::move(newNode);
            tail_ = head_.get();
        } else {
            newNode->next = std::move(head_);
            head_ = std::move(newNode);
        }
    }

    int front() const {
        if (!head_) {
            throw std::runtime_error("IntList::front: list is empty");
        }
        return head_->value;
    }

    bool empty() const {
        return head_ == nullptr;
    }

    void print() const {
        const Node* current = head_.get();
        while (current) {
            std::cout << current->value;
            current = current->next.get();
            if (current) std::cout << ' ';
        }
        std::cout << '\n';
    }
};

int main() {
    IntList list;

    std::cout << "=== Test 1: push_back ===\n";
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    std::cout << "List after push_back: ";
    list.print();
    std::cout << "  expected: 10 20 30\n\n";

    std::cout << "=== Test 2: push_front ===\n";
    list.push_front(5);
    std::cout << "List after push_front: ";
    list.print();
    std::cout << "  expected: 5 10 20 30\n\n";

    std::cout << "=== Test 3: Mixed operations ===\n";
    IntList list2;
    list2.push_front(5);
    list2.push_back(10);
    list2.push_front(1);
    list2.push_back(20);
    list2.push_back(30);
    list2.push_back(99);
    std::cout << "List after mixed ops: ";
    list2.print();
    std::cout << "  expected: 1 5 10 20 30 99\n\n";

    std::cout << "=== Test 4: front and empty ===\n";
    std::cout << "front: " << list2.front() << " (expect 1)\n";
    std::cout << "empty: " << list2.empty() << " (expect 0)\n\n";

    std::cout << "=== Test 5: Empty list ===\n";
    IntList emptyList;
    std::cout << "empty: " << emptyList.empty() << " (expect 1)\n";
    try {
        emptyList.front();
        std::cout << "ERROR: should have thrown!\n";
    } catch (const std::runtime_error& e) {
        std::cout << "Caught: " << e.what() << '\n';
    }
    std::cout << "  expected: IntList::front: list is empty\n\n";

    std::cout << "=== Test 6: Destructor (auto cleanup) ===\n";
    {
        IntList temp;
        for (int i = 0; i < 1000; ++i) {
            temp.push_back(i);
        }
        std::cout << "Created list with 1000 nodes\n";
    }
    std::cout << "List destroyed — no leaks (unique_ptr auto-cleanup)\n\n";

    std::cout << "All tests done.\n";
    return 0;
}
