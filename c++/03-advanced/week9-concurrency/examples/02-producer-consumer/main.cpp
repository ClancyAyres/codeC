// Example 02: Producer-Consumer — mutex + condition_variable
// 演示：一个生产者线程、多个消费者线程协作

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <vector>
#include <sstream>

// ── 共享数据 ──────────────────────────────────────────────────────

std::mutex mtx;
std::condition_variable cv;
std::queue<int> queue;
bool production_done = false;
constexpr int TOTAL_ITEMS = 20;

// ── 生产者 ────────────────────────────────────────────────────────

void producer() {
    for (int i = 1; i <= TOTAL_ITEMS; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        {
            std::lock_guard<std::mutex> lock(mtx);
            queue.push(i);
            std::cout << "[生产者] 生产: " << i << " (队列大小: "
                      << queue.size() << ")\n";
        }
        cv.notify_one();  // 通知一个消费者
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        production_done = true;
    }
    cv.notify_all();  // 通知所有消费者结束
    std::cout << "[生产者] 生产完毕，已通知所有消费者\n";
}

// ── 消费者 ────────────────────────────────────────────────────────

void consumer(int id) {
    int consumed = 0;
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);

        // 使用 predicate 版本，自动处理虚假唤醒
        cv.wait(lock, [] { return !queue.empty() || production_done; });

        if (queue.empty() && production_done) {
            break;  // 队列空且生产完毕，退出
        }

        if (!queue.empty()) {
            int value = queue.front();
            queue.pop();
            ++consumed;
            std::cout << "  [消费者 " << id << "] 消费: " << value << '\n';
        }

        // unlock 后让出 CPU，让其他消费者有机会获取锁
    }
    std::cout << "  [消费者 " << id << "] 退出，共消费 " << consumed << " 项\n";
}

// ── main ──────────────────────────────────────────────────────────

int main() {
    std::cout << "=== 生产者-消费者模型 ===\n";
    std::cout << "总生产: " << TOTAL_ITEMS << " 项\n";
    std::cout << "消费者: 3 个线程\n\n";

    std::thread prod(producer);
    std::vector<std::thread> consumers;
    for (int i = 1; i <= 3; ++i) {
        consumers.emplace_back(consumer, i);
    }

    prod.join();
    for (auto& t : consumers) t.join();

    std::cout << "\n全部线程结束，队列剩余: " << queue.size() << '\n';
    return 0;
}
