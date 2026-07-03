// Exercise 02: Thread-Safe Blocking Queue — 完整解法

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <string>

template<typename T>
class BoundedBlockingQueue {
public:
    explicit BoundedBlockingQueue(size_t capacity)
        : capacity_(capacity) {}

    BoundedBlockingQueue(const BoundedBlockingQueue&) = delete;
    BoundedBlockingQueue& operator=(const BoundedBlockingQueue&) = delete;

    void push(T item) {
        {
            std::unique_lock<std::mutex> lock(mtx_);
            notFull_.wait(lock, [this] { return queue_.size() < capacity_; });
            queue_.push(std::move(item));
        }
        notEmpty_.notify_one();
    }

    T pop() {
        T item;
        {
            std::unique_lock<std::mutex> lock(mtx_);
            notEmpty_.wait(lock, [this] { return !queue_.empty(); });
            item = std::move(queue_.front());
            queue_.pop();
        }
        notFull_.notify_one();
        return item;
    }

    bool tryPop(T& item) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (queue_.empty()) return false;
        item = std::move(queue_.front());
        queue_.pop();
        notFull_.notify_one();
        return true;
    }

    size_t size() {
        std::lock_guard<std::mutex> lock(mtx_);
        return queue_.size();
    }

    bool isEmpty() {
        std::lock_guard<std::mutex> lock(mtx_);
        return queue_.empty();
    }

    bool isFull() {
        std::lock_guard<std::mutex> lock(mtx_);
        return queue_.size() >= capacity_;
    }

private:
    std::queue<T> queue_;
    std::mutex mtx_;
    std::condition_variable notEmpty_;
    std::condition_variable notFull_;
    size_t capacity_;
};

int main() {
    BoundedBlockingQueue<int> bq(5);

    std::thread producer([&bq] {
        for (int i = 1; i <= 10; ++i) {
            bq.push(i);
            std::cout << "[生产] " << i << " (队列大小: " << bq.size() << ")\n";
        }
        std::cout << "[生产] 完成\n";
    });

    std::thread consumer([&bq] {
        for (int i = 0; i < 10; ++i) {
            int val = bq.pop();
            std::cout << "  [消费] " << val << " (队列大小: " << bq.size() << ")\n";
        }
        std::cout << "  [消费] 完成\n";
    });

    producer.join();
    consumer.join();

    std::cout << "最终队列大小: " << bq.size() << " (期望: 0)\n";

    int item;
    if (bq.tryPop(item)) {
        std::cout << "不应执行: 队列应为空\n";
    } else {
        std::cout << "非阻塞出队正确返回 false\n";
    }

    return 0;
}
