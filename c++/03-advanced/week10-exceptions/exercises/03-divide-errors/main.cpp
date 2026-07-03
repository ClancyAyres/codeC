// Exercise 03: Divide Errors — 三种错误处理方式对比
// TODO: 用异常、optional、variant 三种方式实现除法错误处理

#include <iostream>
#include <string>
#include <optional>
#include <variant>
#include <stdexcept>
#include <cmath>
#include <cassert>

// ── 辅助: overloaded ──────────────────────────────────────────
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

// ── TODO 方法1: 异常版 ───────────────────────────────────────

// TODO: 实现 divideThrow — 用异常处理错误
// - b == 0 → 抛 std::invalid_argument("division by zero")
// - 结果溢出或非有限 → 抛 std::overflow_error
// - 正常计算 a / b 并返回
double divideThrow(double a, double b) {
    // TODO
    return 0.0;
}

// ── TODO 方法2: optional 版 ───────────────────────────────────

// TODO: 实现 divideOptional — 用 std::optional 返回
// - b == 0 → 返回 std::nullopt
// - 结果非有限 → 返回 std::nullopt
// - 正常 → 返回 a / b
std::optional<double> divideOptional(double a, double b) {
    // TODO
    return std::nullopt;
}

// ── TODO 方法3: variant 版（完整错误信息） ────────────────────

// TODO: 实现 divideVariant — 成功返回 double，失败返回 string 错误信息
// - b == 0 → 返回错误字符串 "division by zero"
// - 结果非有限 → 返回错误字符串 "result not finite"
// - 正常 → 返回计算结果
using DivideResult = std::variant<double, std::string>;

DivideResult divideVariant(double a, double b) {
    // TODO
    return std::string("not implemented");
}

// ── TODO 方法4: variant + error_code 版 ───────────────────────

#include <system_error>

// TODO: 实现 divideWithCode — 用 variant<double, std::error_code>
// - b == 0 → 返回 std::make_error_code(std::errc::invalid_argument)
// - 结果非有限 → 返回 std::make_error_code(std::errc::result_out_of_range)
// - 正常 → 返回计算结果
using CodedResult = std::variant<double, std::error_code>;

CodedResult divideWithCode(double a, double b) {
    // TODO
    return std::make_error_code(std::errc::invalid_argument);
}

// ── 测试函数 ──────────────────────────────────────────────────

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
