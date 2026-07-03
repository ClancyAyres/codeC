#include <gtest/gtest.h>
#include <cmath>
#include "tensor.hpp"
#include "operators.hpp"

using namespace micro;

TEST(OperatorsTest, Add) {
    Tensor<float, 2> a({2, 3}, 1.0f);
    Tensor<float, 2> b({2, 3}, 2.0f);
    auto c = add(a, b);
    EXPECT_EQ(c.shape(), a.shape());
    for (int64_t i = 0; i < 2; ++i)
        for (int64_t j = 0; j < 3; ++j)
            EXPECT_FLOAT_EQ(c(i, j), 3.0f);
}

TEST(OperatorsTest, Sub) {
    Tensor<float, 2> a({2, 3}, 5.0f);
    Tensor<float, 2> b({2, 3}, 2.0f);
    auto c = sub(a, b);
    for (int64_t i = 0; i < 2; ++i)
        for (int64_t j = 0; j < 3; ++j)
            EXPECT_FLOAT_EQ(c(i, j), 3.0f);
}

TEST(OperatorsTest, Mul) {
    Tensor<float, 2> a({2, 3}, 3.0f);
    Tensor<float, 2> b({2, 3}, 4.0f);
    auto c = mul(a, b);
    for (int64_t i = 0; i < 2; ++i)
        for (int64_t j = 0; j < 3; ++j)
            EXPECT_FLOAT_EQ(c(i, j), 12.0f);
}

TEST(OperatorsTest, Div) {
    Tensor<float, 2> a({2, 3}, 12.0f);
    Tensor<float, 2> b({2, 3}, 4.0f);
    auto c = div(a, b);
    for (int64_t i = 0; i < 2; ++i)
        for (int64_t j = 0; j < 3; ++j)
            EXPECT_FLOAT_EQ(c(i, j), 3.0f);
}

TEST(OperatorsTest, Relu) {
    Tensor<float, 2> t({2, 3});
    t(0, 0) = -1.0f; t(0, 1) = 0.0f; t(0, 2) = 2.0f;
    t(1, 0) = -3.0f; t(1, 1) = 4.0f; t(1, 2) = -5.0f;
    auto r = relu(t);
    EXPECT_FLOAT_EQ(r(0, 0), 0.0f);
    EXPECT_FLOAT_EQ(r(0, 1), 0.0f);
    EXPECT_FLOAT_EQ(r(0, 2), 2.0f);
    EXPECT_FLOAT_EQ(r(1, 0), 0.0f);
    EXPECT_FLOAT_EQ(r(1, 1), 4.0f);
    EXPECT_FLOAT_EQ(r(1, 2), 0.0f);
}

TEST(OperatorsTest, Sigmoid) {
    Tensor<float, 1> t({3});
    t(0) = 0.0f; t(1) = 2.0f; t(2) = -2.0f;
    auto s = sigmoid(t);
    EXPECT_NEAR(s(0), 0.5f, 0.01f);
    EXPECT_GT(s(1), 0.8f);
    EXPECT_LT(s(2), 0.2f);
}

TEST(OperatorsTest, Matmul) {
    Tensor<float, 2> a({2, 3});
    a(0, 0) = 1.0f; a(0, 1) = 2.0f; a(0, 2) = 3.0f;
    a(1, 0) = 4.0f; a(1, 1) = 5.0f; a(1, 2) = 6.0f;

    Tensor<float, 2> b({3, 2});
    b(0, 0) = 7.0f;  b(0, 1) = 8.0f;
    b(1, 0) = 9.0f;  b(1, 1) = 10.0f;
    b(2, 0) = 11.0f; b(2, 1) = 12.0f;

    auto c = matmul(a, b);
    EXPECT_EQ(c.shape(0), 2);
    EXPECT_EQ(c.shape(1), 2);

    EXPECT_FLOAT_EQ(c(0, 0), 58.0f);
    EXPECT_FLOAT_EQ(c(0, 1), 64.0f);
    EXPECT_FLOAT_EQ(c(1, 0), 139.0f);
    EXPECT_FLOAT_EQ(c(1, 1), 154.0f);
}

TEST(OperatorsTest, Softmax1D) {
    Tensor<float, 1> t({3});
    t(0) = 1.0f; t(1) = 2.0f; t(2) = 3.0f;
    auto s = softmax(t);
    float total = s(0) + s(1) + s(2);
    EXPECT_NEAR(total, 1.0f, 0.001f);
    EXPECT_LT(s(0), s(1));
    EXPECT_LT(s(1), s(2));
}

TEST(OperatorsTest, Softmax2D) {
    Tensor<float, 2> t({2, 3});
    t(0, 0) = 1.0f; t(0, 1) = 2.0f; t(0, 2) = 3.0f;
    t(1, 0) = 1.0f; t(1, 1) = 2.0f; t(1, 2) = 3.0f;
    auto s = softmax(t);
    for (int64_t i = 0; i < 2; ++i) {
        float rowSum = 0;
        for (int64_t j = 0; j < 3; ++j) rowSum += s(i, j);
        EXPECT_NEAR(rowSum, 1.0f, 0.001f);
    }
}

TEST(OperatorsTest, Sum) {
    Tensor<float, 2> t({2, 3});
    for (int64_t i = 0; i < 2; ++i)
        for (int64_t j = 0; j < 3; ++j)
            t(i, j) = static_cast<float>(i * 10 + j);
    EXPECT_FLOAT_EQ(sum(t), 0 + 1 + 2 + 10 + 11 + 12);
}

TEST(OperatorsTest, Mean) {
    Tensor<float, 2> t({2, 3});
    for (int64_t i = 0; i < 2; ++i)
        for (int64_t j = 0; j < 3; ++j)
            t(i, j) = static_cast<float>(i * 10 + j);
    float expected = (0 + 1 + 2 + 10 + 11 + 12) / 6.0f;
    EXPECT_FLOAT_EQ(mean(t), expected);
}

TEST(OperatorsTest, Max) {
    Tensor<float, 2> t({2, 3});
    t(0, 0) = 1.0f; t(0, 1) = 5.0f; t(0, 2) = 3.0f;
    t(1, 0) = 9.0f; t(1, 1) = 2.0f; t(1, 2) = 7.0f;
    EXPECT_FLOAT_EQ(micro::max(t), 9.0f);
}

TEST(OperatorsTest, Neg) {
    Tensor<float, 2> t({2, 3});
    t(0, 0) = 1.0f; t(0, 1) = -2.0f; t(0, 2) = 3.0f;
    t(1, 0) = -4.0f; t(1, 1) = 5.0f; t(1, 2) = -6.0f;
    auto n = neg(t);
    EXPECT_FLOAT_EQ(n(0, 0), -1.0f);
    EXPECT_FLOAT_EQ(n(0, 1), 2.0f);
    EXPECT_FLOAT_EQ(n(0, 2), -3.0f);
    EXPECT_FLOAT_EQ(n(1, 0), 4.0f);
    EXPECT_FLOAT_EQ(n(1, 1), -5.0f);
    EXPECT_FLOAT_EQ(n(1, 2), 6.0f);
}

TEST(OperatorsTest, ShapeMismatchThrows) {
    Tensor<float, 2> a({2, 3});
    Tensor<float, 2> b({3, 2});
    EXPECT_THROW(add(a, b), std::runtime_error);
}
