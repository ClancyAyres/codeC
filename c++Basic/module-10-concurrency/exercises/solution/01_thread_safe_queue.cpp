#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <string>

template<typename T>
class ThreadSafeQueue {
public:
    void push(T value) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(value));
        cv_.notify_one();
    }

    bool tryPop(T& out) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) return false;
        out = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this] { return !queue_.empty(); });
        T value = std::move(queue_.front());
        queue_.pop();
        return value;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

private:
    mutable std::mutex mutex_;
    std::queue<T> queue_;
    std::condition_variable cv_;
};

int main() {
    ThreadSafeQueue<int> queue;

    // 生产者
    std::thread producer([&queue]() {
        for (int i = 0; i < 10; ++i) {
            queue.push(i);
            std::cout << "Produced: " << i << '\n';
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });

    // 多个消费者
    std::vector<std::thread> consumers;
    std::mutex coutMutex;
    for (int c = 0; c < 2; ++c) {
        consumers.emplace_back([&queue, &coutMutex, c]() {
            for (int i = 0; i < 5; ++i) {
                int value = queue.pop();
                std::lock_guard<std::mutex> lock(coutMutex);
                std::cout << "Consumer " << c << " got: " << value << '\n';
            }
        });
    }

    producer.join();
    for (auto& t : consumers) t.join();

    std::cout << "Queue empty: " << queue.empty() << '\n';

    return 0;
}
