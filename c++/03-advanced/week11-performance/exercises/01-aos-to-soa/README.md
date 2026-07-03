# 习题 01：AoS → SoA 转换

## 目标

将已有的 AoS（Array of Structures）粒子系统重构为 SoA（Structure of Arrays），
并对比两种内存布局的缓存性能差异。

## 要求

1. **定义 `ParticleSoA` 结构体**：
   - 成员为多个 `std::vector<float>` 和 `std::vector<int>`
   - 构造函数接受 `size_t n`，初始化所有数组到大小 n

2. **实现 `convertToSoA`**：
   - 将 `std::vector<ParticleAoS>` 转换为 `ParticleSoA`
   - 逐字段拷贝数据

3. **实现 SoA 版操作函数**：
   - `updatePositionSoA(ParticleSoA&, float dt)` — 仅访问 x,y,z,vx,vy,vz
   - `decayLifeSoA(ParticleSoA&, float decay)` — 仅访问 life, mass

4. **基准对比**：在 main 中调用 SoA 版本并输出时间

## 提示

- SoA 的核心思想：把相同字段的所有值放在连续内存中
- `std::vector<float> x, y, z;` 意味着 x[0], x[1], ... 连续存储
- `updatePositionSoA` 中循环访问 `x[i], y[i], z[i], vx[i], vy[i], vz[i]`
  虽然看起来访问了 6 个不同数组，但现代 CPU 的 L1 缓存可并行处理
- 关键在于每条 cache line（64 字节）装的是 16 个连续的 float
  而不是 1 个 ParticleAoS（其中还有不用的字段）

## 预期结果

- SoA 版位置更新应快于 AoS 版（2-5×）
- SoA 版生命衰减也应快于 AoS 版
- sizeof(ParticleAoS) 约 36 字节，一条 cache line 仅装 ~1.8 个对象
- SoA 中每条 cache line 可装 16 个连续的 float → 100% 利用率
