# Week 2 知识点笔记：指针与内存模型

## 一、栈 vs 堆

### 1.1 内存布局

```
高地址
┌─────────────────┐
│    命令行参数     │
│    环境变量      │
├─────────────────┤
│      栈          │ ← 局部变量、函数参数、返回地址
│      ↓          │   自动管理，LIFO
│      ↑          │   大小有限（通常 1–8 MB）
│      堆          │ ← new/malloc 分配的内存
│                 │   手动管理（或智能指针）
├─────────────────┤
│   BSS 段         │ ← 未初始化的全局/静态变量
├─────────────────┤
│   数据段         │ ← 已初始化的全局/静态变量
├─────────────────┤
│   代码段         │ ← 程序指令（只读）
└─────────────────┘
低地址
```

### 1.2 栈与堆对比

| 特性 | 栈 (Stack) | 堆 (Heap) |
|------|-----------|----------|
| 分配速度 | 极快（移动栈指针） | 较慢（查找空闲块） |
| 生命周期 | 作用域结束自动释放 | 手动 delete 或智能指针 |
| 大小限制 | 有限（通常 1–8 MB） | 系统可用内存 |
| 碎片问题 | 无 | 可能产生碎片 |
| 线程安全 | 天然线程安全（每线程独立栈） | 需要同步 |
| 典型用途 | 局部变量、函数参数 | 动态大小的数据、长生命周期对象 |

### 1.3 代码示例

```cpp
struct Dog {
    const char* name;
    Dog(const char* n) : name(n) {
        std::cout << "Dog(" << name << ") constructed\n";
    }
    ~Dog() {
        std::cout << "Dog(" << name << ") destroyed\n";
    }
};

void stackDemo() {
    Dog d("StackDog");           // 栈上创建
    // d 在函数返回时自动析构
}                                // ← 输出: Dog(StackDog) destroyed

void heapDemo() {
    Dog* d = new Dog("HeapDog"); // 堆上创建
    delete d;                    // 必须手动析构
}                                // 没有 delete 则永远不会析构（内存泄漏）
```

## 二、指针

### 2.1 基本操作

```cpp
int  x   = 42;          // 普通变量
int* ptr = &x;           // ptr 存储 x 的地址

*ptr     = 100;          // 解引用：通过指针修改 x
int y    = *ptr;         // 解引用：通过指针读取 x

std::cout << ptr;        // 输出地址（如 0x7ffeedd40a3c）
std::cout << *ptr;       // 输出 100
```

### 2.2 指针声明

```cpp
int*    p1;              // 指向 int 的指针
int**   p2;              // 指向 (指向 int 的指针) 的指针
void*   p3;              // 通用指针（不能解引用，不能算术运算）
int*    p4 = nullptr;    // 空指针（C++11+，替代 NULL 和 0）

// 声明风格（都是合法的，选择一种并保持一致）：
int* p;                  // 风格 A：* 靠近类型（强调类型是指针）
int *p;                  // 风格 B：* 靠近变量名
int * p;                 // 风格 C

// 注意：int* p, q;      // p 是指针，q 是 int！（陷阱）
int *p, *q;              // 都是指针
```

### 2.3 指针算术

```cpp
int arr[] = {10, 20, 30, 40, 50};
int* p = arr;                // p 指向 arr[0]

p++;                         // p 指向 arr[1]（地址 + sizeof(int)）
p--;                         // p 指向 arr[0]（地址 - sizeof(int)）
p += 2;                      // p 指向 arr[2]（地址 + 2 * sizeof(int)）

ptrdiff_t diff = &arr[3] - &arr[0];  // 差值 = 3（元素个数，非字节数）

// 指针比较
int* begin = arr;
int* end   = arr + 5;
while (begin < end) {        // 指针可以比较
    std::cout << *begin << ' ';
    begin++;
}

// 指针与数组的等价性
p[2];       // 等价于 *(p + 2)
2[p];       // 等价于 *(2 + p) = p[2] — 有趣但别用
```

### 2.4 指针与 const

```cpp
int x = 10, y = 20;

const int* p1 = &x;          // 指向 const int 的指针
// *p1 = 100;                // 错误：不能通过 p1 修改值
p1 = &y;                     // 合法：指针本身可变

int* const p2 = &x;          // 指向 int 的 const 指针
*p2 = 100;                   // 合法：可以修改值
// p2 = &y;                  // 错误：指针本身不可变

const int* const p3 = &x;    // 指向 const int 的 const 指针
// *p3 = 100;                // 错误
// p3 = &y;                  // 错误
```

## 三、引用

### 3.1 基本规则

引用是变量的别名，不是对象，不占存储（实现上通常用指针实现，但语义上是别名）。

```cpp
int  x   = 10;
int& ref = x;                // ref 是 x 的别名

ref = 20;                    // 等价于 x = 20
std::cout << x;              // 输出 20

// 三条铁律：
// 1. 引用必须初始化
//    int& r;                // 错误！
//
// 2. 引用不能重新绑定
//    int y = 30;
//    ref = y;               // 这是赋值给 x，不是重新绑定 ref！
//
// 3. 引用不能为空
//    int& r = nullptr;      // 错误！
```

### 3.2 引用的用途

```cpp
// 用途 1：函数参数 — 避免拷贝
void processLargeObject(const BigObject& obj) {
    // 使用 obj，不会拷贝，且保证不修改
}

// 用途 2：函数参数 — 修改外部变量（输出参数）
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// 用途 3：范围 for 中修改元素
std::vector<int> v = {1, 2, 3};
for (int& x : v) {           // 用引用避免拷贝且可修改
    x *= 2;
}

// 用途 4：const 引用绑定临时对象（延长生命周期）
const std::string& s = getString();  // 临时对象生命周期延长到 s 离开作用域
```

### 3.3 指针 vs 引用选择指南

| 场景 | 推荐 | 原因 |
|------|------|------|
| 参数可为空 | 指针 | 引用不能为空 |
| 参数必须有效 | 引用 | 语义上保证非空 |
| 输出参数 | 指针（显式 &） | 调用方看到 `&` 知道会被修改 |
| 传递大对象（只读） | `const T&` | 避免拷贝 |
| 多态 | 指针或引用均可 | `Base*` 或 `Base&` |
| 动态分配 | 指针（或智能指针） | 引用不能管理动态内存 |

## 四、new 与 delete

### 4.1 基本规则

```
new     →  delete         (单个对象)
new[]   →  delete[]       (数组)
malloc  →  free           (C 风格，不调用构造/析构)

混用 = 未定义行为！
```

### 4.2 详细规则

```cpp
// 规则 1：new 配对 delete
int* p = new int(42);
delete p;

// 规则 2：new[] 配对 delete[]
int* arr = new int[10];
delete[] arr;               // 如果写成 delete arr → 未定义行为

// 规则 3：不要 delete 两次
int* p = new int(42);
delete p;
// delete p;                // 未定义行为！
p = nullptr;                // 安全：delete nullptr 是空操作

// 规则 4：不要 delete 栈上的地址
int x = 42;
// delete &x;               // 未定义行为！

// 规则 5：不要 delete 非 new 分配的指针
int* p = &x;
// delete p;                // 未定义行为！
```

### 4.3 new/delete 做了什么

```cpp
MyClass* obj = new MyClass(42);
// 1. 调用 operator new(sizeof(MyClass)) 分配内存
// 2. 在分配的内存上调用 MyClass 构造函数

delete obj;
// 1. 调用 MyClass 析构函数
// 2. 调用 operator delete(ptr) 释放内存
```

### 4.4 现代 C++：避免裸 new/delete

```cpp
// 不推荐
int* p = new int(42);
// ... 如果中间抛异常，delete 不会执行 → 内存泄漏
delete p;

// 推荐：智能指针
#include <memory>
auto p = std::make_unique<int>(42);  // C++14
// 离开作用域自动 delete

// 推荐：容器
#include <vector>
std::vector<int> v(100);             // 替代 new int[100]
// 离开作用域自动释放

// 推荐：栈分配
int x = 42;                          // 如果不需要动态生命周期
```

## 五、const 位置快速参考

### 5.1 const 指针位置速查表

```
                  const 在 * 左边 → 指向的值不可变
                  const 在 * 右边 → 指针本身不可变
```

| 声明 | 读法（从右往左） | 能改值？ | 能改指向？ |
|------|-----------------|---------|----------|
| `const int* p` | p 是指针，指向 const int | ❌ | ✅ |
| `int const* p` | 同上（语法等价） | ❌ | ✅ |
| `int* const p` | p 是 const 指针，指向 int | ✅ | ❌ |
| `const int* const p` | p 是 const 指针，指向 const int | ❌ | ❌ |

### 5.2 const 引用

```cpp
int x = 10;
const int& ref = x;          // 不能通过 ref 修改 x
// ref = 20;                 // 错误
x = 20;                      // 合法：通过原名可以修改

// const 引用可以绑定到临时对象
const int& r = 42;           // 合法：42 是临时对象，生命周期延长
// int& r2 = 42;             // 错误：非 const 引用不能绑定临时对象
```

### 5.3 const 成员函数

```cpp
class Dog {
public:
    const char* getName() const {   // const 成员函数
        // this 的类型是 const Dog*
        // name = "other";          // 错误：不能修改成员变量
        return name;
    }
private:
    const char* name;
};
```

## 六、nullptr

### 6.1 为什么需要 nullptr

```cpp
// C++11 之前的问题
void foo(int x)    { std::cout << "int: " << x << '\n'; }
void foo(char* p)  { std::cout << "pointer: " << (p ? p : "null") << '\n'; }

foo(0);       // 调用 foo(int) — 可能符合预期
foo(NULL);    // 调用 foo(int) 还是 foo(char*)？取决于 NULL 的定义！
              // 在大多数平台 NULL 定义为 0，所以调用 foo(int)
              // 这与直觉不符

// C++11 解决方案
foo(nullptr); // 一定调用 foo(char*)，nullptr 的类型是 std::nullptr_t
```

### 6.2 使用规则

```cpp
int* p = nullptr;            // 初始化指针为空
if (p == nullptr) { ... }    // 检查指针是否为空
if (p) { ... }               // 等价于 p != nullptr
if (!p) { ... }              // 等价于 p == nullptr

delete p;                    // delete nullptr 是安全的空操作
p = nullptr;                 // delete 后将指针置空
```

## 七、常见陷阱

### 7.1 悬垂指针 (Dangling Pointer)

```cpp
int* getDangling() {
    int x = 42;
    return &x;               // 返回局部变量的地址！
}                            // x 已被销毁，返回的指针悬垂

int* getDangling2() {
    int* p = new int(42);
    delete p;                // 内存已释放
    return p;                // 返回已释放的地址！
}
```

### 7.2 内存泄漏 (Memory Leak)

```cpp
void leak() {
    int* p = new int(42);
    // 忘记 delete — 内存泄漏
}

void conditionalLeak(bool flag) {
    int* p = new int(42);
    if (flag) return;        // 提前返回，没 delete — 内存泄漏
    delete p;
}
```

### 7.3 双重释放 (Double Free)

```cpp
int* p = new int(42);
int* q = p;
delete p;
delete q;                    // 未定义行为！同一块内存释放两次
```

### 7.4 数组删除错误

```cpp
int* arr = new int[10];
delete arr;                  // 错误：应该用 delete[]
// delete[] arr;             // 正确
```
