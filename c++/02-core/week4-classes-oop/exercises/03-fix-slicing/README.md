# 习题 03：找出并修复对象切片 Bug

## 目标

下面的代码存在对象切片问题。找出 Bug 并修复。

## 原始代码问题

```cpp
class Animal {
protected:
    std::string name_;
public:
    Animal(const std::string& name) : name_(name) {}
    virtual ~Animal() = default;
    virtual std::string sound() const = 0;
    std::string describe() const {
        return name_ + " says " + sound();
    }
};

class Dog : public Animal {
public:
    Dog(const std::string& name) : Animal(name) {}
    std::string sound() const override { return "Woof"; }
};

class Cat : public Animal {
public:
    Cat(const std::string& name) : Animal(name) {}
    std::string sound() const override { return "Meow"; }
};
```

## Bug 位置

`main.cpp` 中的 `addAnimal` 函数按值接收 `Animal`，导致派生类对象切片。`zoo` 容器存储 `Animal` 值，同样切片。

## 任务

1. 阅读 `main.cpp` 的代码
2. 找出所有发生对象切片的位置
3. 修复代码：将容器改为 `vector<unique_ptr<Animal>>`，将 `addAnimal` 改为接收 `unique_ptr<Animal>`
4. 确保输出正确显示多态行为

## 预期输出

```
=== Zoo ===
Buddy says Woof (expect Woof)
Whiskers says Meow (expect Meow)
```
