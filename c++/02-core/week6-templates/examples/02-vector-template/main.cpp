// Example 02: Vector Template — 手写 Vector<T> 类模板
// 演示：类模板定义、成员函数类外定义、动态扩容

#include <iostream>
#include <utility>

template<typename T>
class Vector {
    T* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;

    void reallocate(size_t newCapacity) {
        T* newData = new T[newCapacity];
        for (size_t i = 0; i < size_; ++i) {
            newData[i] = std::move(data_[i]);
        }
        delete[] data_;
        data_ = newData;
        capacity_ = newCapacity;
    }

public:
    Vector() = default;

    ~Vector() {
        delete[] data_;
    }

    Vector(const Vector& other)
        : size_(other.size_), capacity_(other.capacity_) {
        data_ = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }

    Vector(Vector&& other) noexcept
        : data_(std::exchange(other.data_, nullptr))
        , size_(std::exchange(other.size_, 0))
        , capacity_(std::exchange(other.capacity_, 0)) {}

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = std::exchange(other.data_, nullptr);
            size_ = std::exchange(other.size_, 0);
            capacity_ = std::exchange(other.capacity_, 0);
        }
        return *this;
    }

    void push_back(const T& value) {
        if (size_ >= capacity_) {
            reallocate(capacity_ == 0 ? 4 : capacity_ * 2);
        }
        data_[size_++] = value;
    }

    void push_back(T&& value) {
        if (size_ >= capacity_) {
            reallocate(capacity_ == 0 ? 4 : capacity_ * 2);
        }
        data_[size_++] = std::move(value);
    }

    T& operator[](size_t index) { return data_[index]; }
    const T& operator[](size_t index) const { return data_[index]; }

    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }
};

int main() {
    std::cout << "=== Vector<int> ===\n";
    Vector<int> vi;
    std::cout << "empty: " << (vi.empty() ? "true" : "false")
              << ", size=" << vi.size() << ", capacity=" << vi.capacity() << '\n';

    vi.push_back(10);
    vi.push_back(20);
    vi.push_back(30);
    std::cout << "After 3 push_backs: size=" << vi.size()
              << ", capacity=" << vi.capacity() << '\n';
    std::cout << "Elements: ";
    for (size_t i = 0; i < vi.size(); ++i) {
        std::cout << vi[i] << ' ';
    }
    std::cout << '\n';

    vi.push_back(40);
    vi.push_back(50);
    std::cout << "After 5 push_backs: size=" << vi.size()
              << ", capacity=" << vi.capacity() << '\n';
    std::cout << "Elements: ";
    for (size_t i = 0; i < vi.size(); ++i) {
        std::cout << vi[i] << ' ';
    }
    std::cout << '\n';

    std::cout << "\n=== Vector<double> ===\n";
    Vector<double> vd;
    vd.push_back(3.14);
    vd.push_back(2.72);
    vd.push_back(1.41);
    std::cout << "Elements: ";
    for (size_t i = 0; i < vd.size(); ++i) {
        std::cout << vd[i] << ' ';
    }
    std::cout << '\n';

    std::cout << "\n=== Copy & Move ===\n";
    Vector<int> vi2 = vi;         // 拷贝构造
    std::cout << "Copied vector size=" << vi2.size() << '\n';

    Vector<int> vi3 = std::move(vi);  // 移动构造
    std::cout << "Moved-from vector size=" << vi.size()
              << ", moved-to size=" << vi3.size() << '\n';

    return 0;
}
