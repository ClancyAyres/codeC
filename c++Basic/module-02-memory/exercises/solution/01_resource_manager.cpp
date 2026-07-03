// 模块 2 习题答案: RAII 文件写入器
// 规范: Core Guidelines R.1, C.21

#include <iostream>
#include <fstream>
#include <string>
#include <utility>

class FileWriter {
public:
    explicit FileWriter(const std::string& filename)
        : file_(filename) {
        if (!file_.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& other) noexcept
        : file_(std::move(other.file_)) {}

    FileWriter& operator=(FileWriter&& other) noexcept {
        if (this != &other) {
            if (file_.is_open()) {
                file_.close();
            }
            file_ = std::move(other.file_);
        }
        return *this;
    }

    void write(const std::string& line) {
        file_ << line << '\n';
    }

    ~FileWriter() {
        if (file_.is_open()) {
            file_.close();
        }
    }

private:
    std::ofstream file_;
};

int main() {
    {
        FileWriter writer("/tmp/test_writer.txt");
        writer.write("Line 1");
        writer.write("Line 2");
    }

    std::ifstream reader("/tmp/test_writer.txt");
    std::string content;
    std::cout << "File contents:\n";
    while (std::getline(reader, content)) {
        std::cout << "  " << content << '\n';
    }

    // 测试移动
    std::cout << "\n--- 测试移动 ---\n";
    FileWriter w1("/tmp/test_move.txt");
    w1.write("Before move");

    FileWriter w2 = std::move(w1);
    w2.write("After move");

    return 0;
}
