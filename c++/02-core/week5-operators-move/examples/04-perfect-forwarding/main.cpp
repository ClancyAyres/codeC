// Example 04: Perfect Forwarding — 工厂函数与完美转发
// 演示：std::forward 保持参数值类别

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// ============================================================
// 简单工厂：完美转发单个参数
// ============================================================

struct Widget {
    std::string name_;

    Widget(const std::string& name) : name_(name) {
        std::cout << "  Widget(const std::string&) — copy\n";
    }

    Widget(std::string&& name) : name_(std::move(name)) {
        std::cout << "  Widget(std::string&&) — move\n";
    }
};

template<typename T, typename Arg>
std::unique_ptr<T> factory(Arg&& arg) {
    return std::make_unique<T>(std::forward<Arg>(arg));
}

// ============================================================
// 变参模板工厂：完美转发多个参数
// ============================================================

struct Gadget {
    int id_;
    std::string label_;
    std::vector<int> data_;

    Gadget(int id, const std::string& label, std::vector<int> data)
        : id_(id), label_(label), data_(std::move(data)) {
    }

    void print() const {
        std::cout << "  Gadget{id=" << id_ << ", label=\"" << label_
                  << "\", data.size=" << data_.size() << "}\n";
    }
};

template<typename T, typename... Args>
std::unique_ptr<T> make(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// ============================================================
// 转发包装函数
// ============================================================

void process(const std::string& s) {
    std::cout << "  process(const std::string&) — lvalue: " << s << '\n';
}

void process(std::string&& s) {
    std::cout << "  process(std::string&&) — rvalue: " << s << '\n';
}

template<typename T>
void relay(T&& arg) {
    std::cout << "  relay forwarding...\n";
    process(std::forward<T>(arg));
}

// ============================================================
// 对比：错误写法（不用 forward）
// ============================================================

template<typename T>
void relayWrong(T&& arg) {
    std::cout << "  relayWrong: using std::move...\n";
    process(std::move(arg));  // 错误：总是调用右值版本
}

// ============================================================
// 转发到 setter
// ============================================================

class Holder {
    std::string value_;
public:
    template<typename T>
    void setValue(T&& val) {
        value_ = std::forward<T>(val);
    }

    const std::string& value() const { return value_; }
};

int main() {
    std::cout << "=== Perfect Forwarding Factory ===\n";

    std::string name = "Alice";

    std::cout << "factory with lvalue:\n";
    auto w1 = factory<Widget>(name);

    std::cout << "\nfactory with rvalue:\n";
    auto w2 = factory<Widget>(std::string("Bob"));

    std::cout << "\nfactory with moved lvalue:\n";
    auto w3 = factory<Widget>(std::move(name));
    std::cout << "  name after move: \"" << name << "\"\n";

    std::cout << "\n=== Variadic Factory ===\n";
    auto g = make<Gadget>(42, "Sensor", std::vector<int>{1, 2, 3, 4, 5});
    g->print();

    std::cout << "\n=== Relay Function ===\n";
    std::string msg = "hello";

    std::cout << "relay(lvalue):\n";
    relay(msg);

    std::cout << "\nrelay(rvalue):\n";
    relay(std::string("world"));

    std::cout << "\n=== Wrong Relay (always moves) ===\n";
    std::string msg2 = "important data";
    relayWrong(std::move(msg2));
    std::cout << "  msg2 after wrong relay: \"" << msg2 << "\"\n";

    std::cout << "\n=== Forwarding Setter ===\n";
    Holder h;
    std::string data = "original";

    h.setValue(data);
    std::cout << "  After setValue(lvalue): holder=\"" << h.value()
              << "\", data=\"" << data << "\"\n";

    h.setValue(std::move(data));
    std::cout << "  After setValue(rvalue): holder=\"" << h.value()
              << "\", data=\"" << data << "\"\n";

    return 0;
}
