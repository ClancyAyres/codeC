// Exercise 03: Divide Errors — 完整解法

#include <iostream>
#include <string>
#include <optional>
#include <variant>
#include <stdexcept>
#include <cmath>
#include <system_error>
#include <cassert>

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

double divideThrow(double a, double b) {
    if (b == 0.0)
        throw std::invalid_argument("division by zero");
    double result = a / b;
    if (!std::isfinite(result))
        throw std::overflow_error("result not finite");
    return result;
}

std::optional<double> divideOptional(double a, double b) {
    if (b == 0.0) return std::nullopt;
    double result = a / b;
    if (!std::isfinite(result)) return std::nullopt;
    return result;
}

using DivideResult = std::variant<double, std::string>;

DivideResult divideVariant(double a, double b) {
    if (b == 0.0) return std::string("division by zero");
    double result = a / b;
    if (!std::isfinite(result)) return std::string("result not finite");
    return result;
}

using CodedResult = std::variant<double, std::error_code>;

CodedResult divideWithCode(double a, double b) {
    if (b == 0.0)
        return std::make_error_code(std::errc::invalid_argument);
    double result = a / b;
    if (!std::isfinite(result))
        return std::make_error_code(std::errc::result_out_of_range);
    return result;
}

void testDivideThrow() {
    assert(divideThrow(10, 2) == 5.0);
    try {
        divideThrow(1, 0);
        assert(false && "应该抛异常");
    } catch (const std::invalid_argument&) {}

    try {
        divideThrow(1e308, 1e-308);
        assert(false && "应该抛异常");
    } catch (const std::overflow_error&) {}
    std::cout << "[divideThrow] 通过\n";
}

void testDivideOptional() {
    auto r1 = divideOptional(10, 2);
    assert(r1 && *r1 == 5.0);

    auto r2 = divideOptional(1, 0);
    assert(!r2.has_value());

    auto r3 = divideOptional(1e308, 1e-308);
    assert(!r3.has_value());
    std::cout << "[divideOptional] 通过\n";
}

void testDivideVariant() {
    auto r1 = divideVariant(10, 2);
    assert(std::holds_alternative<double>(r1) && std::get<double>(r1) == 5.0);

    auto r2 = divideVariant(1, 0);
    assert(std::holds_alternative<std::string>(r2));

    auto r3 = divideVariant(1e308, 1e-308);
    assert(std::holds_alternative<std::string>(r3));
    std::cout << "[divideVariant] 通过\n";
}

void testDivideWithCode() {
    auto r1 = divideWithCode(10, 2);
    assert(std::holds_alternative<double>(r1) && std::get<double>(r1) == 5.0);

    auto r2 = divideWithCode(1, 0);
    assert(std::holds_alternative<std::error_code>(r2));
    assert(std::get<std::error_code>(r2) == std::make_error_code(std::errc::invalid_argument));

    auto r3 = divideWithCode(1e308, 1e-308);
    assert(std::holds_alternative<std::error_code>(r3));
    std::cout << "[divideWithCode] 通过\n";
}

int main() {
    testDivideThrow();
    testDivideOptional();
    testDivideVariant();
    testDivideWithCode();
    std::cout << "\n所有测试通过。\n";
    return 0;
}
