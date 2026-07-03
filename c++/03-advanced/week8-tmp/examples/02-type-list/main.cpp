// Example 02: Type List — 编译时递归类型列表
// 演示：模板递归、类型操作（Length, TypeAt, Append）、using 别名

#include <iostream>
#include <string>
#include <typeinfo>

struct NullType {};

template<typename H, typename T = NullType>
struct TypeList {
    using Head = H;
    using Tail = T;
};

// Length
template<typename List>
struct Length;
template<>
struct Length<NullType> {
    static constexpr size_t value = 0;
};
template<typename H, typename T>
struct Length<TypeList<H, T>> {
    static constexpr size_t value = 1 + Length<T>::value;
};

// TypeAt
template<typename List, size_t N>
struct TypeAt {
    using type = typename TypeAt<typename List::Tail, N - 1>::type;
};
template<typename List>
struct TypeAt<List, 0> {
    using type = typename List::Head;
};

// Append (add type at end)
template<typename List, typename New>
struct Append;
template<typename New>
struct Append<NullType, New> {
    using type = TypeList<New>;
};
template<typename H, typename T, typename New>
struct Append<TypeList<H, T>, New> {
    using type = TypeList<H, typename Append<T, New>::type>;
};

// Contains — check if a type is in the list
template<typename List, typename Query>
struct Contains : std::false_type {};
template<typename H, typename T, typename Query>
struct Contains<TypeList<H, T>, Query>
    : std::conditional_t<std::is_same_v<H, Query>, std::true_type, Contains<T, Query>> {};

// printer for demo
template<typename List>
struct PrintTypes {
    static void print() {
        std::cout << typeid(typename List::Head).name() << ' ';
        PrintTypes<typename List::Tail>::print();
    }
};
template<>
struct PrintTypes<NullType> {
    static void print() { std::cout << '\n'; }
};

// TypeAt helper
template<typename List, size_t N>
using TypeAt_t = typename TypeAt<List, N>::type;

// Append helper
template<typename List, typename New>
using Append_t = typename Append<List, New>::type;

int main() {
    using MyList = TypeList<int, TypeList<double, TypeList<std::string, TypeList<char>>>>;

    std::cout << "=== Type List ===\n";
    std::cout << "Types: ";
    PrintTypes<MyList>::print();

    std::cout << "\n=== Length ===\n";
    constexpr size_t len = Length<MyList>::value;
    std::cout << "Length = " << len << '\n';

    std::cout << "\n=== TypeAt ===\n";
    static_assert(std::is_same_v<TypeAt_t<MyList, 0>, int>);
    static_assert(std::is_same_v<TypeAt_t<MyList, 1>, double>);
    static_assert(std::is_same_v<TypeAt_t<MyList, 2>, std::string>);
    static_assert(std::is_same_v<TypeAt_t<MyList, 3>, char>);
    std::cout << "TypeAt<0> = int\n";
    std::cout << "TypeAt<1> = double\n";
    std::cout << "TypeAt<2> = std::string\n";
    std::cout << "TypeAt<3> = char\n";

    std::cout << "\n=== Append ===\n";
    using Longer = Append_t<MyList, long>;
    std::cout << "After Append<long>: Length = " << Length<Longer>::value << '\n';
    std::cout << "Types: ";
    PrintTypes<Longer>::print();

    std::cout << "\n=== Contains ===\n";
    std::cout << "Contains<int>: " << Contains<MyList, int>::value << '\n';
    std::cout << "Contains<double>: " << Contains<MyList, double>::value << '\n';
    std::cout << "Contains<float>: " << Contains<MyList, float>::value << '\n';

    return 0;
}
