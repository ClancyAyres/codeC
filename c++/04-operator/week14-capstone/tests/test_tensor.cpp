#include <gtest/gtest.h>
#include "tensor.hpp"

using namespace micro;

TEST(TensorTest, CreateEmpty) {
    Tensor<float, 2> t({3, 4}, 0.0f);
    EXPECT_EQ(t.size(), 12);
    EXPECT_EQ(t.shape(0), 3);
    EXPECT_EQ(t.shape(1), 4);
    EXPECT_EQ(t.rank(), 2);
}

TEST(TensorTest, CreateAndAccess) {
    Tensor<float, 2> t({3, 4});
    for (int64_t i = 0; i < 3; ++i)
        for (int64_t j = 0; j < 4; ++j)
            t(i, j) = static_cast<float>(i * 10 + j);

    EXPECT_FLOAT_EQ(t(0, 0), 0.0f);
    EXPECT_FLOAT_EQ(t(0, 3), 3.0f);
    EXPECT_FLOAT_EQ(t(2, 0), 20.0f);
    EXPECT_FLOAT_EQ(t(2, 3), 23.0f);
}

TEST(TensorTest, RowMajorStorage) {
    Tensor<float, 2> t({2, 3});
    t(0, 0) = 1.0f;
    t(0, 1) = 2.0f;
    t(0, 2) = 3.0f;
    t(1, 0) = 4.0f;
    t(1, 1) = 5.0f;
    t(1, 2) = 6.0f;

    EXPECT_TRUE(t.isContiguous());
    const float* d = t.data();
    EXPECT_FLOAT_EQ(d[0], 1.0f);
    EXPECT_FLOAT_EQ(d[1], 2.0f);
    EXPECT_FLOAT_EQ(d[2], 3.0f);
    EXPECT_FLOAT_EQ(d[3], 4.0f);
    EXPECT_FLOAT_EQ(d[4], 5.0f);
    EXPECT_FLOAT_EQ(d[5], 6.0f);
}

TEST(TensorTest, Slice2D) {
    Tensor<float, 2> t({4, 5});
    for (int64_t i = 0; i < 4; ++i)
        for (int64_t j = 0; j < 5; ++j)
            t(i, j) = static_cast<float>(i * 10 + j);

    auto s = t.slice({1, 1}, {2, 3});
    EXPECT_EQ(s.shape(0), 2);
    EXPECT_EQ(s.shape(1), 3);
    EXPECT_EQ(s.size(), 6);

    EXPECT_FLOAT_EQ(s(0, 0), 11.0f);
    EXPECT_FLOAT_EQ(s(0, 2), 13.0f);
    EXPECT_FLOAT_EQ(s(1, 0), 21.0f);
    EXPECT_FLOAT_EQ(s(1, 2), 23.0f);
}

TEST(TensorTest, Slice3D) {
    Tensor<float, 3> t({3, 4, 5});
    for (int64_t i = 0; i < 3; ++i)
        for (int64_t j = 0; j < 4; ++j)
            for (int64_t k = 0; k < 5; ++k)
                t(i, j, k) = static_cast<float>(i * 100 + j * 10 + k);

    auto s = t.slice({1, 0, 2}, {1, 3, 2});
    EXPECT_EQ(s.shape(0), 1);
    EXPECT_EQ(s.shape(1), 3);
    EXPECT_EQ(s.shape(2), 2);
    EXPECT_FLOAT_EQ(s(0, 0, 0), 102.0f);
    EXPECT_FLOAT_EQ(s(0, 0, 1), 103.0f);
    EXPECT_FLOAT_EQ(s(0, 2, 0), 122.0f);
    EXPECT_FLOAT_EQ(s(0, 2, 1), 123.0f);
}

TEST(TensorTest, Reshape) {
    Tensor<float, 2> t({3, 4});
    for (int64_t i = 0; i < 3; ++i)
        for (int64_t j = 0; j < 4; ++j)
            t(i, j) = static_cast<float>(i * 10 + j);

    auto r = t.reshape({4, 3});
    EXPECT_EQ(r.shape(0), 4);
    EXPECT_EQ(r.shape(1), 3);
    EXPECT_EQ(r.size(), 12);
    EXPECT_FLOAT_EQ(r(0, 0), 0.0f);
    EXPECT_FLOAT_EQ(r(0, 2), 2.0f);
    EXPECT_FLOAT_EQ(r(1, 0), 3.0f);
}

TEST(TensorTest, ReshapeThrowsOnMismatch) {
    Tensor<float, 2> t({3, 4});
    EXPECT_THROW(t.reshape({2, 7}), std::runtime_error);
}

TEST(TensorTest, Copy) {
    Tensor<float, 2> t({2, 3});
    t(0, 0) = 1.0f; t(0, 1) = 2.0f; t(0, 2) = 3.0f;
    t(1, 0) = 4.0f; t(1, 1) = 5.0f; t(1, 2) = 6.0f;

    auto c = t.copy();
    EXPECT_EQ(c.shape(), t.shape());
    EXPECT_FLOAT_EQ(c(0, 0), 1.0f);
    EXPECT_FLOAT_EQ(c(1, 2), 6.0f);

    c(0, 0) = 99.0f;
    EXPECT_FLOAT_EQ(t(0, 0), 1.0f);
}

TEST(TensorTest, 1DTensor) {
    Tensor<float, 1> t({5});
    t(0) = 1.0f; t(1) = 2.0f; t(2) = 3.0f; t(3) = 4.0f; t(4) = 5.0f;
    EXPECT_EQ(t.size(), 5);
    EXPECT_FLOAT_EQ(t(2), 3.0f);
    EXPECT_FLOAT_EQ(t(4), 5.0f);
}

TEST(TensorTest, 1DSlice) {
    Tensor<float, 1> t({10});
    for (int64_t i = 0; i < 10; ++i) t(i) = static_cast<float>(i);
    auto s = t.slice({2}, {4});
    EXPECT_EQ(s.size(), 4);
    EXPECT_FLOAT_EQ(s(0), 2.0f);
    EXPECT_FLOAT_EQ(s(3), 5.0f);
}
