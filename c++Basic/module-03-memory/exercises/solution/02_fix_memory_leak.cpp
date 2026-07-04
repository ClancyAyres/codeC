// 模块 2 习题答案: 修复内存泄漏
// 规范: Core Guidelines R.20, R.11, F.43

#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct DataRecord {
    int id;
    std::string name;
    std::vector<double> scores;

    DataRecord(int i, std::string n, int count)
        : id(i), name(std::move(n)), scores(count, 0.0) {
    }
};

std::unique_ptr<DataRecord> createRecord(int id, const std::string& name) {
    return std::make_unique<DataRecord>(id, name, 5);
}

void processData(const std::string& name) {
    auto record = std::make_unique<DataRecord>(1, name, 10);
}

int getCachedValue() {
    return 42;
}

int main() {
    auto r = createRecord(1, "test");
    std::cout << "Record: " << r->name << '\n';

    processData("example");

    int value = getCachedValue();
    std::cout << "Cached value: " << value << '\n';

    return 0;
}
