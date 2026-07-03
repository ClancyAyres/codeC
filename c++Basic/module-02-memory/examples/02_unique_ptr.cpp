// 模块 2 示例: unique_ptr 独占所有权
// 相关规范: Core Guidelines R.20, F.26

#include <iostream>
#include <memory>
#include <vector>
#include <string>

struct Task {
    int id;
    std::string title;
    Task(int i, std::string t) : id(i), title(std::move(t)) {}
};

// ✅ 正确: 工厂函数返回 unique_ptr，自动转移所有权 — Core Guidelines F.26
std::unique_ptr<Task> createTask(int id, const std::string& title) {
    return std::make_unique<Task>(id, title);
}

void processTask(std::unique_ptr<Task> task) {
    std::cout << "Processing #" << task->id << ": " << task->title << '\n';
}

int main() {
    // ✅ 正确: make_unique 消除 new/delete，异常安全 — Core Guidelines R.22
    auto task1 = std::make_unique<Task>(1, "Fix login bug");
    std::cout << "Task: " << task1->id << " - " << task1->title << '\n';

    // ✅ 正确: std::move 显式转移所有权，task1 变为空 — Core Guidelines R.32
    // ❌ 错误: unique_ptr 不可拷贝 — Core Guidelines R.20
    // auto task1Copy = task1;  // 编译错误: unique_ptr 拷贝构造函数 =delete
    auto task2 = std::move(task1);
    if (!task1) {
        std::cout << "task1 is now null after move\n";
    }

    // ✅ 正确: 工厂创建 + std::move 传递所有权 — Core Guidelines F.26, R.32
    auto task3 = createTask(3, "Add unit tests");
    processTask(std::move(task3));

    // ✅ 正确: unique_ptr 在容器中安全持有资源，容器销毁时自动释放 — Core Guidelines R.20
    std::vector<std::unique_ptr<Task>> tasks;
    tasks.push_back(std::make_unique<Task>(10, "Refactor"));
    tasks.push_back(std::make_unique<Task>(11, "Deploy"));

    for (const auto& t : tasks) {
        std::cout << "  #" << t->id << ": " << t->title << '\n';
    }

    return 0;
}
