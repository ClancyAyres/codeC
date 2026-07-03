#include <gtest/gtest.h>
#include <cmath>
#include "tensor.hpp"
#include "operators.hpp"
#include "autograd.hpp"

using namespace micro;

TEST(AutogradTest, VarCreateAndZeroGrad) {
    auto v = Var<float, 2>::make(Tensor<float, 2>({2, 3}, 1.0f), true);
    EXPECT_TRUE(v->requires_grad);
    EXPECT_EQ(v->data.size(), 6);
    EXPECT_FLOAT_EQ(v->data(0, 0), 1.0f);
    EXPECT_FLOAT_EQ(v->grad(0, 0), 0.0f);

    v->grad(0, 0) = 5.0f;
    v->zeroGrad();
    EXPECT_FLOAT_EQ(v->grad(0, 0), 0.0f);
}

TEST(AutogradTest, AddBackward) {
    auto a = Var<float, 2>::make(Tensor<float, 2>({2, 2}, 2.0f), true);
    auto b = Var<float, 2>::make(Tensor<float, 2>({2, 2}, 3.0f), true);
    auto c = varAdd(a, b);

    c->backward();

    for (int64_t i = 0; i < 2; ++i) {
        for (int64_t j = 0; j < 2; ++j) {
            EXPECT_FLOAT_EQ(a->grad(i, j), 1.0f);
            EXPECT_FLOAT_EQ(b->grad(i, j), 1.0f);
        }
    }
}

TEST(AutogradTest, MulBackward) {
    auto a = Var<float, 2>::make(Tensor<float, 2>({2, 2}, 2.0f), true);
    auto b = Var<float, 2>::make(Tensor<float, 2>({2, 2}, 3.0f), true);
    auto c = varMul(a, b);

    c->backward();

    for (int64_t i = 0; i < 2; ++i) {
        for (int64_t j = 0; j < 2; ++j) {
            EXPECT_FLOAT_EQ(a->grad(i, j), 3.0f);
            EXPECT_FLOAT_EQ(b->grad(i, j), 2.0f);
        }
    }
}

TEST(AutogradTest, ReluBackward) {
    Tensor<float, 2> data({2, 2});
    data(0, 0) = -1.0f; data(0, 1) = 2.0f;
    data(1, 0) = 0.0f; data(1, 1) = 3.0f;

    auto a = Var<float, 2>::make(data, true);
    auto r = varRelu(a);

    r->backward();

    EXPECT_FLOAT_EQ(a->grad(0, 0), 0.0f);
    EXPECT_FLOAT_EQ(a->grad(0, 1), 1.0f);
    EXPECT_FLOAT_EQ(a->grad(1, 0), 0.0f);
    EXPECT_FLOAT_EQ(a->grad(1, 1), 1.0f);
}

TEST(AutogradTest, MatmulBackward) {
    auto a = Var<float, 2>::make(Tensor<float, 2>({2, 3}, 1.0f), true);
    auto b = Var<float, 2>::make(Tensor<float, 2>({3, 2}, 2.0f), true);
    auto c = varMatmul(a, b);

    c->backward();

    EXPECT_EQ(a->grad.shape(0), 2);
    EXPECT_EQ(a->grad.shape(1), 3);
    EXPECT_EQ(b->grad.shape(0), 3);
    EXPECT_EQ(b->grad.shape(1), 2);
}

TEST(AutogradTest, NoGradWhenNotRequired) {
    auto a = Var<float, 2>::make(Tensor<float, 2>({2, 2}, 2.0f), false);
    auto b = Var<float, 2>::make(Tensor<float, 2>({2, 2}, 3.0f), false);
    auto c = varAdd(a, b);

    c->backward();

    for (int64_t i = 0; i < 2; ++i)
        for (int64_t j = 0; j < 2; ++j)
            EXPECT_FLOAT_EQ(a->grad(i, j), 1.0f);
}

TEST(AutogradTest, BackwardGraph) {
    auto a = Var<float, 2>::make(Tensor<float, 2>({1, 1}, 2.0f), true);
    auto b = Var<float, 2>::make(Tensor<float, 2>({1, 1}, 3.0f), true);
    auto c = varAdd(a, b);
    auto d = varMul(c, a);
    auto e = varRelu(d);

    backwardGraph(e);

    EXPECT_GT(a->grad(0, 0), 0.0f);
    EXPECT_GT(b->grad(0, 0), 0.0f);
}

TEST(AutogradTest, OneDimBackward) {
    auto a = Var<float, 1>::make(Tensor<float, 1>({3}, 2.0f), true);
    auto b = Var<float, 1>::make(Tensor<float, 1>({3}, 3.0f), true);
    auto c = varAdd(a, b);

    c->backward();

    for (int64_t i = 0; i < 3; ++i) {
        EXPECT_FLOAT_EQ(a->grad(i), 1.0f);
        EXPECT_FLOAT_EQ(b->grad(i), 1.0f);
    }
}
