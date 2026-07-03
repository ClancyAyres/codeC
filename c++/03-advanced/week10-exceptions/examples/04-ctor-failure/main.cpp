// Example 04: Constructor Failure — 构造失败与 RAII 资源清理
// 目的：展示构造函数抛异常时 RAII 如何保证无泄露

#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>
#include <vector>
#include <fstream>

// ── 场景1: 无 RAII — 手动管理导致泄露 ─────────────────────────

class Connection {
    int id;
public:
    Connection(int id) : id(id) {
        std::cout << "  Connection(" << id << ") 构造\n";
    }
    ~Connection() {
        std::cout << "  Connection(" << id << ") 析构\n";
    }
    int getId() const { return id; }
};

class Statement {
    int id;
public:
    Statement(const Connection& conn, int id) : id(id) {
        std::cout << "  Statement(" << id << ") 构造 (conn " << conn.getId() << ")\n";
    }
    ~Statement() {
        std::cout << "  Statement(" << id << ") 析构\n";
    }
};

// 危险版本：手动 new/delete，构造中抛异常就泄露
class DatabaseBad {
    Connection* conn;
    Statement* stmt;

public:
    DatabaseBad(int id, bool throwOnStmt)
        : conn(nullptr)
        , stmt(nullptr)
    {
        conn = new Connection(id);         // 如果抛异常，ok
        if (throwOnStmt)
            throw std::runtime_error("Statement 构造失败!");
        stmt = new Statement(*conn, id);   // 如果抛异常，conn 泄露！
    }

    ~DatabaseBad() {
        delete stmt;
        delete conn;
        std::cout << "  DatabaseBad 析构\n";
    }
};

// 安全版本：用 unique_ptr — RAII 自动处理所有分支
class DatabaseGood {
    std::unique_ptr<Connection> conn;
    std::unique_ptr<Statement> stmt;

public:
    DatabaseGood(int id, bool throwOnStmt)
        : conn(std::make_unique<Connection>(id))
        , stmt(nullptr)  // 先初始化为空
    {
        if (throwOnStmt)
            throw std::runtime_error("Statement 构造失败!");
        // 如果这里抛异常：
        // 1. stmt 还是 nullptr，析构安全（delete null 是 no-op）
        // 2. conn 已完全构造，其 unique_ptr 析构会调用 Connection 析构
        stmt = std::make_unique<Statement>(*conn, id);
    }
    // 不需要手写析构函数 — 成员 unique_ptr 自动清理
};

void demo_raw_vs_raii() {
    std::cout << "── 场景1: 手动管理 vs RAII ──\n";

    std::cout << "\n[手动 new/delete — 构造失败]\n";
    try {
        DatabaseBad db(1, true);  // throwOnStmt = true
    } catch (const std::exception& e) {
        std::cout << "  捕获异常: " << e.what() << '\n';
        std::cout << "  ⚠ Connection(1) 析构了吗？没有！内存泄露！\n";
    }

    std::cout << "\n[RAII unique_ptr — 构造失败]\n";
    try {
        DatabaseGood db(2, true);
    } catch (const std::exception& e) {
        std::cout << "  捕获异常: " << e.what() << '\n';
        std::cout << "  ✓ Connection(2) 自动析构，无泄露\n";
    }
}

// ── 场景2: 多层构造 — 每层都安全 ────────────────────────────

struct Widget {
    std::string name;
    explicit Widget(std::string n) : name(std::move(n)) {
        std::cout << "  Widget(" << name << ") 构造\n";
    }
    ~Widget() {
        std::cout << "  Widget(" << name << ") 析构\n";
    }
};

class Container {
    std::unique_ptr<Widget> w1;
    std::unique_ptr<Widget> w2;
    std::unique_ptr<Widget> w3;

public:
    Container()
        : w1(std::make_unique<Widget>("w1"))
        , w2(std::make_unique<Widget>("w2"))
        , w3(nullptr)
    {
        // 模拟 w3 构造失败
        throw std::runtime_error("w3 创建失败");
    }
};

void demo_multilayer() {
    std::cout << "\n── 场景2: 多层构造回滚 ──\n";

    try {
        Container c;
    } catch (const std::exception& e) {
        std::cout << "  捕获异常: " << e.what() << '\n';
        std::cout << "  ✓ w1 和 w2 自动析构，w3 未构造所以无析构\n";
    }
}

// ── 场景3: 构造函数 + 文件资源 ──────────────────────────────

class ConfigFile {
    std::string path;
    std::string content;

    std::string readFile(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file)
            throw std::runtime_error("无法打开文件: " + filepath);
        std::string result;
        std::string line;
        while (std::getline(file, line))
            result += line + '\n';
        return result;
    }

public:
    explicit ConfigFile(std::string filepath)
        : path(std::move(filepath))     // 先初始化成员（不抛）
        , content(readFile(path))       // 如果 readFile 抛异常：
                                        // path 已完全构造 → 析构时正确释放
                                        // content 尚未构造 → 不需要清理
    {
        std::cout << "  ConfigFile(" << path << ") 加载成功: "
                  << content.size() << " bytes\n";
    }
};

void demo_file() {
    std::cout << "\n── 场景3: 构造函数中的文件资源 ──\n";

    try {
        ConfigFile cfg("/nonexistent/config.txt");
    } catch (const std::exception& e) {
        std::cout << "  捕获异常: " << e.what() << '\n';
        std::cout << "  ✓ path 成员自动析构，无资源泄露\n";
    }
}

// ── 场景4: vector 中的构造失败 ──────────────────────────────

struct Element {
    int id;
    bool throwOnCtor;

    Element(int id, bool throwOnCtor = false)
        : id(id), throwOnCtor(throwOnCtor)
    {
        std::cout << "  Element(" << id << ") 构造\n";
        if (throwOnCtor)
            throw std::runtime_error("Element(" + std::to_string(id) + ") 构造失败");
    }

    Element(const Element& other)
        : id(other.id), throwOnCtor(other.throwOnCtor)
    {
        std::cout << "  Element(" << id << ") 拷贝构造\n";
        if (throwOnCtor)
            throw std::runtime_error("Element(" + std::to_string(id) + ") 拷贝失败");
    }

    ~Element() {
        std::cout << "  Element(" << id << ") 析构\n";
    }
};

void demo_vector() {
    std::cout << "\n── 场景4: vector 中的元素构造失败 ──\n";

    std::cout << "\n[push_back 失败]\n";
    try {
        std::vector<Element> v;
        v.push_back(Element(1));
        v.push_back(Element(2));
        v.push_back(Element(3, true));  // 第3个元素抛异常
    } catch (const std::exception& e) {
        std::cout << "  捕获异常: " << e.what() << '\n';
        std::cout << "  ✓ vector 保证已构造的元素被析构\n";
    }
}

// ── main ──────────────────────────────────────────────────────

int main() {
    std::cout << "=== 构造函数失败与 RAII ===\n\n";
    demo_raw_vs_raii();
    demo_multilayer();
    demo_file();
    demo_vector();
    std::cout << "\n所有演示完成。\n";
    return 0;
}
