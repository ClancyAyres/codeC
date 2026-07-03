// Exercise 01: Vector Push-Back — 完整解法

#include <iostream>
#include <string>
#include <stdexcept>
#include <cassert>
#include <algorithm>

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
                new (&newData[i]) T(data_[i]);
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

    void push_back(const T& value) {
        if (size_ < capacity_) {
            // 有空闲容量：原地构造
            new (&data_[size_]) T(value);  // 可能抛异常
            ++size_;
            // 如果抛异常 → size_ 不变 → 强保证
            return;
        }

        // 需要扩容：强保证 = copy-and-swap
        size_t newCap = std::max(capacity_ * 2, size_t(4));
        T* newData = static_cast<T*>(::operator new(newCap * sizeof(T)));

        size_t i = 0;
        try {
            // 拷贝已有元素
            for (; i < size_; ++i)
                new (&newData[i]) T(data_[i]);
            // 构造新元素
            new (&newData[i]) T(value);
        } catch (...) {
            // 回滚：销毁新内存中已构造的，释放新内存，原 data_ 不变
            for (size_t j = 0; j < i; ++j)
                newData[j].~T();
            ::operator delete(newData);
            throw;
        }

        // 全部成功 → 替换
        for (size_t j = 0; j < size_; ++j)
            data_[j].~T();
        ::operator delete(data_);

        data_ = newData;
        size_ = size_ + 1;
        capacity_ = newCap;
    }
};

int main() {
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

    {
        SimpleVector<TestElem> v;
        v.push_back(TestElem(10));
        v.push_back(TestElem(20));

        size_t oldSize = v.size();
        int oldFirst = v[0].id;
        int oldSecond = v[1].id;

        std::cout << "\n[强保证] 尝试 push_back 会抛异常的元素\n";
        try {
            v.push_back(TestElem(30, true));
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
