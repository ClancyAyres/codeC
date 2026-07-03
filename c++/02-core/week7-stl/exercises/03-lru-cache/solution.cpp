#include <iostream>
#include <list>
#include <unordered_map>
#include <utility>

class LRUCache {
    using ListIterator = std::list<std::pair<int, int>>::iterator;

public:
    explicit LRUCache(int capacity) : capacity_(capacity) {}

    int get(int key) {
        auto it = cache_.find(key);
        if (it == cache_.end()) {
            return -1;
        }
        items_.splice(items_.begin(), items_, it->second);
        return it->second->second;
    }

    void put(int key, int value) {
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            it->second->second = value;
            items_.splice(items_.begin(), items_, it->second);
            return;
        }

        if (static_cast<int>(items_.size()) >= capacity_) {
            auto last = items_.back();
            cache_.erase(last.first);
            items_.pop_back();
        }

        items_.emplace_front(key, value);
        cache_[key] = items_.begin();
    }

private:
    int capacity_;
    std::list<std::pair<int, int>> items_;
    std::unordered_map<int, ListIterator> cache_;
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
