// 模块 1 示例: const 正确性
// 相关规范: Core Guidelines Con.1, Con.2, Con.3; Google Style: Use of const
//
// Java 的 final 只能阻止重新赋值，C++ 的 const 能阻止任何修改

#include <iostream>
#include <string>

class Document {
public:
    explicit Document(std::string title) : title_(std::move(title)) {}
    const std::string& title() const { return title_; }
    void setTitle(const std::string& t) { title_ = t; }
private:
    std::string title_;
};

void showDocument(const Document& doc) {
    std::cout << "Document: " << doc.title() << '\n';
}

int main() {
    const int value = 42;

    const int* p1 = &value;
    p1 = nullptr;

    int x = 5;
    int* const p2 = &x;
    *p2 = 10;

    const Document doc("readme.txt");
    showDocument(doc);

    Document editable("draft.txt");
    editable.setTitle("final.txt");

    return 0;
}
