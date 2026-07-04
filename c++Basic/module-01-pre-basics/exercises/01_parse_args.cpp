// 模块 1 习题: 命令行参数解析

#include <iostream>
#include <string>
#include <vector>

// TODO: 实现 hasFlag — 检查 argv 中是否有指定 flag
// bool hasFlag(int argc, char* argv[], const std::string& flag) { ... }

// TODO: 实现 getArg — 解析 --key=value 形式
// std::string getArg(int argc, char* argv[], const std::string& key) { ... }

int main(int argc, char* argv[]) {
    std::cout << "argc = " << argc << '\n';
    for (int i = 0; i < argc; ++i) {
        std::cout << "argv[" << i << "] = " << argv[i] << '\n';
    }

    // TODO: 测试 hasFlag 和 getArg

    return 0;
}
