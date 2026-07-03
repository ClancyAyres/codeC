#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // 打印所有命令行参数
    std::cout << "参数数量: " << argc << '\n';
    for (int i = 0; i < argc; ++i) {
        std::cout << "argv[" << i << "] = " << argv[i] << '\n';
    }

    // 从标准输入读取一行
    std::cout << "\n请输入你的名字: ";
    std::string name;
    std::getline(std::cin, name);

    std::cout << "你好, " << name << "! 欢迎来到 C++ 世界。\n";

    // 读取一个整数
    std::cout << "请输入一个数字: ";
    int number = 0;
    std::cin >> number;

    std::cout << "你输入的数字是: " << number << '\n';
    std::cout << "它的两倍是: " << number * 2 << '\n';

    return 0;
}
