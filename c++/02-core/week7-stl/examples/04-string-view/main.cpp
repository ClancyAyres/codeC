// Example 04: string_view — 零拷贝字符串视图
// 演示：避免不必要的字符串拷贝，substr 性能对比，生命周期注意事项

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <chrono>

// 函数参数使用 string_view：同时接受 string, const char*, string_view
void greet(std::string_view name) {
    std::cout << "Hello, " << name << "!\n";
}

// 解析 CSV 行：使用 string_view 避免子串拷贝
void parseCSV(std::string_view line) {
    std::cout << "Parsing: \"" << line << "\"\n";
    size_t pos = 0;
    int field = 1;
    while (pos < line.size()) {
        auto comma = line.find(',', pos);
        auto field_sv = line.substr(pos, comma - pos);
        std::cout << "  field " << field++ << ": \"" << field_sv << "\"\n";
        if (comma == std::string_view::npos) break;
        pos = comma + 1;
    }
}

// string_view 作为返回值需要小心 — 它只是视图
std::string_view getExtension(std::string_view filename) {
    auto dot = filename.rfind('.');
    if (dot == std::string_view::npos) return {};
    return filename.substr(dot);  // 零拷贝子视图
}

int main() {
    // ── 基本用法 ──────────────────────────────────────────
    std::cout << "=== Basic Usage ===\n";

    std::string s = "Hello, World!";
    std::string_view sv1 = s;               // 从 std::string 构造
    std::string_view sv2 = "Hello, World!"; // 从字面量构造
    std::string_view sv3 = sv1;             // 从 string_view 拷贝（浅拷贝）

    std::cout << "sv1: \"" << sv1 << "\"  size=" << sv1.size() << '\n';
    std::cout << "sv2: \"" << sv2 << "\"  size=" << sv2.size() << '\n';

    // ── substr 零拷贝 ─────────────────────────────────────
    std::cout << "\n=== substr: Zero-Copy ===\n";

    std::string_view world = sv1.substr(7, 5);
    std::cout << "sv1.substr(7, 5) = \"" << world << "\"\n";
    std::cout << "world.data() == sv1.data()+7? "
              << (world.data() == sv1.data() + 7 ? "yes (same buffer)" : "no")
              << '\n';

    // 对比 std::string::substr 会分配新内存
    std::string subStr = s.substr(7, 5);
    std::cout << "std::string::substr(7,5) copies data: "
              << (subStr.data() == s.data() + 7 ? "yes" : "no (new allocation)")
              << '\n';

    // ── 函数参数兼容性 ────────────────────────────────────
    std::cout << "\n=== Function Parameter Compatibility ===\n";

    greet("Alice");                // const char*
    greet(std::string("Bob"));     // std::string (temporary)
    std::string name = "Charlie";
    greet(name);                   // std::string (lvalue)
    greet(std::string_view(name)); // std::string_view

    // ── CSV 解析示例 ──────────────────────────────────────
    std::cout << "\n=== CSV Parsing with string_view ===\n";

    std::string csv = "Alice,30,Engineer,75000";
    parseCSV(csv);

    // ── 文件扩展名提取 ────────────────────────────────────
    std::cout << "\n=== Extension Extraction ===\n";

    std::string_view ext1 = getExtension("document.pdf");
    std::string_view ext2 = getExtension("Makefile");
    std::string_view ext3 = getExtension("archive.tar.gz");

    std::cout << "document.pdf  → \"" << ext1 << "\"\n";
    std::cout << "Makefile      → \"" << ext2 << "\" (empty)\n";
    std::cout << "archive.tar.gz → \"" << ext3 << "\"\n";

    // ── 前缀/后缀匹配 (C++20) ─────────────────────────────
    // starts_with / ends_with 在 C++20 可用
    // 此处演示 C++17 手动实现
    std::cout << "\n=== Prefix/Suffix Check (C++17 manual) ===\n";

    auto startsWith = [](std::string_view sv, std::string_view prefix) {
        return sv.size() >= prefix.size() &&
               sv.substr(0, prefix.size()) == prefix;
    };
    auto endsWith = [](std::string_view sv, std::string_view suffix) {
        return sv.size() >= suffix.size() &&
               sv.substr(sv.size() - suffix.size()) == suffix;
    };

    std::string_view url = "https://example.com/index.html";
    std::cout << "\"" << url << "\"\n";
    std::cout << "  starts_with(\"https\") = " << (startsWith(url, "https") ? "true" : "false") << '\n';
    std::cout << "  starts_with(\"http\")  = " << (startsWith(url, "http") ? "true" : "false") << '\n';
    std::cout << "  ends_with(\".html\")   = " << (endsWith(url, ".html") ? "true" : "false") << '\n';
    std::cout << "  ends_with(\".css\")    = " << (endsWith(url, ".css") ? "true" : "false") << '\n';

    // ── 性能对比 ──────────────────────────────────────────
    std::cout << "\n=== Performance: string vs string_view substr ===\n";

    std::string bigStr(10000, 'x');
    bigStr[5000] = 'Y';  // 在中间放一个标记

    const int iterations = 100000;

    // string::substr (分配新内存)
    {
        auto start = std::chrono::high_resolution_clock::now();
        volatile size_t total = 0;
        for (int i = 0; i < iterations; ++i) {
            auto sub = bigStr.substr(0, 100);  // 每次分配新字符串
            total += sub.size();
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "std::string::substr(0,100) × " << iterations
                  << ": " << ms << "ms\n";
    }

    // string_view::substr (零拷贝)
    {
        auto start = std::chrono::high_resolution_clock::now();
        volatile size_t total = 0;
        std::string_view sv = bigStr;
        for (int i = 0; i < iterations; ++i) {
            auto sub = sv.substr(0, 100);  // 零拷贝
            total += sub.size();
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "std::string_view::substr(0,100) × " << iterations
                  << ": " << ms << "ms\n";
    }

    // ── 生命周期陷阱 ──────────────────────────────────────
    std::cout << "\n=== Lifetime Pitfalls ===\n";

    // 危险：临时 string 被销毁后 string_view 悬空
    std::string_view dangling;
    {
        std::string temp = "temporary";
        dangling = temp;  // dangling 指向 temp 的数据
        std::cout << "Inside scope: \"" << dangling << "\" (valid)\n";
    }
    // temp 已销毁，dangling 现在是悬空指针！
    std::cout << "Outside scope: \"" << dangling << "\" (DANGLING — undefined behavior!)\n";

    // 安全做法：不要从临时对象创建 long-lived string_view
    std::cout << "Rule: string_view must not outlive the data it points to.\n";

    return 0;
}
