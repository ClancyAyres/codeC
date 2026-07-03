# 习题 03：LRU Cache — 用 list + unordered_map 实现

## 目标

实现一个 LRU（Least Recently Used）缓存，使用 `std::list` + `std::unordered_map` 达到 O(1) 的 get/put 操作。

## 要求

```cpp
class LRUCache {
public:
    LRUCache(int capacity);

    // 获取 key 对应的 value，如果 key 不存在返回 -1
    // 访问后该 key 成为最近使用
    int get(int key);

    // 插入或更新 key-value
    // 如果容量已满，淘汰最久未使用的 key
    void put(int key, int value);
};
```

## 数据结构提示

```cpp
// list 存储 key-value 对，头部是最新使用，尾部是最久未使用
std::list<std::pair<int, int>> items_;

// unordered_map 从 key 映射到 list 中的位置
std::unordered_map<int, std::list<std::pair<int, int>>::iterator> cache_;
```

## 算法

- `get(key)`: 在 map 中查找 → 未找到返回 -1；找到则将节点移到 list 头部并返回值
- `put(key, value)`: key 存在则更新值并移到头部；不存在则检查容量，必要时删除尾部节点，然后插入头部

## 预期输出

```
LRU Cache (capacity=2):
put(1, 1)
put(2, 2)
get(1) = 1
put(3, 3)  → evicts key 2
get(2) = -1  (not found)
put(4, 4)  → evicts key 1
get(1) = -1  (not found)
get(3) = 3
get(4) = 4
```
