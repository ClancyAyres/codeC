#include <iostream>

int main() {
    int x = 10, y = 20;

    // ============================================================
    // 1. const int* p1 — 指向 const int 的指针
    //    可以改变指针指向，但不能通过指针修改值
    // ============================================================
    {
        std::cout << "=== const int* (pointer to const int) ===\n";
        std::cout << "  x=" << x << ", y=" << y << '\n';

        const int* p1 = &x;
        std::cout << "  const int* p1 = &x;  *p1 = " << *p1 << '\n';

        // *p1 = 100;             // 错误：不能通过 p1 修改值
        p1 = &y;                  // 合法：指针本身可变
        std::cout << "  p1 = &y;  *p1 = " << *p1 << '\n';
        std::cout << "  x=" << x << " (unchanged)\n\n";
    }

    // ============================================================
    // 2. int* const p2 — 指向 int 的 const 指针
    //    可以修改值，但不能改变指针指向
    // ============================================================
    {
        std::cout << "=== int* const (const pointer to int) ===\n";
        std::cout << "  x=" << x << ", y=" << y << '\n';

        int* const p2 = &x;
        std::cout << "  int* const p2 = &x;  *p2 = " << *p2 << '\n';

        *p2 = 100;                // 合法：可以修改值
        std::cout << "  *p2 = 100;  x = " << x << " (modified)\n";

        // p2 = &y;               // 错误：指针本身不可变
        std::cout << "  p2 cannot point to y\n\n";
    }

    // ============================================================
    // 3. const int* const p3 — 指向 const int 的 const 指针
    //    既不能修改值，也不能改变指针指向
    // ============================================================
    {
        std::cout << "=== const int* const (const pointer to const int) ===\n";

        const int* const p3 = &x;
        std::cout << "  const int* const p3 = &x;  *p3 = " << *p3 << '\n';

        // *p3 = 200;             // 错误：不能修改值
        // p3 = &y;               // 错误：不能改变指向
        std::cout << "  Neither *p3 nor p3 can be changed\n\n";
    }

    // ============================================================
    // 4. const int& ref — const 引用
    //    不能通过引用修改值
    // ============================================================
    {
        std::cout << "=== const int& (const reference) ===\n";

        int z = 30;
        const int& ref = z;
        std::cout << "  const int& ref = z;  ref = " << ref << '\n';

        // ref = 40;              // 错误：不能通过 const 引用修改
        z = 40;                   // 合法：通过原名可以修改
        std::cout << "  z = 40;  ref = " << ref << " (reflects change)\n\n";
    }

    // ============================================================
    // 5. 助记总结
    // ============================================================
    {
        std::cout << "=== Quick Reference ===\n";
        std::cout << "  const int*     : can't change value, can change pointer\n";
        std::cout << "  int* const     : can change value, can't change pointer\n";
        std::cout << "  const int* const: can't change either\n";
        std::cout << "  const int&     : can't change through reference\n";
        std::cout << "\n  Rule: const left of *  → value is const\n";
        std::cout << "        const right of * → pointer is const\n";
    }

    return 0;
}
