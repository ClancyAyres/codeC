#include <iostream>

struct Resource {
    int id;
    Resource(int i) : id(i) {
        std::cout << "Resource(" << id << ") acquired\n";
    }
    ~Resource() {
        std::cout << "Resource(" << id << ") released\n";
    }
};

// BUG 1: 如果 id < 0，提前返回 nullptr，但没有 delete r
Resource* createResource(int id) {
    Resource* r = new Resource(id);
    if (id < 0) {
        return nullptr;   // 泄漏了 r！
    }
    return r;
}

// BUG 2: 提前返回时泄漏了 data 和 r
// BUG 3: delete r 错误地使用了 delete[]（应为 delete）
void processData(int size) {
    int* data = new int[size];
    Resource* r = new Resource(99);

    if (size > 1000) {
        std::cout << "Size too large, aborting\n";
        return;           // 泄漏了 data 和 r！
    }

    delete[] data;
    delete[] r;             // BUG: r 是用 new 分配的单个对象，应该用 delete 而不是 delete[]
}

int main() {
    Resource* r = createResource(-5);
    // BUG 4: createResource 泄漏了 -5 对应的 Resource
    // BUG 5: r 是 nullptr，但如果它非空呢？
    if (r) {
        delete r;
    }

    processData(2000);
    // processData 泄漏了 data 和 r

    return 0;
}
