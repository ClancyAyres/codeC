#include <iostream>

void swapByValue(int a, int b) {
    std::cout << "  swapByValue: a=" << a << ", b=" << b;
    int temp = a;
    a = b;
    b = temp;
    std::cout << " -> a=" << a << ", b=" << b << '\n';
}

void swapByPointer(int* a, int* b) {
    std::cout << "  swapByPointer: *a=" << *a << ", *b=" << *b;
    int temp = *a;
    *a = *b;
    *b = temp;
    std::cout << " -> *a=" << *a << ", *b=" << *b << '\n';
}

void swapByReference(int& a, int& b) {
    std::cout << "  swapByReference: a=" << a << ", b=" << b;
    int temp = a;
    a = b;
    b = temp;
    std::cout << " -> a=" << a << ", b=" << b << '\n';
}

int main() {
    int x = 10, y = 20;

    std::cout << "Before any swap: x=" << x << ", y=" << y << "\n\n";

    std::cout << "--- swapByValue ---\n";
    swapByValue(x, y);
    std::cout << "After swapByValue: x=" << x << ", y=" << y
              << "  (unchanged — values were copied)\n\n";

    std::cout << "--- swapByPointer ---\n";
    swapByPointer(&x, &y);
    std::cout << "After swapByPointer: x=" << x << ", y=" << y
              << "  (swapped — passed addresses)\n\n";

    x = 10;
    y = 20;
    std::cout << "Reset: x=" << x << ", y=" << y << "\n\n";

    std::cout << "--- swapByReference ---\n";
    swapByReference(x, y);
    std::cout << "After swapByReference: x=" << x << ", y=" << y
              << "  (swapped — passed aliases)\n";

    return 0;
}
