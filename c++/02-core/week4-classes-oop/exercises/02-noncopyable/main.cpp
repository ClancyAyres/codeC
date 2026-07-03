#include <iostream>
#include <utility>

// TODO: 实现 NonCopyable 基类
// - 拷贝构造函数 = delete
// - 拷贝赋值运算符 = delete
// - 默认构造函数 = default
// - 移动构造函数 = default
// - 移动赋值运算符 = default
// - protected 析构函数

class NonCopyable {
    // TODO
};

// TODO: UniqueResource 继承 NonCopyable
class UniqueResource {
    int* data_;

public:
    // TODO: 构造函数 — 用 new 分配 int，存储传入值
    explicit UniqueResource(int value) {
        // TODO: data_ = new int(value);
        // TODO: 输出 "Creating r with value <value>"
    }

    // TODO: 析构函数 — delete data_（检查非空），输出销毁信息
    ~UniqueResource() {
        // TODO: if (data_) { delete data_; 输出 "Destroying resource <value>" }
    }

    // 拷贝 — 已通过 NonCopyable 禁止

    // TODO: 移动构造函数 — 接管资源，源对象 data_ 设为 nullptr
    UniqueResource(UniqueResource&& other) noexcept {
        // TODO
    }

    // TODO: 移动赋值运算符 — 自赋值检查，释放当前资源，接管源资源
    UniqueResource& operator=(UniqueResource&& other) noexcept {
        // TODO
        return *this;
    }

    // TODO: int value() const — 返回 *data_
    int value() const {
        // TODO
        return 0;
    }

    // TODO: void setValue(int v) — 修改 *data_
    void setValue(int v) {
        // TODO
    }
};

int main() {
    std::cout << "Creating r1 with value 42\n";
    UniqueResource r1(42);
    std::cout << "  r1.value() = " << r1.value() << '\n';

    std::cout << "Moving r1 to r2\n";
    UniqueResource r2 = std::move(r1);
    std::cout << "  r1 still valid? "
              << (r1.value() > 0 ? "yes" : "no (nullptr)") << '\n';
    std::cout << "  r2.value() = " << r2.value() << '\n';

    std::cout << "r2.setValue(100)\n";
    r2.setValue(100);
    std::cout << "  r2.value() = " << r2.value() << '\n';

    // 以下代码如果取消注释，应该编译失败：
    // UniqueResource r3 = r2;       // 错误：拷贝构造已删除
    // UniqueResource r4(r2);        // 错误：拷贝构造已删除

    return 0;
}
