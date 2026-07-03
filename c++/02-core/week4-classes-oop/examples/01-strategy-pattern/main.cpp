// Example 01: Strategy Pattern — 虚函数实现策略模式
// Java 接口对应：interface TaxStrategy { double calculate(double income); }

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ============================================================
// Java 等价写法 (注释):
// interface TaxStrategy {
//     double calculate(double income);
// }
// ============================================================

class TaxStrategy {
public:
    virtual ~TaxStrategy() = default;
    virtual double calculate(double income) const = 0;
    virtual std::string name() const = 0;
};

// Java: class FlatTax implements TaxStrategy
class FlatTax : public TaxStrategy {
    double rate_;
public:
    explicit FlatTax(double rate) : rate_(rate) {}

    double calculate(double income) const override {
        return income * rate_;
    }

    std::string name() const override {
        return "FlatTax(" + std::to_string(static_cast<int>(rate_ * 100)) + "%)";
    }
};

// Java: class ProgressiveTax implements TaxStrategy
class ProgressiveTax : public TaxStrategy {
public:
    struct Bracket {
        double threshold;
        double rate;
    };
private:
    std::vector<Bracket> brackets_;
public:
    ProgressiveTax(std::initializer_list<Bracket> brackets)
        : brackets_(brackets) {
    }

    double calculate(double income) const override {
        double tax = 0.0;
        double remaining = income;
        double previousThreshold = 0.0;

        for (const auto& bracket : brackets_) {
            double taxableInBracket = std::min(
                remaining,
                bracket.threshold - previousThreshold
            );
            if (taxableInBracket <= 0) break;

            tax += taxableInBracket * bracket.rate;
            remaining -= taxableInBracket;
            previousThreshold = bracket.threshold;
        }
        return tax;
    }

    std::string name() const override {
        return "ProgressiveTax";
    }
};

// Java: class TaxFree implements TaxStrategy
class TaxFree : public TaxStrategy {
public:
    double calculate(double) const override { return 0.0; }
    std::string name() const override { return "TaxFree"; }
};

// ============================================================
// Java 等价写法 (注释):
// class TaxCalculator {
//     private TaxStrategy strategy;
//     TaxCalculator(TaxStrategy strategy) { this.strategy = strategy; }
//     void setStrategy(TaxStrategy s) { this.strategy = s; }
//     double compute(double income) { return strategy.calculate(income); }
// }
// ============================================================

class TaxCalculator {
    std::unique_ptr<TaxStrategy> strategy_;
public:
    explicit TaxCalculator(std::unique_ptr<TaxStrategy> strategy)
        : strategy_(std::move(strategy)) {
    }

    void setStrategy(std::unique_ptr<TaxStrategy> strategy) {
        strategy_ = std::move(strategy);
    }

    double compute(double income) const {
        return strategy_->calculate(income);
    }

    std::string strategyName() const {
        return strategy_->name();
    }
};

int main() {
    const double incomes[] = {30000, 80000, 150000, 500000};

    TaxCalculator calculator(std::make_unique<FlatTax>(0.15));
    std::cout << "=== Strategy: " << calculator.strategyName() << " ===\n";
    for (double income : incomes) {
        std::cout << "  Income $" << std::fixed << std::setprecision(0)
                  << income << " → Tax $" << std::fixed
                  << std::setprecision(2) << calculator.compute(income) << '\n';
    }

    std::cout << "\n=== Strategy: ProgressiveTax ===\n";
    calculator.setStrategy(std::make_unique<ProgressiveTax>(
        std::initializer_list<ProgressiveTax::Bracket>{
            {50000, 0.10},
            {100000, 0.20},
            {200000, 0.30},
            {1e9, 0.40}
        }
    ));

    for (double income : incomes) {
        std::cout << "  Income $" << std::fixed << std::setprecision(0)
                  << income << " → Tax $" << std::fixed
                  << std::setprecision(2) << calculator.compute(income) << '\n';
    }

    std::cout << "\n=== Strategy: TaxFree ===\n";
    calculator.setStrategy(std::make_unique<TaxFree>());
    std::cout << "  Income $100000 → Tax $"
              << calculator.compute(100000) << '\n';

    std::cout << "\n=== Dynamic strategy switching ===\n";
    calculator.setStrategy(std::make_unique<FlatTax>(0.25));
    std::cout << "  Flat 25% on $80000: $" << calculator.compute(80000) << '\n';

    calculator.setStrategy(std::make_unique<FlatTax>(0.10));
    std::cout << "  Flat 10% on $80000: $" << calculator.compute(80000) << '\n';

    return 0;
}
