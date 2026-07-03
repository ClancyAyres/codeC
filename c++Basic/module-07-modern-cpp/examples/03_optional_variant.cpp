// 模块 7 示例: optional 与 variant
// 相关规范: Core Guidelines I.12, I.13

#include <iostream>
#include <optional>
#include <variant>
#include <string>
#include <vector>
#include <map>

// ✅ optional: 替代 null — Java Optional<T> 等价
std::optional<int> findInMap(const std::map<std::string, int>& m,
                              const std::string& key) {
    auto it = m.find(key);
    if (it != m.end()) {
        return it->second;
    }
    return std::nullopt;  // 无值
}

// ✅ variant: 类型安全的 union — Java 没有等价物
using JsonValue = std::variant<int, double, std::string, std::nullptr_t>;

void printJson(const JsonValue& value) {
    // std::visit + lambda: 模式匹配
    std::visit([](const auto& v) {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, std::nullptr_t>) {
            std::cout << "null\n";
        } else {
            std::cout << v << '\n';
        }
    }, value);
}

int main() {
    // === optional ===
    std::map<std::string, int> scores = {{"Alice", 90}, {"Bob", 85}};

    auto result = findInMap(scores, "Alice");
    if (result.has_value()) {
        std::cout << "Alice: " << result.value() << '\n';
    }
    // ✅ value_or: 默认值 — Java orElse 等价
    std::cout << "Charlie: " << findInMap(scores, "Charlie").value_or(-1) << '\n';

    // === variant ===
    JsonValue v1 = 42;
    JsonValue v2 = 3.14;
    JsonValue v3 = std::string("hello");
    JsonValue v4 = nullptr;

    printJson(v1);  // 42
    printJson(v2);  // 3.14
    printJson(v3);  // hello
    printJson(v4);  // null

    // 检查 variant 当前持有的类型
    if (std::holds_alternative<int>(v1)) {
        std::cout << "v1 holds int: " << std::get<int>(v1) << '\n';
    }

    return 0;
}
