# 习题 02：Benchmark Macro — 实现基准测试宏

## 目标

实现一个便捷的 `BENCHMARK` 宏，封装 `<chrono>` 计时逻辑，
简化微基准测试的编写。

## 要求

### 基础要求

实现 `BENCHMARK(block, iterations, description)` 宏：

```cpp
BENCHMARK({
    int s = 0;
    for (int i = 0; i < 1000; ++i) s += i;
    doNotOptimize(s);
}, 100, "sum 1..1000");
```

输出格式：
```
[sum 1..1000]  平均: 123.45 ns  (100 iters)
```

具体要求：
1. 预热 1 次（执行 block 但不计时）
2. 执行 iterations 次，用 `steady_clock` 记录总耗时
3. 输出平均纳秒耗时和迭代次数
4. 使用 `do { ... } while(0)` 包装宏体

### 进阶要求

实现 `BENCHMARK_MULTI(block, warmup, measure, description)` 宏：
- 预热 `warmup` 次
- 测量 `measure` 次
- 输出平均、中位数、最小值

## 提示

- 宏中变量名可能污染外部命名空间——使用 `__` 前缀或 `do{}while(0)` 内局部变量
- `std::chrono::duration_cast<std::chrono::nanoseconds>(...)` 获取纳秒
- 中位数需要收集每次测量的时间 → 用 `std::vector<double>`
- 切记不要用 `system_clock`（可能受 NTP 调整影响），用 `steady_clock`

## 测试用例

代码中预置了两个测试场景：

1. **`vector::reserve`** — 有/无 reserve 的性能对比
2. **Row-major vs Column-major** — 矩阵遍历顺序对缓存的影响
