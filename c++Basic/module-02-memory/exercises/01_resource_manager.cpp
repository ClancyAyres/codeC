// 模块 2 习题: RAII 文件写入器
// 规范: Core Guidelines R.1, C.21

#include <iostream>
#include <fstream>
#include <string>

class FileWriter {
public:
    // TODO: 构造函数 — 打开文件
    explicit FileWriter(const std::string& filename) {
        // ...
    }

    // TODO: 禁用拷贝
    // FileWriter(const FileWriter&) = ...;
    // FileWriter& operator=(const FileWriter&) = ...;

    // TODO: 实现移动构造
    // FileWriter(FileWriter&& other) noexcept { ... }

    // TODO: 实现移动赋值
    // FileWriter& operator=(FileWriter&& other) noexcept { ... }

    // TODO: 写入一行
    void write(const std::string& line) {
        // ...
    }

    // TODO: 析构函数 — 关闭文件
    ~FileWriter() {
        // ...
    }

private:
    std::ofstream file_;
};

int main() {
    // 测试基本写入
    {
        FileWriter writer("/tmp/test_writer.txt");
        writer.write("Line 1");
        writer.write("Line 2");
    }

    // TODO: 验证文件内容
    std::ifstream reader("/tmp/test_writer.txt");
    std::string content;
    std::cout << "File contents:\n";
    while (std::getline(reader, content)) {
        std::cout << "  " << content << '\n';
    }

    return 0;
}
