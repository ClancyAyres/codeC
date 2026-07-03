# 习题 03：找内存泄漏

## 目标

阅读下方有 bug 的代码，找出并修复所有内存问题。

## 有问题的代码

```cpp
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

Resource* createResource(int id) {
    Resource* r = new Resource(id);
    if (id < 0) {
        return nullptr;   // BUG 1：提前返回，泄漏了 r
    }
    return r;
}

void processData(int size) {
    int* data = new int[size];
    Resource* r = new Resource(99);

    if (size > 1000) {
        std::cout << "Size too large, aborting\n";
        return;           // BUG 2：提前返回，泄漏了 data 和 r
    }

    delete[] data;
    delete[] r;             // BUG 3：r 是 new 分配的，应该用 delete 而非 delete[]
}

int main() {
    Resource* r = createResource(-5);
    // BUG 4：createResource 泄漏了 -5 对应的 Resource

    processData(2000);
    // BUG 5：processData 泄漏了 data 和 r

    return 0;
}
```

## 要求

1. 找出代码中所有 3 处内存问题
2. 逐一修复（不要重写整个逻辑，只需修复内存管理）
3. 确保所有路径都能正确释放资源

## 提示

- `new` 必须配 `delete`，`new[]` 必须配 `delete[]`
- 检查每个 `return` 路径是否都释放了已分配的资源
- 函数 `createResource` 的契约问题
