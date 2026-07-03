// Exercise 02: Thread-Safe Queue
// TODO: 实现一个线程安全的有限阻塞队列

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <string>

// TODO: 实现线程安全队列模板类
// 功能:
//   - push(T): 入队，若队列满则阻塞等待空间
//   - pop(): 出队，若队列空则阻塞等待数据
//   - tryPop(T&): 非阻塞出队，队列空返回 false
//   - size(): 当前元素数
//   - isEmpty(): 是否为空
//   - isFull(): 是否为满
template<typename T>
class BoundedBlockingQueue {
public:
    // TODO: 构造函数，指定最大容量
    BoundedBlockingQueue(size_t capacity);

    // TODO: 禁止拷贝和移动
    BoundedBlockingQueue(const BoundedBlockingQueue&) = delete;
    BoundedBlockingQueue& operator=(const BoundedBlockingQueue&) = delete;

    // TODO: 阻塞入队
    void push(T item);

    // TODO: 阻塞出队
    T pop();

    // TODO: 非阻塞出队（返回 true 表示成功，item 被填充）
    bool tryPop(T& item);

    // TODO: 返回当前元素数量（线程安全）
    size_t size();

    // TODO: 判断是否为空
    bool isEmpty();

    // TODO: 判断是否已满
    bool isFull();

private:
    // TODO: 成员变量
    // std::queue<T> queue_;
    // std::mutex mtx_;
    // std::condition_variable notEmpty_;
    // std::condition_variable notFull_;
    // size_t capacity_;
};

// ── 测试代码 ──────────────────────────────────────────────────────

int main() {
    BoundedBlockingQueue<int> bq(5);

    // 生产者线程
    std::thread producer([&bq] {
        for (int i = 1; i <= 10; ++i) {
            bq.push(i);
            std::cout << "[生产] " << i << " (队列大小: " << bq.size() << ")\n";
        }
        std::cout << "[生产] 完成\n";
    });

    // 消费者线程
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

    // 测试非阻塞出队
    int item;
    if (bq.tryPop(item)) {
        std::cout << "不应执行: 队列应为空\n";
    } else {
        std::cout << "非阻塞出队正确返回 false\n";
    }

    return 0;
}
