// Exercise 02: Optional Find — 完整解法

#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <cassert>

struct User {
    int id;
    std::string name;
    std::optional<std::string> email;
};

std::optional<User> findByName(const std::vector<User>& users, const std::string& name) {
    for (const auto& u : users)
        if (u.name == name) return u;
    return std::nullopt;
}

std::optional<std::string> findEmail(const std::vector<User>& users, const std::string& name) {
    auto user = findByName(users, name);
    if (!user) return std::nullopt;
    return user->email;  // 可能是有值的 string 或 nullopt
}

std::string getOrDefault(const std::optional<std::string>& opt, const std::string& def) {
    return opt.value_or(def);
}

int safeGet(const std::optional<int>& opt) {
    return opt.value();  // 空时抛 std::bad_optional_access
}

struct ExpensiveCache {
    int data;
    explicit ExpensiveCache(int d) : data(d) {}
};

class ConfigLoader {
    std::optional<ExpensiveCache> cache_;

public:
    int getConfig() {
        if (!cache_)
            cache_.emplace(42);
        return cache_->data;
    }
};

int main() {
    std::vector<User> users = {
        {1, "Alice", "alice@example.com"},
        {2, "Bob",   std::nullopt},
        {3, "Charlie", "charlie@example.com"},
    };

    {
        auto u = findByName(users, "Bob");
        assert(u.has_value());
        assert(u->id == 2);
        std::cout << "[findByName] 找到 Bob (id=" << u->id << ")\n";
    }
    {
        auto u = findByName(users, "Diana");
        assert(!u.has_value());
        std::cout << "[findByName] Diana 未找到，返回 nullopt\n";
    }

    {
        auto e = findEmail(users, "Alice");
        assert(e.has_value() && *e == "alice@example.com");
        std::cout << "[findEmail] Alice email: " << *e << '\n';
    }
    {
        auto e = findEmail(users, "Bob");
        assert(!e.has_value());
        std::cout << "[findEmail] Bob email: 无邮箱\n";
    }
    {
        auto e = findEmail(users, "Diana");
        assert(!e.has_value());
        std::cout << "[findEmail] Diana email: 无用户\n";
    }

    {
        std::optional<std::string> opt = "hello";
        assert(getOrDefault(opt, "default") == "hello");
        opt = std::nullopt;
        assert(getOrDefault(opt, "default") == "default");
        std::cout << "[getOrDefault] 通过\n";
    }

    {
        std::optional<int> opt = 42;
        assert(safeGet(opt) == 42);
        opt = std::nullopt;
        bool caught = false;
        try {
            safeGet(opt);
        } catch (const std::bad_optional_access&) {
            caught = true;
        }
        assert(caught);
        std::cout << "[safeGet] value() 正确抛出 bad_optional_access\n";
    }

    {
        ConfigLoader loader;
        int v1 = loader.getConfig();
        int v2 = loader.getConfig();
        assert(v1 == v2);
        std::cout << "[延迟初始化] 通过 (" << v1 << ")\n";
    }

    std::cout << "\n所有测试通过。\n";
    return 0;
}
