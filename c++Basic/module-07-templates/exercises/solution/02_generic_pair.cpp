#include <iostream>
#include <string>
#include <utility>

template<typename T1, typename T2>
class Pair {
public:
    Pair(const T1& f, const T2& s) : first_(f), second_(s) {}
    Pair(T1&& f, T2&& s) : first_(std::move(f)), second_(std::move(s)) {}

    const T1& first() const { return first_; }
    const T2& second() const { return second_; }

    bool operator==(const Pair& other) const {
        return first_ == other.first_ && second_ == other.second_;
    }

    bool operator!=(const Pair& other) const { return !(*this == other); }

private:
    T1 first_;
    T2 second_;

    template<typename U1, typename U2>
    friend std::ostream& operator<<(std::ostream& os, const Pair<U1, U2>& p);
};

template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const Pair<T1, T2>& p) {
    return os << "(" << p.first_ << ", " << p.second_ << ")";
}

template<typename T1, typename T2>
Pair<T1, T2> makePair(T1 first, T2 second) {
    return Pair<T1, T2>(std::move(first), std::move(second));
}

int main() {
    auto p1 = makePair(42, std::string("answer"));
    auto p2 = Pair<int, std::string>(42, "answer");
    auto p3 = Pair<int, std::string>(99, "different");

    std::cout << "p1: " << p1 << '\n';
    std::cout << "p2: " << p2 << '\n';

    std::cout << "p1 == p2: " << (p1 == p2) << '\n';
    std::cout << "p1 == p3: " << (p1 == p3) << '\n';

    std::cout << "p1.first: " << p1.first() << '\n';
    std::cout << "p1.second: " << p1.second() << '\n';

    return 0;
}
