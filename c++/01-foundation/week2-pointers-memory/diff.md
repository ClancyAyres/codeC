# Week 2：指针与内存模型 — Java vs C++ 差异对照

## 一、引用与指针

### 1.1 核心概念对照

| 概念 | Java | C++ |
|------|------|-----|
| 对象变量 | 引用类型变量（隐式指针，不可算术运算） | 可以是对象本身、指针、或引用 |
| 指针 | 无 | `T*` — 存储地址，可算术运算 |
| 引用 | `T&`（仅方法参数，且非显式） | `T&` — 别名，必须初始化，不可为空 |
| 空值 | `null` | `nullptr`（C++11+，类型安全的空指针） |
| 取地址 | 无直接方式 | `&variable` |
| 解引用 | 自动 | `*ptr` 或 `ptr->member` |

### 1.2 代码对照

```java
// Java: 一切对象变量都是引用（指针），但隐藏了指针语义
Dog dog = new Dog();          // dog 是引用，指向堆上的对象
dog.name = "Buddy";           // 自动解引用
dog = null;                   // 引用可以为空
// dog.bark();                // NullPointerException

// Java 没有指针算术、没有取地址、没有手动解引用
```

```cpp
// C++: 显式控制指针行为
Dog  dog;                     // 栈上的对象（不是引用/指针）
Dog* ptr = &dog;              // ptr 存储 dog 的地址
ptr->name = "Buddy";          // 显式解引用（-> 等价于 (*ptr).）
dog.name  = "Buddy";          // 直接访问

Dog* p2 = nullptr;            // 空指针（C++11+）
// p2->bark();                // 未定义行为！不保证抛异常

int arr[] = {1, 2, 3};
int* p = arr;                 // 数组退化为指针
p++;                          // 指针算术：移动到下一个元素
```

### 1.3 Java 引用 vs C++ 引用

```java
// Java: 引用可以重新绑定
Dog d = new Dog("A");
d = new Dog("B");             // d 现在指向另一个对象
d = null;                     // d 可以为空
```

```cpp
// C++ 引用：不可重新绑定，不可为空
Dog d1("A");
Dog d2("B");
Dog& ref = d1;                // ref 是 d1 的别名
ref = d2;                     // 这是赋值！d1 的内容被 d2 覆盖，ref 仍绑定 d1
// ref = Dog("C");            // 合法：赋值给 d1
// Dog& ref2;                 // 错误：引用必须初始化
// int& r = nullptr;          // 错误：引用不能为空

// C++ 指针：可以重新绑定，可以为空
Dog* ptr = &d1;               // ptr 指向 d1
ptr = &d2;                    // ptr 现在指向 d2
ptr = nullptr;                // 合法
```

## 二、const vs final

### 2.1 对照表

| 维度 | Java `final` | C++ `const` |
|------|-------------|-------------|
| 基本类型变量 | `final int x = 5;` 值不可变 | `const int x = 5;` 值不可变 |
| 对象引用 | `final Dog d = new Dog();` 引用不可变，但对象可修改 | `Dog* const d = new Dog();` 指针不可变，但对象可修改 |
| 不可变对象 | `final Dog d` + 类设计为不可变 | `const Dog* d` 不能通过此指针修改对象 |
| 方法参数 | `void foo(final int x)` 较少使用 | `void foo(const int& x)` 避免拷贝，保证不修改 |
| 方法 | `final void foo()` 不能被子类覆盖 | `void foo() const` 承诺不修改成员变量（const 成员函数） |
| 类 | `final class Dog` 不能被继承 | C++11: `class Dog final` 不能被继承 |

### 2.2 const 位置（C++ 特有）

```cpp
int x = 10;
const int* p1 = &x;           // 不能通过 p1 修改 x，但 p1 可以指向别处
int* const p2 = &x;           // p2 不能指向别处，但可以通过 p2 修改 x
const int* const p3 = &x;     // 既不能通过 p3 修改 x，p3 也不能指向别处

// 助记：const 在 * 左边 → 指向的值不可变；const 在 * 右边 → 指针本身不可变
```

## 三、new/delete vs GC

### 3.1 对照表

| 维度 | Java | C++ |
|------|------|-----|
| 分配对象 | `new Dog()` | `new Dog()` 或 `new Dog[10]()` |
| 释放 | 垃圾回收器自动回收 | 必须手动 `delete` / `delete[]` |
| 内存泄漏 | 可能（引用被持有未释放） | 可能（忘记 delete） |
| 确定性析构 | 无（`finalize()` 不可靠） | 有（`delete` 立即调用析构函数） |
| RAII | 需要 try-with-resources | 核心惯用法（构造获取，析构释放） |
| 智能指针 | 无（GC 替代） | `unique_ptr`, `shared_ptr`（推荐替代裸 new/delete） |

### 3.2 关键规则

```cpp
// 规则 1：new 配对 delete
int* p = new int(42);
delete p;                      // 必须配对

// 规则 2：new[] 配对 delete[]
int* arr = new int[100];
delete[] arr;                  // 必须配对，用错是未定义行为

// 规则 3：不要 delete 两次
int* p = new int(42);
delete p;
// delete p;                   // 未定义行为！

// 规则 4：delete 后设为 nullptr（防御性编程）
int* p = new int(42);
delete p;
p = nullptr;                   // 防止悬垂指针被误用

// 规则 5：优先使用智能指针
#include <memory>
auto p = std::make_unique<int>(42);  // 自动 delete，无需手动管理
```

### 3.3 代码对照

```java
// Java: 依赖 GC，对象何时回收不确定
public void process() {
    Dog dog = new Dog();
    dog.bark();
    // dog 在方法结束后变为不可达，GC 会在未来某个时刻回收
}
```

```cpp
// C++: 手动管理（裸指针）
void process() {
    Dog* dog = new Dog();
    dog->bark();
    delete dog;                // 必须手动释放！
}

// C++: RAII（推荐）
void process() {
    Dog dog;                   // 栈上分配，函数结束时自动析构
    dog.bark();
}

// C++: 智能指针（推荐用于堆分配）
#include <memory>
void process() {
    auto dog = std::make_unique<Dog>();
    dog->bark();
    // unique_ptr 离开作用域自动 delete
}
```

## 四、数组

### 4.1 对照表

| 维度 | Java | C++ |
|------|------|-----|
| 声明 | `int[] arr = new int[10];` | `int arr[10];`（栈）或 `int* arr = new int[10];`（堆） |
| 边界检查 | 有（`ArrayIndexOutOfBoundsException`） | 无（越界是未定义行为） |
| 长度获取 | `arr.length` | `sizeof(arr)/sizeof(arr[0])`（仅栈数组）或手动记录 |
| 数组退化为指针 | 不适用 | `int arr[10];` → `int*`（丢失长度信息） |
| 多维数组 | `int[][] mat = new int[3][4];` | `int mat[3][4];`（栈）或 `int** mat`（动态） |

### 4.2 代码对照

```java
// Java: 安全的数组
int[] arr = new int[10];
arr[0] = 42;
System.out.println(arr.length);  // 10
// arr[10] = 99;                 // ArrayIndexOutOfBoundsException
```

```cpp
// C++: 无边界检查
int arr[10];                     // 栈上分配，大小必须是编译期常量
arr[0] = 42;
// arr[10] = 99;                 // 未定义行为！可能静默破坏其他内存

// 动态数组
int* dyn = new int[10];
dyn[0] = 42;
// dyn[10] = 99;                 // 同样：未定义行为
delete[] dyn;

// 推荐：std::array（栈）或 std::vector（堆）
#include <array>
#include <vector>
std::array<int, 10> arr2;        // 栈上，大小编译期确定，有 .size()
std::vector<int> vec(10);        // 堆上，大小可动态变化，有 .size()
// vec.at(10);                   // 抛出 std::out_of_range（边界检查版本）
```

### 4.3 指针与数组的等价性

```cpp
int arr[] = {10, 20, 30};
int* p = arr;

// 以下四行完全等价：
p[0];   *(p + 0);   arr[0];   *(arr + 0);

// 有趣的性质：
2[arr];  // 等价于 *(2 + arr) = arr[2] = 30
```

## 五、快速参考卡

| 场景 | Java 做法 | C++ 做法 |
|------|----------|---------|
| 创建对象 | `Dog d = new Dog();` | `Dog d;`（栈）或 `auto d = std::make_unique<Dog>();`（堆） |
| 访问成员 | `d.bark()` | `d.bark()` 或 `ptr->bark()` |
| 空值 | `null` | `nullptr` |
| 释放对象 | 无需（GC） | `delete ptr;` 或自动（智能指针） |
| 交换两变量 | 无法直接实现 `swap(a, b)` | `std::swap(a, b)` 通过引用/指针 |
| 函数修改外部变量 | 传入对象，修改其字段 | 传入指针 `T*` 或引用 `T&` |
| 传递数组到函数 | `void foo(int[] arr)` | `void foo(int* arr, size_t size)` |
| 只读参数 | `final`（少见） | `const T&` 或 `const T*` |
