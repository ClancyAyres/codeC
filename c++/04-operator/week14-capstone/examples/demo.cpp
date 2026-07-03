#include <iostream>
#include <iomanip>
#include "tensor.hpp"
#include "operators.hpp"
#include "autograd.hpp"

using namespace micro;

void printTensor(const Tensor<float, 2>& t, const std::string& name) {
    std::cout << name << " (" << t.shape(0) << "x" << t.shape(1) << "):\n";
    std::cout << std::fixed << std::setprecision(4);
    for (int64_t i = 0; i < t.shape(0); ++i) {
        std::cout << "  [";
        for (int64_t j = 0; j < t.shape(1); ++j) {
            std::cout << std::setw(10) << t(i, j);
        }
        std::cout << " ]\n";
    }
    std::cout << '\n';
}

void printTensor(const Tensor<float, 1>& t, const std::string& name) {
    std::cout << name << " (" << t.shape(0) << "): [";
    std::cout << std::fixed << std::setprecision(6);
    for (int64_t i = 0; i < t.shape(0); ++i) {
        std::cout << t(i);
        if (i < t.shape(0) - 1) std::cout << ", ";
    }
    std::cout << "]\n\n";
}

int main() {
    std::cout << "===== Micro Tensor Library — Capstone Demo =====\n\n";

    // ── Part 1: Tensor Creation & Indexing ──────────────
    std::cout << "── Part 1: Tensor Creation & Indexing ──\n";
    {
        Tensor<float, 2> t({3, 4});
        for (int64_t i = 0; i < 3; ++i)
            for (int64_t j = 0; j < 4; ++j)
                t(i, j) = static_cast<float>(i * 10 + j);

        printTensor(t, "Original 3x4");

        auto slice = t.slice({0, 1}, {2, 2});
        printTensor(slice, "Slice rows[0:2] cols[1:3]");

        auto reshaped = t.reshape({4, 3});
        printTensor(reshaped, "Reshaped to 4x3");
    }

    // ── Part 2: Element-wise Operators ────────────────
    std::cout << "── Part 2: Element-wise Operators ──\n";
    {
        Tensor<float, 2> a({2, 3}, 2.0f);
        Tensor<float, 2> b({2, 3}, 3.0f);

        auto added = add(a, b);
        auto multiplied = mul(a, b);
        auto relued = relu(Tensor<float, 2>({1, 4},
            {1.0f, -2.0f, 3.0f, -4.0f}));

        printTensor(added, "2 + 3");
        printTensor(multiplied, "2 * 3");
        printTensor(relued, "ReLU([1, -2, 3, -4])");
    }

    // ── Part 3: Matmul ────────────────────────────────
    std::cout << "── Part 3: Matrix Multiplication ──\n";
    {
        Tensor<float, 2> a({2, 3});
        a(0, 0) = 1; a(0, 1) = 2; a(0, 2) = 3;
        a(1, 0) = 4; a(1, 1) = 5; a(1, 2) = 6;

        Tensor<float, 2> b({3, 2});
        b(0, 0) = 7;  b(0, 1) = 8;
        b(1, 0) = 9;  b(1, 1) = 10;
        b(2, 0) = 11; b(2, 1) = 12;

        printTensor(a, "A (2x3)");
        printTensor(b, "B (3x2)");

        auto c = matmul(a, b);
        printTensor(c, "A × B (2x2)");
    }

    // ── Part 4: Softmax & Reduce ──────────────────────
    std::cout << "── Part 4: Softmax, Sum, Mean, Max ──\n";
    {
        Tensor<float, 1> logits({4});
        logits(0) = 1.0f; logits(1) = 2.0f;
        logits(2) = 3.0f; logits(3) = 4.0f;

        printTensor(logits, "Logits");
        auto probs = softmax(logits);
        printTensor(probs, "Softmax");

        std::cout << "sum = " << sum(probs) << '\n';
        std::cout << "mean = " << mean(logits) << '\n';
        std::cout << "max = " << micro::max(logits) << "\n\n";
    }

    // ── Part 5: Autograd ──────────────────────────────
    std::cout << "── Part 5: Automatic Differentiation ──\n";
    {
        auto x = Var<float, 2>::make(Tensor<float, 2>({1, 4},
            {1.0f, 2.0f, 3.0f, 4.0f}), true);

        auto w = Var<float, 2>::make(Tensor<float, 2>({4, 2},
            {0.5f, 0.2f, 0.3f, 0.8f, 0.1f, 0.5f, 0.4f, 0.6f}), true);

        auto h = varMatmul(x, w);
        auto activated = varRelu(h);

        std::cout << "Forward pass: x(1x4) × w(4x2) → h → ReLU\n";
        printTensor(activated->data, "result = ReLU(x × w)");
        printTensor(h->data, "intermediate h = x × w");

        backwardGraph(activated);

        std::cout << "Gradient of x (dL/dx = dL/dh_reLU · dh_reLU/dh · dh/dx):\n";
        printTensor(x->grad, "dx (1x4)");

        std::cout << "Gradient of w (dL/dw = dL/dh_reLU · dh_reLU/dh · dh/dw):\n";
        printTensor(w->grad, "dw (4x2)");
    }

    // ── Part 6: Multi-hop backward graph ─────────────
    std::cout << "── Part 6: Multi-hop Backward Graph ──\n";
    {
        auto a = Var<float, 2>::make(Tensor<float, 2>({1, 1}, 2.0f), true);
        auto b = Var<float, 2>::make(Tensor<float, 2>({1, 1}, 3.0f), true);
        auto c = varAdd(a, b);
        auto d = varMul(c, a);

        std::cout << "Computation: a=2, b=3\n";
        std::cout << "  c = a + b = " << c->data(0, 0) << "\n";
        std::cout << "  d = c * a = " << d->data(0, 0) << "\n";

        backwardGraph(d);

        std::cout << "  a.grad = " << a->grad(0, 0) << "\n";
        std::cout << "  b.grad = " << b->grad(0, 0) << "\n";
        std::cout << "  (d/dx of c*a w.r.t a is c + a = 5 + 2 = 7)\n";
        std::cout << "  (d/dx of c*a w.r.t b is a = 2)\n\n";
    }

    std::cout << "===== Demo Complete =====\n";
    return 0;
}
