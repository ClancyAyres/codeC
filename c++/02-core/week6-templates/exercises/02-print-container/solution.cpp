#include <iostream>
#include <vector>
#include <list>
#include <string>

template<typename Container>
void printContainer(const Container& c, const char* separator = " ") {
    bool first = true;
    for (auto it = std::begin(c); it != std::end(c); ++it) {
        if (!first) {
            std::cout << separator;
        }
        std::cout << *it;
        first = false;
    }
}

int main() {
    std::cout << "=== vector<int> ===\n";
    std::vector<int> v = {1, 2, 3, 4, 5};
    printContainer(v);
    std::cout << '\n';
    printContainer(v, ", ");
    std::cout << '\n';

    std::cout << "\n=== vector<string> ===\n";
    std::vector<std::string> sv = {"hello", "world", "C++", "templates"};
    printContainer(sv);
    std::cout << '\n';

    std::cout << "\n=== list<double> ===\n";
    std::list<double> l = {3.14, 2.72, 1.41};
    printContainer(l);
    std::cout << '\n';

    std::cout << "\n=== Custom Array ===\n";
    int arr[] = {10, 20, 30, 40};
    printContainer(arr);
    std::cout << '\n';

    return 0;
}
