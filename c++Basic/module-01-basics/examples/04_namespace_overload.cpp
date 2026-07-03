// 模块 1 示例: namespace 与函数重载/默认参数
// 相关规范: Core Guidelines SF.6, SF.7; Google Style: Namespaces

#include <iostream>
#include <string>

namespace utils {
    void log(const std::string& msg, const std::string& level = "INFO") {
        std::cout << "[" << level << "] " << msg << '\n';
    }

    int max(int a, int b) { return a > b ? a : b; }
    double max(double a, double b) { return a > b ? a : b; }

    class Buffer {
    public:
        char& at(size_t index) { return data_[index]; }
        const char& at(size_t index) const { return data_[index]; }
    private:
        char data_[256]{};
    };
}

int main() {
    utils::log("Server started");
    utils::log("Connection lost", "ERROR");

    std::cout << utils::max(3, 7) << '\n';
    std::cout << utils::max(3.1, 7.2) << '\n';

    return 0;
}
