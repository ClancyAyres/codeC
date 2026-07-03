# 习题 01：异常安全 Vector Push-Back

## 目标

实现提供强异常安全保证的 `push_back`。

## 要求

完成 `SimpleVector<T>::push_back`：

- 容量不够时扩容（2 倍策略，最小容量 4）
- 扩容期间元素拷贝可能抛异常
- 新元素拷贝构造可能抛异常
- **任何异常发生时，vector 状态必须与 push_back 前一致**（强保证）

## 提示

- `reserve()` 已提供基本保证实现（临时内存 + placement new + catch 回滚）
- `push_back` 要达到强保证：先在临时内存中完成所有可能抛异常的操作，成功后再替换 `data_`
- 如果当前有空余容量，只需在当前位置 placement new 构造新元素
- 如果需要扩容 + 新增：
  - 在新内存中先拷贝所有已有元素
  - 在新内存末尾 placement new 构造新元素
  - 全部成功才销毁旧元素、替换 data_
  - 任何一步失败：销毁新内存中已构造的元素，释放新内存，原 data_ 不变
- `static_cast<T*>(::operator new(n * sizeof(T)))` 分配未初始化内存
- `new (ptr) T(value)` 在指定地址构造对象（placement new）
- `ptr->~T()` 析构对象但不释放内存

## 预期输出

```
[基本] size=2 cap=4
  通过

[强保证] 尝试 push_back 会抛异常的元素
  异常: Copy failed for id=...
  size=2 (原=2)
  cap=4 (原=4)
  强保证通过: 状态未变

所有测试通过。
```
