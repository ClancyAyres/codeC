#include <iostream>
#include <list>
#include <unordered_map>
#include <utility>
#include <cassert>

// TODO: 实现 LRUCache
// 使用 std::list + std::unordered_map 实现 O(1) get/put
class LRUCache {
public:
    explicit LRUCache(int capacity) : capacity_(capacity) {
        // TODO: 初始化
    }

    int get(int key) {
        // TODO: 查找 key
        // 找到 → 移到头部，返回值
        // 未找到 → 返回 -1
        return -1;
    }

    void put(int key, int value) {
        // TODO: 如果 key 存在 → 更新值，移到头部
        // TODO: 如果 key 不存在 → 检查容量，淘汰尾部，插入头部
    }

private:
    int capacity_;
    // TODO: 添加数据结构成员
    // std::list<std::pair<int, int>> items_;
    // std::unordered_map<int, decltype(items_)::iterator> cache_;
};

int main() {
    std::cout << "LRU Cache (capacity=2):\n";

    LRUCache cache(2);

    cache.put(1, 1);
    std::cout << "put(1, 1)\n";

    cache.put(2, 2);
    std::cout << "put(2, 2)\n";

    std::cout << "get(1) = " << cache.get(1) << '\n';

    cache.put(3, 3);
    std::cout << "put(3, 3)  → evicts key 2\n";

    std::cout << "get(2) = " << cache.get(2) << "  (not found)\n";

    cache.put(4, 4);
    std::cout << "put(4, 4)  → evicts key 1\n";

    std::cout << "get(1) = " << cache.get(1) << "  (not found)\n";
    std::cout << "get(3) = " << cache.get(3) << '\n';
    std::cout << "get(4) = " << cache.get(4) << '\n';

    return 0;
}
