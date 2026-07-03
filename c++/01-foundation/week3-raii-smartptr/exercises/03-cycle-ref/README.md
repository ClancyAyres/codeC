# 习题 03：找出并修复 shared_ptr 循环引用

## 目标

阅读下面的代码，找出循环引用问题并修复。

## 有问题的代码

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Person {
    std::string name;
    std::shared_ptr<Person> spouse;    // 配偶

    Person(const std::string& n) : name(n) {
        std::cout << "Person(" << name << ") created\n";
    }
    ~Person() {
        std::cout << "Person(" << name << ") destroyed\n";
    }
};

int main() {
    auto alice = std::make_shared<Person>("Alice");
    auto bob = std::make_shared<Person>("Bob");

    alice->spouse = bob;
    bob->spouse = alice;   // BUG: 循环引用！

    // alice 和 bob 离开作用域：
    //   alice 引用计数 = 2 (alice 自身 + bob->spouse)
    //   bob   引用计数 = 2 (bob 自身   + alice->spouse)
    // 局部变量销毁后各剩 1 → 析构函数永不调用 → 内存泄漏！

    return 0;
}
```

## 要求

1. 找出循环引用问题
2. 修改 `spouse` 成员的类型来修复（保持逻辑正确）
3. 添加一个 `getSpouse()` 方法，返回 `std::shared_ptr<Person>`
4. 验证：程序结束时两个 Person 对象的析构函数都被调用

## 提示

- 将 `spouse` 成员的类型改为 `std::weak_ptr<Person>`
- 提供 `getSpouse()` 方法，内部调用 `weak_ptr::lock()`
- 测试时使用 `getSpouse()` 而非直接访问 `spouse`

## 预期输出

```
Person(Alice) created
Person(Bob) created
Alice's spouse: Bob
Bob's spouse: Alice
Alice's spouse use_count: 1
After resetting alice:
Bob's spouse: (none)
Person(Alice) destroyed
Person(Bob) destroyed
```
