# 习题 04：并行归约

## 目标

实现通用并行归约模板函数，支持求和、最大/最小值等任意二元归约操作。

## 要求

### parallelReduce

```cpp
template<typename T, typename BinaryOp>
T parallelReduce(const std::vector<T>& data, T init, BinaryOp op, int numThreads = 0);
```

- 将 `data` 均匀分片给 `numThreads` 个线程
- 每个线程在分片上做串行归约：`init` 作为初值，依次应用 `op`
- 主线程收集所有局部结果，再次用 `op` 归约得到最终结果
- 如果 `numThreads <= 0`，使用 `std::thread::hardware_concurrency()`

### parallelSum

```cpp
template<typename T>
T parallelSum(const std::vector<T>& data, int numThreads = 0);
```

- 调用 `parallelReduce`，初始化 `T{0}`，操作为 `std::plus<T>()`

### parallelMax

```cpp
template<typename T>
T parallelMax(const std::vector<T>& data, int numThreads = 0);
```

- 调用 `parallelReduce`，初始化 `std::numeric_limits<T>::lowest()`，操作为返回较大值的 lambda

## 提示

- 用 `std::vector<T>` 存储各线程的局部结果（索引和线程一一映射）
- 线程 lambda 捕获按引用：`[&, threadIdx]`
- 局部归约使用 `std::accumulate(begin, end, init, op)`
- 保证 `numThreads` 不超过 `data.size()`

## 预期输出

```
数据大小: 1000000
期望求和: 500000500000
期望最大值: 1000000

并行求和(4线程): 500000500000 (PASS)
并行最大值(4线程): 1000000 (PASS)
```
