# 习题 04：组合优于继承 — 重构多重继承为组合

## 目标

将一段使用多重继承的代码重构为使用组合模式，体现"组合优于继承"的设计原则。

## 原始设计（多重继承）

```cpp
class Engine { /* start, stop */ };
class Wheels { /* rotate */ };
class Lights { /* on, off */ };

class Car : public Engine, public Wheels, public Lights {
    void drive() {
        start();    // from Engine
        rotate();   // from Wheels
        on();       // from Lights
    }
};
```

问题：
- 多重继承增加了耦合
- Engine、Wheels、Lights 作为基类语义不当（Car 不是一种 Engine）
- 难以替换实现（如换用 ElectricEngine）

## 任务

1. 阅读 `main.cpp` 中的原始多重继承代码
2. 重构为组合模式：
   - Engine、Wheels、Lights 作为成员变量（使用 unique_ptr）
   - Car 通过成员变量调用功能
   - 添加一个接口类（纯虚类）使 Engine 可替换
3. 演示使用 ElectricEngine 替换 Engine

## 提示

- 将 Engine 改为抽象接口 `IEngine`，ConcreteEngine 和 ElectricEngine 继承它
- Car 持有 `unique_ptr<IEngine>`、`unique_ptr<Wheels>`、`unique_ptr<Lights>`
- 通过构造函数注入依赖

## 预期输出

```
=== Car with concrete engine ===
ConcreteEngine starting
Wheels rotating
Lights on
=== Car with electric engine ===
ElectricEngine starting silently
Wheels rotating
Lights on
```
