#include <iostream>

struct Resource {
    int id;
    Resource(int i) : id(i) {
        std::cout << "Resource(" << id << ") acquired\n";
    }
    ~Resource() {
        std::cout << "Resource(" << id << ") released\n";
    }
};

Resource* createResource(int id) {
    if (id < 0) {
        return nullptr;
    }
    return new Resource(id);
}

void processData(int size) {
    int* data = new int[size];
    Resource* r = new Resource(99);

    bool shouldAbort = (size > 1000);
    if (shouldAbort) {
        std::cout << "Size too large, aborting\n";
    }

    delete[] data;
    delete r;

    if (shouldAbort) {
        return;
    }
}

int main() {
    Resource* r = createResource(-5);
    if (r) {
        delete r;
    }

    processData(2000);

    return 0;
}
