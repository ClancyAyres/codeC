#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>

class FileHandle {
    FILE* file_;

public:
    FileHandle(const std::string& path, const std::string& mode)
        : file_(std::fopen(path.c_str(), mode.c_str())) {
        if (!file_) {
            throw std::runtime_error("Failed to open file: " + path);
        }
        std::cout << "[RAII] File '" << path << "' opened\n";
    }

    ~FileHandle() {
        if (file_) {
            std::fclose(file_);
            std::cout << "[RAII] File closed (destructor)\n";
        }
    }

    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    FileHandle(FileHandle&& other) noexcept
        : file_(other.file_) {
        other.file_ = nullptr;
        std::cout << "[RAII] File moved\n";
    }

    FileHandle& operator=(FileHandle&& other) noexcept {
        if (this != &other) {
            if (file_) std::fclose(file_);
            file_ = other.file_;
            other.file_ = nullptr;
        }
        std::cout << "[RAII] File move-assigned\n";
        return *this;
    }

    void writeLine(const std::string& line) {
        std::fprintf(file_, "%s\n", line.c_str());
        std::fflush(file_);
    }

    bool readLine(std::string& out) {
        char buffer[1024];
        if (std::fgets(buffer, sizeof(buffer), file_)) {
            // 去除末尾换行符
            size_t len = 0;
            while (buffer[len] != '\0') ++len;
            if (len > 0 && buffer[len - 1] == '\n') buffer[len - 1] = '\0';
            out = buffer;
            return true;
        }
        return false;
    }
};

void writeExample() {
    std::cout << "=== writeExample ===\n";
    FileHandle f("test_raii.txt", "w");
    f.writeLine("Hello, RAII!");
    f.writeLine("This file will be auto-closed.");
    std::cout << "Lines written.\n";
    // f 离开作用域，析构函数自动调用 fclose
}

void readExample() {
    std::cout << "\n=== readExample ===\n";
    FileHandle f("test_raii.txt", "r");
    std::string line;
    while (f.readLine(line)) {
        std::cout << "Read: " << line << '\n';
    }
    // f 离开作用域，析构函数自动调用 fclose
}

void exceptionExample() {
    std::cout << "\n=== exceptionExample ===\n";
    try {
        FileHandle f("test_raii.txt", "r");
        std::cout << "File opened, about to throw...\n";
        throw std::runtime_error("Simulated error");
        // f 不会泄漏 — 栈展开时析构函数自动调用 fclose
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << '\n';
    }
}

void moveExample() {
    std::cout << "\n=== moveExample ===\n";
    FileHandle f1("test_raii.txt", "r");
    std::cout << "Moving f1 to f2...\n";
    FileHandle f2 = std::move(f1);
    // f1 现在为空（file_ == nullptr）
    std::string line;
    if (f2.readLine(line)) {
        std::cout << "Read via f2: " << line << '\n';
    }
}

int main() {
    writeExample();
    readExample();
    exceptionExample();
    moveExample();

    // 清理
    std::remove("test_raii.txt");
    std::cout << "\nAll examples complete. No file handles leaked.\n";
    return 0;
}
