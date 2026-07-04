// 模块 8 示例: 异常基础
// Java vs C++ 核心差异: 没有 checked exception

#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>

// ✅ 用标准异常或派生 — Core Guideline E.14
class FileOpenError : public std::runtime_error {
public:
    explicit FileOpenError(const std::string& filename)
        : std::runtime_error("Cannot open file: " + filename) {}
};

void readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw FileOpenError(filename);  // ✅ 抛出派生类
    }
    std::string line;
    std::getline(file, line);
    std::cout << "First line: " << line << '\n';
}

int main() {
    try {
        readFile("nonexistent.txt");
    }
    // ✅ 按引用捕获: 避免切片 — Core Guideline E.15
    catch (const FileOpenError& e) {
        std::cout << "File error: " << e.what() << '\n';
    }
    catch (const std::runtime_error& e) {
        std::cout << "Runtime error: " << e.what() << '\n';
    }
    catch (const std::exception& e) {
        std::cout << "Standard exception: " << e.what() << '\n';
    }
    catch (...) {
        std::cout << "Unknown exception\n";
    }

    std::cout << "Program continues after exception\n";
    return 0;
}
