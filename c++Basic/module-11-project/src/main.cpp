// 模块 10 综合项目: 终端任务管理器
// 串联课程全部知识点: 引用/指针、智能指针、RAII、OOP、STL、模板、lambda、异常处理
//
// 支持命令: add <title>, list, done <id>, undo, remove <id>, save, load, help, quit

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <memory>
#include <optional>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <cctype>
#include <future>

// ============================================================================
// 数据模型: Task — 值语义、const 正确性、运算符重载
// 模块 3(OOP) + 模块 4(值语义)
// ============================================================================

class Task {
public:
    Task(int id, std::string title)
        : id_(id), title_(std::move(title)), done_(false) {}

    // 只读访问 — Core Guideline Con.2
    int id() const { return id_; }
    const std::string& title() const { return title_; }
    bool isDone() const { return done_; }

    void markDone() { done_ = true; }
    void markUndone() { done_ = false; }
    void setTitle(const std::string& t) { title_ = t; }

private:
    int id_;
    std::string title_;
    bool done_;
};

// 输出运算符 — 模块 4
std::ostream& operator<<(std::ostream& os, const Task& task) {
    os << "  [" << (task.isDone() ? "x" : " ") << "] "
       << "#" << task.id() << " " << task.title();
    return os;
}

// ✅ 模板函数: 通用查找 — 模块 6 模板
template<typename Container, typename Predicate>
auto findIf(Container& container, Predicate pred) -> decltype(container.begin()->get()) {
    auto it = std::find_if(container.begin(), container.end(), pred);
    if (it != container.end()) return it->get();
    return nullptr;
}

// ============================================================================
// TaskManager — 模块 2(智能指针+RAII) + 模块 5(STL) + 模块 7(optional)
// ============================================================================

class TaskManager {
public:
    TaskManager() = default;

    // ✅ 添加任务
    Task& addTask(std::string title) {
        auto task = std::make_unique<Task>(nextId_++, std::move(title));
        Task& ref = *task;
        tasks_.push_back(std::move(task));
        return ref;
    }

    // ✅ 完成任务 — 模块 7 optional
    bool markDone(int id) {
        auto* task = findTask(id);
        if (!task) return false;
        task->markDone();
        return true;
    }

    bool markUndone(int id) {
        auto* task = findTask(id);
        if (!task) return false;
        task->markUndone();
        return true;
    }

    // ✅ 删除任务
    bool removeTask(int id) {
        auto it = std::find_if(tasks_.begin(), tasks_.end(),
            [id](const auto& t) { return t->id() == id; });
        if (it == tasks_.end()) return false;
        tasks_.erase(it);
        return true;
    }

    // ✅ 查找任务 — 返回指针，允许修改
    Task* findTask(int id) {
        return findIf(tasks_, [id](const auto& t) { return t->id() == id; });
    }

    const std::vector<std::unique_ptr<Task>>& tasks() const { return tasks_; }

    // ✅ RAII 文件保存 — 模块 2 + 模块 8
    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file for writing: " + filename);
        }
        for (const auto& task : tasks_) {
            file << task->id() << '|'
                 << (task->isDone() ? '1' : '0') << '|'
                 << task->title() << '\n';
        }
    }
    // RAII: file 析构自动 close

    // ✅ 异步保存 — 模块 9 并发 (std::async)
    void saveToFileAsync(const std::string& filename) const {
        auto future = std::async(std::launch::async, [this, &filename]() {
            saveToFile(filename);
        });
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file for reading: " + filename);
        }

        tasks_.clear();
        int maxId = 0;

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            // 解析: id|done|title
            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);
            if (pos1 == std::string::npos || pos2 == std::string::npos) continue;

            int id = std::stoi(line.substr(0, pos1));
            bool done = line[pos1 + 1] == '1';
            std::string title = line.substr(pos2 + 1);

            auto task = std::make_unique<Task>(id, std::move(title));
            if (done) task->markDone();
            tasks_.push_back(std::move(task));

            if (id > maxId) maxId = id;
        }

        nextId_ = maxId + 1;
    }

private:
    std::vector<std::unique_ptr<Task>> tasks_;
    int nextId_ = 1;
};

// ============================================================================
// 命令系统 — 模块 3(抽象+多态) + 模块 6(模板) + 模块 7(lambda)
// ============================================================================

class Command {
public:
    virtual ~Command() = default;
    virtual std::string name() const = 0;
    virtual std::string help() const = 0;
    virtual void execute(TaskManager& manager, const std::vector<std::string>& args) = 0;
};

class AddCommand : public Command {
public:
    std::string name() const override { return "add"; }
    std::string help() const override { return "add <title> — Add a new task"; }

    void execute(TaskManager& manager, const std::vector<std::string>& args) override {
        if (args.empty()) {
            std::cout << "Usage: add <title>\n";
            return;
        }
        std::string title;
        for (size_t i = 0; i < args.size(); ++i) {
            if (i > 0) title += ' ';
            title += args[i];
        }
        auto& task = manager.addTask(std::move(title));
        std::cout << "Added: " << task << '\n';
    }
};

class ListCommand : public Command {
public:
    std::string name() const override { return "list"; }
    std::string help() const override { return "list — Show all tasks"; }

    void execute(TaskManager& manager, const std::vector<std::string>& args) override {
        bool showAll = true;
        bool showDone = false;
        bool showPending = false;

        // 模块 7: lambda 过滤
        for (const auto& arg : args) {
            if (arg == "--done") { showAll = false; showDone = true; }
            if (arg == "--pending") { showAll = false; showPending = true; }
        }

        const auto& tasks = manager.tasks();
        if (tasks.empty()) {
            std::cout << "No tasks. Use 'add' to create one.\n";
            return;
        }

        // ✅ range-for + const auto& — 模块 1
        for (const auto& task : tasks) {
            if (showAll || (showDone && task->isDone()) || (showPending && !task->isDone())) {
                std::cout << *task << '\n';
            }
        }
    }
};

class DoneCommand : public Command {
public:
    std::string name() const override { return "done"; }
    std::string help() const override { return "done <id> — Mark task as done"; }

    void execute(TaskManager& manager, const std::vector<std::string>& args) override {
        if (args.empty()) { std::cout << "Usage: done <id>\n"; return; }
        int id = std::stoi(args[0]);
        if (manager.markDone(id)) {
            std::cout << "Task #" << id << " marked as done.\n";
        } else {
            std::cout << "Task #" << id << " not found.\n";
        }
    }
};

class UndoCommand : public Command {
public:
    std::string name() const override { return "undo"; }
    std::string help() const override { return "undo <id> — Mark task as not done"; }

    void execute(TaskManager& manager, const std::vector<std::string>& args) override {
        if (args.empty()) { std::cout << "Usage: undo <id>\n"; return; }
        int id = std::stoi(args[0]);
        if (manager.markUndone(id)) {
            std::cout << "Task #" << id << " marked as undone.\n";
        } else {
            std::cout << "Task #" << id << " not found.\n";
        }
    }
};

class RemoveCommand : public Command {
public:
    std::string name() const override { return "remove"; }
    std::string help() const override { return "remove <id> — Delete a task"; }

    void execute(TaskManager& manager, const std::vector<std::string>& args) override {
        if (args.empty()) { std::cout << "Usage: remove <id>\n"; return; }
        int id = std::stoi(args[0]);
        if (manager.removeTask(id)) {
            std::cout << "Task #" << id << " removed.\n";
        } else {
            std::cout << "Task #" << id << " not found.\n";
        }
    }
};

class SaveCommand : public Command {
public:
    std::string name() const override { return "save"; }
    std::string help() const override { return "save [filename] — Save tasks to file"; }

    void execute(TaskManager& manager, const std::vector<std::string>& args) override {
        bool async = false;
        std::string filename;
        for (const auto& arg : args) {
            if (arg == "--async") async = true;
            else filename = arg;
        }
        if (filename.empty()) filename = "tasks.txt";
        try {
            if (async) {
                std::cout << "Saving asynchronously...\n";
                manager.saveToFileAsync(filename);
            } else {
                manager.saveToFile(filename);
            }
            std::cout << "Tasks saved to " << filename << '\n';
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << '\n';
        }
    }
};

class LoadCommand : public Command {
public:
    std::string name() const override { return "load"; }
    std::string help() const override { return "load [filename] — Load tasks from file"; }

    void execute(TaskManager& manager, const std::vector<std::string>& args) override {
        std::string filename = args.empty() ? "tasks.txt" : args[0];
        try {
            manager.loadFromFile(filename);
            std::cout << "Tasks loaded from " << filename << '\n';
            std::cout << "Loaded " << manager.tasks().size() << " tasks.\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << '\n';
        }
    }
};

class HelpCommand : public Command {
public:
    explicit HelpCommand(const std::map<std::string, Command*>& commands)
        : commands_(&commands) {}

    std::string name() const override { return "help"; }
    std::string help() const override { return "help — Show this help"; }

    void execute(TaskManager&, const std::vector<std::string>&) override {
        std::cout << "Commands:\n";
        for (const auto& [name, cmd] : *commands_) {
            std::cout << "  " << cmd->help() << '\n';
        }
    }

private:
    const std::map<std::string, Command*>* commands_;
};

// ============================================================================
// 命令解析 — 模块 5(string) + 模块 7(string_view)
// ============================================================================

// 拆分输入为命令名和参数
std::optional<std::pair<std::string, std::vector<std::string>>> parseInput(std::string_view input) {
    // 去除首尾空格
    while (!input.empty() && std::isspace(input.front())) input.remove_prefix(1);
    while (!input.empty() && std::isspace(input.back())) input.remove_suffix(1);

    if (input.empty()) return std::nullopt;

    std::stringstream ss{std::string(input)};
    std::string command;
    ss >> command;

    std::vector<std::string> args;
    std::string arg;
    while (ss >> arg) {
        args.push_back(arg);
    }

    return std::make_pair(command, args);
}

// ============================================================================
// main — 模块 8(异常处理)
// ============================================================================

int main() {
    TaskManager manager;

    // 注册命令 — 多态 Command 模式
    AddCommand addCmd;
    ListCommand listCmd;
    DoneCommand doneCmd;
    UndoCommand undoCmd;
    RemoveCommand removeCmd;
    SaveCommand saveCmd;
    LoadCommand loadCmd;

    std::map<std::string, Command*> commands;
    commands["add"] = &addCmd;
    commands["list"] = &listCmd;
    commands["done"] = &doneCmd;
    commands["undo"] = &undoCmd;
    commands["remove"] = &removeCmd;
    commands["save"] = &saveCmd;
    commands["load"] = &loadCmd;

    HelpCommand helpCmd(commands);
    commands["help"] = &helpCmd;

    std::cout << "=== Task Manager ===\n";
    std::cout << "Type 'help' for commands, 'quit' to exit.\n\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;  // EOF

        auto result = parseInput(line);
        if (!result) continue;
        auto& [cmdName, args] = *result;
        if (cmdName == "quit" || cmdName == "exit") break;

        auto it = commands.find(cmdName);
        if (it != commands.end()) {
            try {
                it->second->execute(manager, args);
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << '\n';
            }
        } else {
            std::cout << "Unknown command: " << cmdName << "\nType 'help' for available commands.\n";
        }
    }

    std::cout << "Goodbye!\n";
    return 0;
}
