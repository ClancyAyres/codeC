#include <iostream>
#include <string>

struct Dog {
    std::string name;

    Dog(const std::string& n) : name(n) {
        std::cout << "Dog(" << name << ") constructed\n";
    }

    ~Dog() {
        std::cout << "Dog(" << name << ") destroyed\n";
    }

    void bark() const {
        std::cout << name << " says: Woof!\n";
    }
};

void stackDemo() {
    std::cout << "\n=== stackDemo() begin ===\n";
    Dog d("StackDog");
    d.bark();
    std::cout << "=== stackDemo() end ===\n";
    // d 离开作用域，自动调用析构函数
}

void heapDemo() {
    std::cout << "\n=== heapDemo() begin ===\n";
    Dog* d = new Dog("HeapDog");
    d->bark();
    // 注意：这里没有 delete d
    std::cout << "=== heapDemo() end (no delete!) ===\n";
    // d 指针本身在栈上被销毁，但它指向的堆内存没有被释放 → 内存泄漏
}

void heapDemoFixed() {
    std::cout << "\n=== heapDemoFixed() begin ===\n";
    Dog* d = new Dog("HeapDogFixed");
    d->bark();
    delete d;
    d = nullptr;
    std::cout << "=== heapDemoFixed() end ===\n";
    // delete 触发析构，内存被正确释放
}

int main() {
    std::cout << "=== main() begin ===\n";

    stackDemo();
    // stackDemo 返回后，StackDog 已经被析构

    std::cout << "\n--- between calls ---\n";

    heapDemo();
    // heapDemo 返回后，HeapDog 没有被析构（内存泄漏）

    std::cout << "\n--- between calls ---\n";

    heapDemoFixed();
    // heapDemoFixed 返回后，HeapDogFixed 被正确析构

    std::cout << "\n=== main() end ===\n";
    return 0;
}
