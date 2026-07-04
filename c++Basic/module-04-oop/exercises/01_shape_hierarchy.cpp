// 模块 3 习题: 图形层次结构
// 规范: Core Guidelines C.35, C.121, C.128

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>

// TODO: 定义抽象基类 Shape
// class Shape { ... };

// TODO: 实现 Circle
// class Circle : public Shape { ... };

// TODO: 实现 Rectangle
// class Rectangle : public Shape { ... };

// TODO: 实现工厂函数
// std::unique_ptr<Shape> createShape(...) { ... }

int main() {
    // 使用工厂函数创建图形
    std::vector<std::unique_ptr<Shape>> shapes;
    // TODO: 添加 Circle 和 Rectangle 并计算总面积

    return 0;
}
