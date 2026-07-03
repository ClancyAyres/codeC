// Example 04: Diamond Inheritance — 菱形继承问题与 virtual 继承解决方案
// Java 不存在此问题（接口无状态，类只能单继承）

#include <iostream>
#include <string>

// ============================================================
// 没有 virtual 继承 — D 包含两个 A 子对象
// ============================================================
class ANonVirtual {
public:
    int value = 10;
    ANonVirtual() { std::cout << "  ANonVirtual() value=" << value << '\n'; }
    virtual ~ANonVirtual() = default;
    virtual void show() const {
        std::cout << "  ANonVirtual::show() value=" << value << '\n';
    }
};

class BNonVirtual : public ANonVirtual {
public:
    BNonVirtual() { value = 20; std::cout << "  BNonVirtual() value=" << value << '\n'; }
};

class CNonVirtual : public ANonVirtual {
public:
    CNonVirtual() { value = 30; std::cout << "  CNonVirtual() value=" << value << '\n'; }
};

class DNonVirtual : public BNonVirtual, public CNonVirtual {
public:
    DNonVirtual() { std::cout << "  DNonVirtual()\n"; }
};

// ============================================================
// 使用 virtual 继承 — D 只包含一个 A 子对象
// ============================================================
class AVirtual {
public:
    int value;
    AVirtual(int v = 10) : value(v) {
        std::cout << "  AVirtual(" << v << ") value=" << value << '\n';
    }
    virtual ~AVirtual() = default;
    virtual void show() const {
        std::cout << "  AVirtual::show() value=" << value << '\n';
    }
};

class BVirtual : virtual public AVirtual {
public:
    BVirtual() {
        std::cout << "  BVirtual()\n";
    }
};

class CVirtual : virtual public AVirtual {
public:
    CVirtual() {
        std::cout << "  CVirtual()\n";
    }
};

class DVirtual : public BVirtual, public CVirtual {
public:
    DVirtual() : AVirtual(42) {  // 最终派生类负责初始化 virtual 基类
        std::cout << "  DVirtual() — AVirtual initialized by D\n";
    }
};

void demonstrateAmbiguity() {
    std::cout << "=== Non-virtual diamond: ambiguity ===\n";
    DNonVirtual d;

    // d.value = 5;          // 编译错误：歧义
    // d.show();             // 编译错误：歧义
    d.BNonVirtual::value = 5;  // 必须显式指定路径
    d.CNonVirtual::value = 15; // 两个独立的 value

    std::cout << "  B::value = " << d.BNonVirtual::value << '\n';
    std::cout << "  C::value = " << d.CNonVirtual::value << '\n';
    std::cout << "  sizeof(DNonVirtual) = " << sizeof(d)
              << " (two A subobjects)\n";
}

void demonstrateVirtual() {
    std::cout << "\n=== Virtual diamond: single A subobject ===\n";
    DVirtual d;
    d.value = 100;  // 无歧义！
    d.show();

    std::cout << "  sizeof(DVirtual) = " << sizeof(d)
              << " (one A subobject + vptr overhead)\n";
}

void demonstrateConstructionOrder() {
    std::cout << "\n=== Construction order with virtual inheritance ===\n";
    std::cout << "Creating DVirtual:\n";
    DVirtual d;
    // 顺序：AVirtual → BVirtual → CVirtual → DVirtual
    // 注意：AVirtual 由 DVirtual 直接构造，BVirtual 和 CVirtual 中的
    //       AVirtual 初始化被忽略
}

int main() {
    demonstrateAmbiguity();
    demonstrateVirtual();
    demonstrateConstructionOrder();

    std::cout << "\n=== Summary ===\n";
    std::cout << "  Without virtual: D has TWO A subobjects → ambiguity\n";
    std::cout << "  With virtual:    D has ONE  A subobject  → clear\n";
    std::cout << "  Cost: virtual inheritance adds vptr overhead\n";

    return 0;
}
