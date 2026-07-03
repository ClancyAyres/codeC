#include <iostream>
#include <utility>

class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) noexcept = default;
    NonCopyable& operator=(NonCopyable&&) noexcept = default;
protected:
    ~NonCopyable() = default;
};

class UniqueResource : public NonCopyable {
    int* data_;

public:
    explicit UniqueResource(int value) : data_(new int(value)) {
        std::cout << "Creating r with value " << value << '\n';
    }

    ~UniqueResource() {
        if (data_) {
            std::cout << "Destroying resource " << *data_ << '\n';
            delete data_;
        }
    }

    UniqueResource(UniqueResource&& other) noexcept
        : data_(std::exchange(other.data_, nullptr)) {
    }

    UniqueResource& operator=(UniqueResource&& other) noexcept {
        if (this == &other) return *this;
        if (data_) {
            std::cout << "Destroying resource " << *data_ << '\n';
            delete data_;
        }
        data_ = std::exchange(other.data_, nullptr);
        return *this;
    }

    int value() const {
        return data_ ? *data_ : 0;
    }

    void setValue(int v) {
        if (data_) *data_ = v;
    }
};

int main() {
    UniqueResource r1(42);
    std::cout << "  r1.value() = " << r1.value() << '\n';

    std::cout << "Moving r1 to r2\n";
    UniqueResource r2 = std::move(r1);
    std::cout << "  r1 still valid? "
              << (r1.value() > 0 ? "yes" : "no (nullptr)") << '\n';
    std::cout << "  r2.value() = " << r2.value() << '\n';

    std::cout << "r2.setValue(100)\n";
    r2.setValue(100);
    std::cout << "  r2.value() = " << r2.value() << '\n';

    return 0;
}
