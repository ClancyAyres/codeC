#include <iostream>

// ============================================================
// 原始设计：多重继承（需要重构）
// ============================================================

class Engine {
public:
    void start() { std::cout << "Engine starting\n"; }
    void stop() { std::cout << "Engine stopping\n"; }
};

class Wheels {
public:
    void rotate() { std::cout << "Wheels rotating\n"; }
    void brake() { std::cout << "Wheels braking\n"; }
};

class Lights {
public:
    void on() { std::cout << "Lights on\n"; }
    void off() { std::cout << "Lights off\n"; }
};

// TODO: 重构 — 将多重继承改为组合
// 步骤：
// 1. 创建 IEngine 接口（纯虚类），ConcreteEngine 和 ElectricEngine 继承它
// 2. Car 持有 unique_ptr<IEngine>, unique_ptr<Wheels>, unique_ptr<Lights>
// 3. Car 通过构造函数接收依赖
class Car : public Engine, public Wheels, public Lights {
public:
    void drive() {
        start();
        rotate();
        on();
    }
};

int main() {
    std::cout << "=== Car with concrete engine ===\n";
    Car car;
    car.drive();

    // TODO: 重构后应支持：
    // std::cout << "\n=== Car with electric engine ===\n";
    // Car electricCar(std::make_unique<ElectricEngine>(),
    //                 std::make_unique<Wheels>(),
    //                 std::make_unique<Lights>());
    // electricCar.drive();

    return 0;
}
