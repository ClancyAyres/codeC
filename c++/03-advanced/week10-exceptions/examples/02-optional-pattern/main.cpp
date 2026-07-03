// Example 02: Optional Pattern — 用 std::optional 替代哨兵值和输出参数
// 目的：展示 optional 在查找、解析、延迟初始化等场景的使用

#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <map>

// ── 场景1: 查找函数 — 替代哨兵值（-1, nullptr, ""） ──────────

std::optional<size_t> findIndex(const std::vector<int>& v, int target) {
    for (size_t i = 0; i < v.size(); ++i)
        if (v[i] == target) return i;
    return std::nullopt;
}

void demo_find() {
    std::cout << "── 场景1: 查找 ──\n";
    std::vector<int> nums = {10, 20, 30, 40, 50};

    if (auto idx = findIndex(nums, 30)) {
        std::cout << "  找到 30 在索引 " << *idx << '\n';
    } else {
        std::cout << "  未找到 30\n";
    }

    if (auto idx = findIndex(nums, 99)) {
        std::cout << "  找到 99 在索引 " << *idx << '\n';
    } else {
        std::cout << "  未找到 99\n";
    }
}

// ── 场景2: 解析函数 — "可能失败"的转换 ─────────────────────────

std::optional<int> safeParseInt(const std::string& s) {
    try {
        size_t pos = 0;
        int val = std::stoi(s, &pos);
        if (pos != s.size()) return std::nullopt;  // 有尾随字符
        return val;
    } catch (...) {
        return std::nullopt;
    }
}

void demo_parse() {
    std::cout << "\n── 场景2: 安全解析 ──\n";

    auto printResult = [](const std::string& s) {
        auto opt = safeParseInt(s);
        std::cout << "  parse('" << s << "') = "
                  << (opt ? std::to_string(*opt) : "无值") << '\n';
    };

    printResult("42");
    printResult("  -7");
    printResult("123abc");
    printResult("hello");
}

// ── 场景3: 配置查找 — optional 作为 map 查询结果 ──────────────

class Config {
    std::map<std::string, std::string> settings;

public:
    void set(const std::string& key, const std::string& value) {
        settings[key] = value;
    }

    std::optional<std::string> get(const std::string& key) const {
        auto it = settings.find(key);
        if (it != settings.end()) return it->second;
        return std::nullopt;
    }
};

void demo_config() {
    std::cout << "\n── 场景3: 配置查询 ──\n";

    Config cfg;
    cfg.set("host", "localhost");
    cfg.set("port", "8080");

    std::cout << "  host = " << cfg.get("host").value_or("未设置") << '\n';
    std::cout << "  port = " << cfg.get("port").value_or("未设置") << '\n';
    std::cout << "  user = " << cfg.get("user").value_or("未设置") << '\n';
}

// ── 场景4: 延迟初始化 — optional 持有可能昂贵的对象 ──────────

struct Expensive {
    int value;
    Expensive(int v) : value(v) {
        std::cout << "  [构造 Expensive(" << v << ")]\n";
    }
};

class CachedService {
    std::optional<Expensive> cache;

public:
    const Expensive& get() {
        if (!cache) {
            std::cout << "  [首次访问，初始化缓存]\n";
            cache.emplace(42);  // 原地构造
        }
        return *cache;
    }
};

void demo_lazy() {
    std::cout << "\n── 场景4: 延迟初始化 ──\n";

    CachedService svc;
    std::cout << "  第一次调用:\n";
    auto& e1 = svc.get();
    std::cout << "  结果: " << e1.value << '\n';

    std::cout << "  第二次调用:\n";
    auto& e2 = svc.get();
    std::cout << "  结果: " << e2.value << " (无重复构造)\n";
}

// ── 场景5: optional 与 value_or 链式调用 ─────────────────────

struct Address { std::string city; std::string street; };
struct User { std::string name; std::optional<Address> address; };

std::optional<User> findUser(int id) {
    if (id == 1) return User{"Alice", Address{"NYC", "5th Ave"}};
    if (id == 2) return User{"Bob", std::nullopt};  // 有用户，没地址
    return std::nullopt;  // 没用户
}

void demo_chain() {
    std::cout << "\n── 场景5: 链式 optional ──\n";

    auto printCity = [](int id) {
        auto user = findUser(id);
        if (!user) {
            std::cout << "  id=" << id << ": 无用户\n";
            return;
        }
        std::cout << "  id=" << id << ": "
                  << user->name << " — "
                  << (user->address ? user->address->city : "无地址")
                  << '\n';
    };

    printCity(1);
    printCity(2);
    printCity(3);
}

// ── main ──────────────────────────────────────────────────────

int main() {
    std::cout << "=== std::optional 使用模式 ===\n\n";
    demo_find();
    demo_parse();
    demo_config();
    demo_lazy();
    demo_chain();
    return 0;
}
