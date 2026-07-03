# Week 1：从 Java 到 C++ 的第一步 — Java vs C++ 差异对照

## 一、编译与运行

| 维度 | Java | C++ |
|------|------|-----|
| 编译产物 | `.java` → `.class`（JVM 字节码） | `.cpp` → `.o`（目标文件） → 可执行文件（原生机器码） |
| 运行时 | JVM（HotSpot、GraalVM 等） | 直接在操作系统上运行，无虚拟机 |
| `main` 函数 | `public static void main(String[] args)` | `int main(int argc, char* argv[])` |
| 返回值 | `System.exit(n)` 或 `void` | `return 0;`（0 表示成功） |
| 文件组织 | 一个 `.java` 文件一个公共类 | `.hpp`（声明） + `.cpp`（实现），可多个类在同一文件 |
| 包/命名空间 | `package com.example;` + 目录对应 | `namespace example { }`，与目录无关 |
| 导入 | `import java.util.List;` | `#include <vector>`（文本替换，非符号导入） |
| 构建工具 | Maven / Gradle | CMake / Make / Bazel |

### 编译运行命令对照

```bash
# Java
javac Main.java
java Main

# C++（直接编译）
g++ -std=c++17 main.cpp -o main
./main

# C++（CMake）
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
./example-01-hello
```

## 二、基本类型对照

| Java 类型 | 大小 | C++ 等价类型 | 说明 |
|-----------|------|-------------|------|
| `byte` | 1 字节 | `int8_t`（`<cstdint>`） | C++ 的 `char` 有符号性未定义 |
| `short` | 2 字节 | `int16_t` / `short` | |
| `int` | 4 字节（固定） | `int32_t` / `int` | C++ `int` 大小平台相关（≥16 位） |
| `long` | 8 字节（固定） | `int64_t` / `long long` | C++ `long` 在 Windows 64 位仍是 4 字节 |
| `float` | 4 字节 | `float` | |
| `double` | 8 字节 | `double` | |
| `boolean` | JVM 相关 | `bool` | |
| `char` | 2 字节（UTF-16） | `char`（1 字节，ASCII） | 多字节字符用 `wchar_t` 或 UTF-8 库 |
| — | — | `size_t` | 无符号整数，用于 sizeof / 容器大小 |
| — | — | `ptrdiff_t` | 有符号整数，用于指针差值 |

### 定长类型 `<cstdint>`

```cpp
#include <cstdint>

int8_t   a = -128;        // 有符号 8 位
uint8_t  b = 255;         // 无符号 8 位
int16_t  c = -32768;
uint16_t d = 65535;
int32_t  e = -2147483648;
uint32_t f = 4294967295U;
int64_t  g = -9223372036854775807LL - 1;
uint64_t h = 18446744073709551615ULL;
```

**关键差异**：Java 基本类型大小是规范定义的；C++ 只保证最小值（如 `int` ≥ 16 位），实际大小由编译器和平台决定。推荐使用 `<cstdint>` 的定长类型。

## 三、关键差异代码示例

### 3.1 类型大小不固定

```java
// Java: int 永远是 4 字节
int x = 42;  // 固定 32 位
```

```cpp
// C++: int 大小取决于平台
#include <iostream>
int main() {
    std::cout << "int: " << sizeof(int) << " bytes\n";     // 通常 4
    std::cout << "long: " << sizeof(long) << " bytes\n";    // macOS/Linux: 8, Windows: 4
    std::cout << "long long: " << sizeof(long long) << " bytes\n"; // 始终 8
    return 0;
}
```

### 3.2 泛型支持基本类型

```java
// Java: 泛型不支持基本类型，必须装箱
List<Integer> list = new ArrayList<>();  // 自动装箱，有性能开销
```

```cpp
// C++: 模板直接支持基本类型，零开销
#include <vector>
std::vector<int> vec;        // 直接存 int，无装箱
std::vector<double> vec2;    // 直接存 double
```

### 3.3 字符串

```java
// Java: String 是不可变对象
String s = "hello";
s = s + " world";  // 创建新对象，原对象被 GC
```

```cpp
// C++: std::string 是可变对象
#include <string>
std::string s = "hello";
s += " world";     // 原地修改（可能重新分配内存）
s[0] = 'H';        // 直接修改字符

// std::string_view：只读视图，不拷贝
#include <string_view>
void process(std::string_view sv) { /* ... */ }
```

### 3.4 I/O

```java
// Java
System.out.println("Hello, " + name);
Scanner scanner = new Scanner(System.in);
String line = scanner.nextLine();
```

```cpp
// C++
#include <iostream>
#include <string>

std::cout << "Hello, " << name << std::endl;   // 使用 << 运算符
std::string line;
std::getline(std::cin, line);                   // 读取一行
int num;
std::cin >> num;                                // 读取一个整数
```

### 3.5 命名空间

```java
// Java
package com.example.math;
import com.example.text.StringUtils;
```

```cpp
// C++
namespace math {
    int max(int a, int b) { return a > b ? a : b; }
}

// 使用
using math::max;           // using 声明（推荐）
// using namespace math;   // using 指令（头文件中避免）
int result = max(3, 5);

// 全局命名空间
::max(3, 5);               // 显式调用全局的 max
```

### 3.6 溢出行为

```java
// Java: 有符号整数溢出是静默回绕（wrap-around）
int max = Integer.MAX_VALUE;
int overflow = max + 1;  // = Integer.MIN_VALUE（不报错）
```

```cpp
// C++: 有符号整数溢出是未定义行为（UB）
#include <climits>
#include <iostream>
int max = INT_MAX;
int overflow = max + 1;  // 未定义行为！编译器可能优化掉

// 无符号整数溢出是定义的（回绕）
unsigned int umax = UINT_MAX;
unsigned int uoverflow = umax + 1;  // 定义行为，结果为 0
```

## 四、快速参考卡

| 场景 | Java 做法 | C++ 做法 |
|------|----------|---------|
| 打印到控制台 | `System.out.println(x)` | `std::cout << x << '\n';` |
| 读取输入 | `new Scanner(System.in)` | `std::cin >> x;` |
| 定义常量 | `static final int N = 10;` | `const int N = 10;` 或 `constexpr int N = 10;` |
| 数组 | `int[] arr = new int[10];` | `int arr[10];`（栈上）或 `std::array<int, 10> arr;` |
| 动态数组 | `ArrayList<Integer>` | `std::vector<int>` |
| 字符串拼接 | `s1 + s2` | `s1 + s2`（同语法，语义不同） |
| 类型转换 | `(int) x` | `static_cast<int>(x)` |
| 空值 | `null` | `nullptr`（C++11+） |
| 获取类型大小 | 无内置 | `sizeof(int)` |
| 断言 | `assert condition;` | `#include <cassert>` → `assert(condition);` |
