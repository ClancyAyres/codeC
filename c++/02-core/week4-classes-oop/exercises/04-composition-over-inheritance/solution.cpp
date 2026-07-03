#include <iostream>
#include <memory>
#include <utility>

class IEngine {
public:
    virtual ~IEngine() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
};

class ConcreteEngine : public IEngine {
public:
    void start() override { std::cout << "ConcreteEngine starting\n"; }
    void stop() override { std::cout << "ConcreteEngine stopping\n"; }
};

class ElectricEngine : public IEngine {
public:
    void start() override { std::cout << "ElectricEngine starting silently\n"; }
    void stop() override { std::cout << "ElectricEngine stopping\n"; }
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

class Car {
    std::unique_ptr<IEngine> engine_;
    std::unique_ptr<Wheels> wheels_;
    std::unique_ptr<Lights> lights_;

public:
    Car(std::unique_ptr<IEngine> engine,
        std::unique_ptr<Wheels> wheels,
        std::unique_ptr<Lights> lights)
        : engine_(std::move(engine))
        , wheels_(std::move(wheels))
        , lights_(std::move(lights)) {
    }

    void drive() {
        engine_->start();
        wheels_->rotate();
        lights_->on();
    }

    void park() {
        lights_->off();
        wheels_->brake();
        engine_->stop();
    }
};

int main() {
    std::cout << "=== Car with concrete engine ===\n";
    Car car(std::make_unique<ConcreteEngine>(),
            std::make_unique<Wheels>(),
            std::make_unique<Lights>());
    car.drive();

    std::cout << "\n=== Car with electric engine ===\n";
    Car electricCar(std::make_unique<ElectricEngine>(),
                    std::make_unique<Wheels>(),
                    std::make_unique<Lights>());
    electricCar.drive();

    return 0;
}
