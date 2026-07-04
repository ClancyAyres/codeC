// 模块 1 习题答案: 命令行参数解析

#include <iostream>
#include <string>
#include <vector>

bool hasFlag(int argc, char* argv[], const std::string& flag) {
    for (int i = 1; i < argc; ++i) {
        if (argv[i] == flag) return true;
    }
    return false;
}

std::string getArg(int argc, char* argv[], const std::string& key) {
    std::string prefix = "--" + key + "=";
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find(prefix) == 0) {
            return arg.substr(prefix.length());
        }
    }
    return "";
}

int main(int argc, char* argv[]) {
    std::cout << "argc = " << argc << '\n';
    for (int i = 0; i < argc; ++i) {
        std::cout << "argv[" << i << "] = " << argv[i] << '\n';
    }

    std::cout << "\nHas --verbose: " << (hasFlag(argc, argv, "--verbose") ? "yes" : "no") << '\n';
    std::cout << "Output file: '" << getArg(argc, argv, "output") << "'\n";

    return 0;
}
