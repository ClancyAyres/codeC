# 习题 04：Optimization Levels — 编译优化级别测试

## 目标

实现测试函数，并在不同优化等级（-O0/-O1/-O2/-O3）下编译运行，
观察编译器优化对性能的影响。

## 要求

### 1. 多项式求值（Horner vs 直接）

实现两种多项式求值方法并对比：

```cpp
// Horner 方法: a0 + x*(a1 + x*(a2 + x*a3 + ...))
double hornerPoly(const std::vector<double>& coeffs, double x);

// 直接计算: a0 + a1*x + a2*x*x + a3*x*x*x + ...
double directPoly(const std::vector<double>& coeffs, double x);
```

- Horner 需 N-1 次乘法，直接法理论需 N(N-1)/2 次
- 在 -O2 以下直接法可能慢 2-5×
- 在 -O3 -ffast-math 下编译器可能自动转换为 Horner 形式

### 2. 死代码消除（DCE）

```cpp
int deadCodeTest(int n);
```

函数内部有复杂计算，但 main 中调用时**不使用返回值**。
- -O0: 完整执行计算
- -O2: 整个函数调用可能被消除（benchmark 显示 ~0 us）

### 3. 常量折叠

```cpp
int compileTimeFold();
```

返回编译期可计算的值（如 `(1+2+...+10)²`）。
- -O0: 运行时计算
- -O1+: 编译为 `return 3025;`

### 4. 循环不变量外提（LICM）

```cpp
double loopInvariantTest(const std::vector<double>& arr);
```

循环内有不随循环变化的值（如 `sqrt(2.0)*sin(3.14)`）。
- -O0: 每次迭代都计算（用 benchmark 确认时间差）
- -O1+: 提到循环外（LICM）

## 操作步骤

```bash
# 完成代码实现后，用不同优化级别编译运行：
g++ -std=c++17 -O0 solution.cpp -o bench_O0 && ./bench_O0
g++ -std=c++17 -O1 solution.cpp -o bench_O1 && ./bench_O1
g++ -std=c++17 -O2 solution.cpp -o bench_O2 && ./bench_O2
g++ -std=c++17 -O3 solution.cpp -o bench_O3 && ./bench_O3
g++ -std=c++17 -O3 -march=native -ffast-math solution.cpp -o bench_O3fast && ./bench_O3fast
```

编译时用 `-S` 也可以查看生成的汇编：
```bash
g++ -std=c++17 -O0 -S solution.cpp -o solution_O0.s
g++ -std=c++17 -O2 -S solution.cpp -o solution_O2.s
# 对比两个 .s 文件的差异
```

## 预期结果

| 测试 | -O0 | -O2 | -O3 |
|------|-----|-----|-----|
| Horner Poly | ~慢 | 快 5-10× | 更快 |
| Direct Poly | ~慢 | 快但不如 Horner | 接近 Horner（被自动转换） |
| 死代码消除 | 正常耗时 | ~0 us（被消除） | ~0 us |
| 常量折叠 | 有耗时 | ~0 us | ~0 us |
| LICM | 慢 | 快 2-5× | 快 2-5× |
