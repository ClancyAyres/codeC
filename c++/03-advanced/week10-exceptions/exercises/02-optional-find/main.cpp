// Exercise 02: Optional Find — 用 std::optional 实现键值查找
// TODO: 实现 find, value_or 和链式查找

#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <cassert>

// ── 任务1: 实现 findByName — 在对象列表中查找 ──────────────

struct User {
    int id;
    std::string name;
    std::optional<std::string> email;  // 可选字段
};

// TODO: 实现 findByName，返回找到的 User
// - 找到 → 返回该 User
// - 未找到 → 返回 std::nullopt
std::optional<User> findByName(const std::vector<User>& users, const std::string& name) {
    // TODO
    return std::nullopt;
}

// TODO: 实现 findEmail，返回用户的 email（如果存在）
// - 先查找用户
// - 用户存在 → 返回其 email（可能为 nullopt）
// - 用户不存在 → 返回 nullopt
std::optional<std::string> findEmail(const std::vector<User>& users, const std::string& name) {
    // TODO
    return std::nullopt;
}

// ── 任务2: 实现 getOrDefault ────────────────────────────────

// TODO: 实现 getOrDefault，用 value_or 获取值
// - 有值 → 返回值
// - 无值 → 返回默认值
std::string getOrDefault(const std::optional<std::string>& opt, const std::string& def) {
    // TODO
    return def;
}

// ── 任务3: 实现 safeGet（value vs operator*） ────────────────

// TODO: 实现 safeGet，展示 value() 和 operator* 的区别
// - 如果 opt 有值 → 返回 *opt
// - 如果 opt 无值，useValue → 抛出 std::bad_optional_access
//   （用 opt.value() 而不用 *opt，因为 operator* 在无值时是 UB）
int safeGet(const std::optional<int>& opt) {
    // TODO: 使用 opt.value() — 空时抛 std::bad_optional_access
    return 0;
}

// ── 任务4: 延迟初始化 ────────────────────────────────────────

struct ExpensiveCache {
    int data;
    explicit ExpensiveCache(int d) : data(d) {
        // 模拟昂贵构造
    }
};

class ConfigLoader {
    std::optional<ExpensiveCache> cache_;

public:
    // TODO: 实现 getConfig
    // - 第一次调用时构造 cache_
    // - 后续调用直接返回已有值
    int getConfig() {
        // TODO
        return 0;
    }
};

// ── main ──────────────────────────────────────────────────────

int main() {
    std::vector<User> users = {
        {1, "Alice", "alice@example.com"},
        {2, "Bob",   std::nullopt},
        {3, "Charlie", "charlie@example.com"},
    };

    // 测试 findByName
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

    // 测试 findEmail
    {
        auto e = findEmail(users, "Alice");
        assert(e.has_value() && *e == "alice@example.com");
        std::cout << "[findEmail] Alice email: " << *e << '\n';
    }
    {
        auto e = findEmail(users, "Bob");
        assert(!e.has_value());  // 用户存在但无邮箱 → nullopt
        std::cout << "[findEmail] Bob email: 无邮箱\n";
    }
    {
        auto e = findEmail(users, "Diana");
        assert(!e.has_value());
        std::cout << "[findEmail] Diana email: 无用户\n";
    }

    // 测试 getOrDefault
    {
        std::optional<std::string> opt = "hello";
        assert(getOrDefault(opt, "default") == "hello");
        opt = std::nullopt;
        assert(getOrDefault(opt, "default") == "default");
        std::cout << "[getOrDefault] 通过\n";
    }

    // 测试 safeGet
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

    // 测试延迟初始化
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
