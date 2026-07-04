#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <sstream>

class FileOpenError : public std::runtime_error {
public:
    explicit FileOpenError(const std::string& filename)
        : std::runtime_error("Cannot open file: " + filename) {}
};

std::string readFileSafe(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw FileOpenError(filename);
    }

    // RAII: file 在异常时仍然会析构（自动关闭）
    std::ostringstream content;
    content << file.rdbuf();

    if (file.bad()) {
        throw std::runtime_error("Failed to read file: " + filename);
    }

    return content.str();
    // file 析构，自动关闭 — 安全
}

int main() {
    // 测试正常文件
    {
        std::ofstream tmp("/tmp/test_safe_read.txt");
        tmp << "Hello\nWorld\n";
        tmp.close();

        try {
            auto content = readFileSafe("/tmp/test_safe_read.txt");
            std::cout << "File content:\n" << content;
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << '\n';
        }
    }

    // 测试不存在的文件
    try {
        readFileSafe("/tmp/nonexistent_file.txt");
    } catch (const FileOpenError& e) {
        std::cout << "Expected error: " << e.what() << '\n';
    }

    return 0;
}
