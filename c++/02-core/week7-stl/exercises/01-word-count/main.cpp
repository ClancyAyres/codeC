#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

// TODO: 实现 wordCount 函数
// 输入文本，返回每个单词出现次数的映射
// - 单词由字母组成，忽略大小写
// - 忽略标点符号
std::unordered_map<std::string, int> wordCount(const std::string& text) {
    std::unordered_map<std::string, int> counts;
    // TODO: 用 istringstream 分割单词
    // TODO: 过滤非字母字符，统一转小写
    // TODO: 统计每个单词的出现次数
    return counts;
}

int main() {
    std::string text = R"(
        C++ is a powerful language.
        Cpp is powerful, and C++ is fast!
        The C++ standard library provides many containers.
        The library is well-designed and the library is efficient.
    )";

    auto counts = wordCount(text);

    // TODO: 按频率降序排序并输出
    // 提示：将 unordered_map 转为 vector<pair>，按 second 降序排序

    std::cout << "Word frequency (descending):\n";

    return 0;
}
