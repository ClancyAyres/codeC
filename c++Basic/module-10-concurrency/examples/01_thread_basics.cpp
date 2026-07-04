// 模块 9 示例: std::thread
// 类似 Java Thread，但需要手动 join/detach

#include <iostream>
#include <thread>
#include <chrono>
#include <string>

void worker(int id, const std::string& name) {
    std::cout << "Thread " << id << " (" << name << ") started\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Thread " << id << " (" << name << ") done\n";
}

int main() {
    std::cout << "Main thread started\n";

    // 创建线程: 类似 Java new Thread(...).start()
    std::thread t1(worker, 1, "Alice");
    std::thread t2([]() {
        std::cout << "Lambda thread running\n";
    });

    // ✅ 必须 join 或 detach，否则析构时 std::terminate
    t1.join();   // 等待 t1 完成
    t2.join();   // 等待 t2 完成

    std::cout << "Main thread done\n";
    return 0;
}
