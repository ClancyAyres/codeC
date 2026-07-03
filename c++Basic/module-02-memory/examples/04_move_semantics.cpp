// 模块 2 示例: 移动语义
// 相关规范: Core Guidelines C.64, F.18, ES.56
//
// 移动语义是 Java 完全没有的概念。核心思想:
// "我不要这个资源了，你拿走吧" — 避免深拷贝。

#include <iostream>
#include <string>
#include <vector>
#include <utility>

void moveString() {
    std::cout << "--- 字符串移动 ---\n";
    std::string s1 = "Hello, this is a very long string that would be expensive to copy";

    std::string s2 = s1;
    std::cout << "After copy:  s1='" << s1 << "'\n";

    // ✅ 正确: std::move 将 s1 资源转移给 s3，避免深拷贝 — Core Guidelines ES.56
    std::string s3 = std::move(s1);
    // ❌ 错误: s1 已被移动，继续使用 s1 是未定义行为 — Core Guidelines ES.56
    // Code Review 直接打回: 移动后 s1 处于"有效但未指定"状态
    std::cout << "After move:  s1='" << s1 << "' (empty or unspecified)\n";
    std::cout << "             s3='" << s3 << "'\n";
}

void moveVector() {
    std::cout << "--- vector 移动 ---\n";
    // ✅ 正确: vector 移动只转移内部指针，O(1) 而非 O(n) 拷贝 — Core Guidelines C.64
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::cout << "v1 size: " << v1.size() << '\n';

    std::vector<int> v2 = std::move(v1);
    std::cout << "After move: v1 size=" << v1.size()
              << ", v2 size=" << v2.size() << '\n';
}

// ✅ 正确: 返回局部变量自动触发移动（NRVO），无需 std::move — Core Guidelines F.48
std::vector<int> createLargeVector() {
    std::vector<int> result;
    for (int i = 0; i < 1000; ++i) {
        result.push_back(i);
    }
    return result;
}

int main() {
    moveString();
    moveVector();

    auto v = createLargeVector();
    std::cout << "Created vector with " << v.size() << " elements\n";

    return 0;
}
