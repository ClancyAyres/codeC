// Exercise 01: Vector Push-Back — 实现异常安全的 push_back
// TODO: 为 SimpleVector 实现提供强异常安全保证的 push_back

#include <iostream>
#include <string>
#include <stdexcept>
#include <cassert>

// 一个可抛异常的测试类型
struct TestElem {
    int id;
    bool throwOnCopy;

    explicit TestElem(int id, bool throwOnCopy = false)
        : id(id), throwOnCopy(throwOnCopy) {}

    TestElem(const TestElem& other)
        : id(other.id), throwOnCopy(other.throwOnCopy)
    {
        if (throwOnCopy)
            throw std::runtime_error("Copy failed for id=" + std::to_string(id));
    }

    ~TestElem() = default;
};

template<typename T>
class SimpleVector {
    T* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;

public:
    SimpleVector() = default;

    ~SimpleVector() {
        for (size_t i = 0; i < size_; ++i)
            data_[i].~T();
        ::operator delete(data_);
    }

    SimpleVector(const SimpleVector&) = delete;
    SimpleVector& operator=(const SimpleVector&) = delete;

    size_t size() const noexcept { return size_; }
    size_t capacity() const noexcept { return capacity_; }
    const T& operator[](size_t i) const { return data_[i]; }
    T& operator[](size_t i) { return data_[i]; }

    void reserve(size_t newCap) {
        if (newCap <= capacity_) return;
        T* newData = static_cast<T*>(::operator new(newCap * sizeof(T)));
        size_t i = 0;
        try {
            for (; i < size_; ++i)
                new (&newData[i]) T(data_[i]);  // placement new + 拷贝构造
        } catch (...) {
            for (size_t j = 0; j < i; ++j)
                newData[j].~T();
            ::operator delete(newData);
            throw;
        }
        for (size_t j = 0; j < size_; ++j)
            data_[j].~T();
        ::operator delete(data_);
        data_ = newData;
        capacity_ = newCap;
    }

    // TODO: 实现 push_back，提供强异常安全保证
    // 要求：
    // 1. 如果容量不够，先扩容（2倍策略，最少4）
    // 2. 扩容时构造元素可能抛异常
    // 3. 新增元素时拷贝构造可能抛异常
    // 4. 任何异常发生时，SimpleVector 状态必须与 push_back 前完全一致
    //    （强保证：要么成功，要么什么都不变）
    // 提示：
    // - 先在临时内存中完成所有可能抛异常的操作
    // - 成功了再替换 data_
    void push_back(const T& value) {
        // TODO: 实现强异常安全的 push_back
    }
};

int main() {
    // 测试1: 基本 push_back
    {
        SimpleVector<TestElem> v;
        v.push_back(TestElem(10));
        v.push_back(TestElem(20));
        std::cout << "[基本] size=" << v.size() << " cap=" << v.capacity() << '\n';
        assert(v.size() == 2);
        assert(v[0].id == 10);
        assert(v[1].id == 20);
        std::cout << "  通过\n";
    }

    // 测试2: 拷贝抛异常时状态不变（强保证）
    {
        SimpleVector<TestElem> v;
        v.push_back(TestElem(10));
        v.push_back(TestElem(20));

        size_t oldSize = v.size();
        int oldFirst = v[0].id;
        int oldSecond = v[1].id;

        std::cout << "\n[强保证] 尝试 push_back 会抛异常的元素\n";
        try {
            v.push_back(TestElem(30, true));  // 拷贝会抛异常
            std::cout << "  意外成功（未抛异常）\n";
        } catch (const std::exception& e) {
            std::cout << "  异常: " << e.what() << '\n';
            std::cout << "  size=" << v.size() << " (原=" << oldSize << ")\n";
            assert(v.size() == oldSize);
            assert(v[0].id == oldFirst);
            assert(v[1].id == oldSecond);
            std::cout << "  强保证通过: 状态未变\n";
        }
    }

    std::cout << "\n所有测试通过。\n";
    return 0;
}
