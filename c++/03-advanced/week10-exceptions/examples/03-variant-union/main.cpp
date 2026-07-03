// Example 03: Variant Union — 类型安全联合体
// 目的：展示 variant 作为 tagged union 替代手动 union、Result 模式、AST 节点

#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <system_error>
#include <cmath>

// ── 辅助: overloaded 模式（C++17 需推导指引） ──────────────────
template<class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

// ── 场景1: variant 基本操作 ──────────────────────────────────

void demo_basic() {
    std::cout << "── 场景1: 基本操作 ──\n";

    // variant 同一时刻持有一种类型
    std::variant<int, double, std::string> v;

    v = 42;
    std::cout << "  index = " << v.index()
              << " type = " << typeid(int).name() << '\n';

    v = "hello world";
    std::cout << "  新值: " << std::get<std::string>(v) << '\n';

    // get_if — 不抛异常的访问方式
    v = 3.14;
    if (auto* p = std::get_if<double>(&v)) {
        std::cout << "  get_if<double>: " << *p << '\n';
    }
    if (auto* p = std::get_if<int>(&v)) {
        std::cout << "  get_if<int>: " << *p << '\n';
    } else {
        std::cout << "  get_if<int>: nullptr (当前不是 int)\n";
    }

    // holds_alternative
    std::cout << "  holds<int>: " << std::holds_alternative<int>(v) << '\n';
}

// ── 场景2: variant 作为 Result<T, Error> ─────────────────────

template<typename T>
using Result = std::variant<T, std::string>;  // 成功返回 T，失败返回 string 错误信息

Result<double> safeDivide(double a, double b) {
    if (b == 0.0)
        return "division by zero";
    if (!std::isfinite(a / b))
        return "result is not finite";
    return a / b;
}

Result<int> safeParse(const std::string& s) {
    try {
        size_t pos;
        int v = std::stoi(s, &pos);
        if (pos != s.size())
            return "trailing characters after: " + std::to_string(v);
        return v;
    } catch (const std::invalid_argument&) {
        return "not a number";
    } catch (const std::out_of_range&) {
        return "number out of range";
    }
}

void demo_result() {
    std::cout << "\n── 场景2: Result 模式 ──\n";

    auto handle = [](const auto& result) {
        std::visit(overloaded{
            [](double v)  { std::cout << "  结果: " << v << '\n'; },
            [](const std::string& err) { std::cout << "  错误: " << err << '\n'; },
        }, result);
    };

    handle(safeDivide(10, 2));
    handle(safeDivide(1, 0));
    handle(safeDivide(0, 0));
}

// ── 场景3: AST 节点 — 多种类型安全的子节点 ────────────────────

struct Number { double value; };
struct String { std::string value; };
struct BinOp {
    enum Op { Add, Sub, Mul, Div } op;
    int lhs, rhs;  // 子节点索引
};

using AstNode = std::variant<Number, String, BinOp>;

double evaluate(const std::vector<AstNode>& nodes, int idx) {
    return std::visit(overloaded{
        [](const Number& n) { return n.value; },
        [](const String&) -> double {
            throw std::runtime_error("cannot evaluate string");
        },
        [&](const BinOp& op) -> double {
            double left = evaluate(nodes, op.lhs);
            double right = evaluate(nodes, op.rhs);
            switch (op.op) {
                case BinOp::Add: return left + right;
                case BinOp::Sub: return left - right;
                case BinOp::Mul: return left * right;
                case BinOp::Div: return left / right;
            }
            throw std::runtime_error("unknown op");
        },
    }, nodes[idx]);
}

void demo_ast() {
    std::cout << "\n── 场景3: AST 节点 ──\n";

    std::vector<AstNode> nodes;
    // 构造: (10 + 20) * 3
    nodes.push_back(Number{10});     // idx=0
    nodes.push_back(Number{20});     // idx=1
    nodes.push_back(BinOp{BinOp::Add, 0, 1});  // idx=2: 10+20
    nodes.push_back(Number{3});      // idx=3
    nodes.push_back(BinOp{BinOp::Mul, 2, 3});  // idx=4: (10+20)*3

    std::cout << "  (10 + 20) * 3 = " << evaluate(nodes, 4) << '\n';
}

// ── 场景4: variant 的状态机状态 ──────────────────────────────

struct Idle {};
struct Running { int progress; };
struct Paused { int progress; std::string reason; };
struct Finished { int result; };

using State = std::variant<Idle, Running, Paused, Finished>;

void describe(const State& state) {
    std::visit(overloaded{
        [](Idle)        { std::cout << "  状态: 空闲\n"; },
        [](Running r)   { std::cout << "  状态: 运行中 (进度 " << r.progress << "%)\n"; },
        [](Paused p)    { std::cout << "  状态: 已暂停 (进度 " << p.progress
                                    << "%, 原因: " << p.reason << ")\n"; },
        [](Finished f)  { std::cout << "  状态: 已完成 (结果: " << f.result << ")\n"; },
    }, state);
}

void demo_state() {
    std::cout << "\n── 场景4: 状态机 ──\n";

    State s = Idle{};
    describe(s);

    s = Running{50};
    describe(s);

    s = Paused{50, "等待用户输入"};
    describe(s);

    s = Finished{200};
    describe(s);
}

// ── main ──────────────────────────────────────────────────────

int main() {
    std::cout << "=== std::variant 类型安全联合体 ===\n\n";
    demo_basic();
    demo_result();
    demo_ast();
    demo_state();
    return 0;
}
