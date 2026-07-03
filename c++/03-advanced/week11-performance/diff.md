# Week 11：性能优化基础 — Java vs C++ 差异对照

## 一、JIT vs AOT 优化

```java
// Java — JIT（Just-In-Time）编译器运行时优化
// HotSpot C1（快速编译）→ C2（激进优化）分层编译
public int sumArray(int[] arr) {
    int sum = 0;
    for (int x : arr) {
        sum += x;               // 运行时：
    }                           // → 循环展开（loop unrolling）
    return sum;                 // → 逃逸分析（栈分配代替堆分配）
}                               // → 内联（inline）热点方法
                                // → 去虚拟化（虚调用→直接调用）
```

```cpp
// C++ — AOT（Ahead-Of-Time）编译器静态优化
// 在编译期完成所有优化
[[gnu::noinline]]
int sumArray(const int* arr, size_t n) {
    int sum = 0;
    for (size_t i = 0; i < n; ++i) {
        sum += arr[i];          // 编译器在-O2/-O3进行：
    }                           // → 自动向量化（SIMD）
    return sum;                 // → 循环展开 + 循环融合
}                               // → 常量折叠和传播
                                // → 死代码消除
                                // 没有运行时性能监控反馈
```

| 特性 | Java JIT | C++ AOT |
|------|----------|---------|
| 编译时机 | 运行时（解释→JIT编译） | 构建时 |
| 暖身开销 | 有（warm-up，需要预热） | 无 |
| 运行时反馈 | 收集热点，推测优化，可反优化 | 无，编译后固定 |
| 跨函数优化 | 有限（JIT时间预算） | 激进（LTO全程序优化） |
| 最终性能峰值 | 接近C++（长期运行） | 通常最高 |
| 确定性 | 低（GC暂停/JIT编译暂停） | 高 |
| 平台相关 | 一次编写到处运行 | 需为每个目标编译 |
| Profile-Guided | 隐式（JIT自然收集） | 显式：-fprofile-generate → -fprofile-use（PGO） |

## 二、对象头开销

```java
// Java — 每个对象都有对象头（Object Header）
// 在64位JVM中，对象头至少占 12-16 字节：
//   - Mark Word:    8 字节（锁状态、GC分代年龄、identity hash code）
//   - Klass Pointer: 4 字节（压缩）或 8 字节（未压缩，指向类元数据）
// 再加上 8 字节对齐 → 空对象占 16 字节

class Point {           // 对象头 12-16 字节
    int x;              //     int x:     4 字节
    int y;              //     int y:     4 字节
}                       // + padding:    0-4 字节
                        // = 总计:      24 字节（两个 int 本只需 8 字节！）

// 数组还有额外 4 字节用于存储长度
int[] arr = new int[4]; // 对象头(12-16) + 长度(4) + 数据(4×4)
                        // = 32-40 字节（数据只有 16 字节）
```

```cpp
// C++ — 无对象头，内存布局完全由开发者控制
// 默认布局遵循成员声明顺序（可能有对齐填充）

struct Point {          // 无对象头
    int x;              //     int x:     4 字节
    int y;              //     int y:     4 字节
};                      // = 总计:        8 字节（精确！）

// 即使在容器中也没有额外开销
std::vector<int> vec(4); // 堆数据: 4×4=16 字节（无对象头！）
                         // vector本身: 通常 3 个指针 = 24 字节（在栈上）

// 甚至可以不经过内存分配
int arr[4];             // 直接在栈上: 16 字节
```

| 方面 | Java | C++ |
|------|------|-----|
| 每个对象附加内存 | 12-16 字节（对象头） | 0 字节 |
| 数组额外内存 | 对象头 + 4 字节长度 | 0（如栈数组） |
| `int[1000]` 总内存 | ~4024 字节 | 4000 字节（或在栈上用 `std::array`） |
| 内存布局控制 | JVM 决定 | 完全控制（含 padding 控制） |
| 值类型 | 只有引用类型 + 基本类型 | 所有类型都可值语义 |
| 小对象惩罚 | 严重（对象头占比高） | 无 |

## 三、精确内存布局

```java
// Java — 无法控制内存布局
class Entity {
    byte   flags;       // JVM 可能重排字段以优化内存
    int    id;          // 实际布局取决于 JVM 实现
    double health;      // 开发者无法预测或控制
}
// 即使理论最小 1+4+8=13 字节，实际可能达到 24-32 字节
// 且 JVM 可能在 GC 时移动对象（compaction）
```

```cpp
// C++ — 完全控制内存布局
struct Entity {
    double health;      // 8 字节（放前面避免 padding）
    int    id;          // 4 字节
    uint8_t flags;      // 1 字节
};                      // → alignof 8 → sizeof 16 (含 3 字节尾部填充)
// 总计 16 字节，完全可预测

// 紧凑布局（取消对齐）：
#pragma pack(push, 1)
struct PackedEntity {
    double health;      // 8 字节（可能非对齐！慢）
    int    id;          // 4 字节
    uint8_t flags;      // 1 字节
};                      // sizeof = 13
#pragma pack(pop)

// 对齐控制：
struct alignas(64) CacheLineAligned {
    int data;
    char padding[60];   // 手动填充到 64 字节
};

// 热/冷数据分离（Cache 友好）：
struct EntityHot {      // 频繁访问的字段放一起
    double x, y, z;     // 3×8 = 24 字节（占用不到半条 cache line）
    float health;
};
struct EntityCold {     // 很少访问的字段分开放
    std::string name;
    std::string description;
};
// 这样遍历 EntityHot 数组时，每 64 字节 cache line 能装下 2+ 个元素
```

## 四、Profile-Guided Optimization

```java
// JIT 自然就是 PGO：运行时持续收集统计
// 分支预测 → 热路径内联 → 去虚拟化 → 然后可能反优化
// 开发者无需手动操作
```

```cpp
// C++ PGO 需要显式三步流程：
// 1. 插桩编译
//    g++ -O2 -fprofile-generate -o program program.cpp
// 2. 用代表性数据集运行（生成 .gcda 文件）
//    ./program < typical_workload.dat
// 3. 用反馈数据重新编译
//    g++ -O2 -fprofile-use -o program program.cpp
// 结果：相当于免费获得 10-20% 性能提升
```

## 五、快速参考卡

| 场景 | Java 做法 | C++ 做法 |
|------|----------|---------|
| 编译优化 | 依赖 JIT，预热后自动内联/去虚拟化 | `-O2`/`-O3` + 源码调整 |
| 内存布局控制 | 不可控（JVM 决定） | `struct` + `alignas` + 排列成员 |
| 避免对象头 | 不可行（除 `int`/`long` 等基本类型） | 值语义默认无头 |
| 缓存行对齐 | 不可控 | `alignas(64)` |
| 配置文件引导优化 | JIT 自动做 | `-fprofile-generate` + 运行 + `-fprofile-use` |
| SIMD | JIT 自动向量化（有限） | 编译器自动向量化 + 手写 intrinsics |
| 逃逸分析 | JIT 逃逸分析 → 栈分配 | 直接值语义（本就在栈上） |
| 链接时优化 | 不适用（单个类文件） | `-flto`（链接时优化） |
| 性能分析 | JFR / async-profiler | `perf` / `valgrind --tool=callgrind` |
