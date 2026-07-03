// 模块 2 习题: 修复内存泄漏和悬垂指针
// 下面的代码有多个问题，请修复它们并遵照编码规范

#include <iostream>
#include <string>
#include <cstring>

// ❌ 问题1: 使用裸 new，没有配对的 delete

struct DataRecord {
    int id;
    std::string name;
    double* scores;
    int scoreCount;

    DataRecord(int i, const std::string& n, int count)
        : id(i), name(n), scoreCount(count) {
        scores = new double[count];
    }

    ~DataRecord() {
        delete[] scores;
    }
};

// ❌ 问题2: 返回裸指针，调用者可能忘记 delete
DataRecord* createRecord(int id, const std::string& name) {
    return new DataRecord(id, name, 5);
}

// ❌ 问题3: 在异常路径可能泄漏
void processData(const std::string& name) {
    DataRecord* record = new DataRecord(1, name, 10);
    delete record;
}

// ❌ 问题4: 返回局部变量指针 — 悬垂指针!
int* getCachedValue() {
    int value = 42;
    return &value;
}

int main() {
    auto* r = createRecord(1, "test");
    std::cout << "Record: " << r->name << '\n';
    delete r;

    processData("example");

    int* p = getCachedValue();
    // *p 现在是悬垂指针 — 未定义行为!

    return 0;
}
