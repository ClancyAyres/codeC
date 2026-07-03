#pragma once

#include "tensor.hpp"
#include <cmath>
#include <algorithm>
#include <limits>
#include <stdexcept>

namespace micro {

template<size_t Rank>
bool incrementIndex(std::array<int64_t, Rank>& idx, const std::array<int64_t, Rank>& shape) {
    for (int d = static_cast<int>(Rank) - 1; d >= 0; --d) {
        idx[d]++;
        if (idx[d] < shape[d]) return true;
        idx[d] = 0;
    }
    return false;
}

template<typename T, size_t Rank>
Tensor<T, Rank> add(const Tensor<T, Rank>& a, const Tensor<T, Rank>& b) {
    if (a.shape() != b.shape()) throw std::runtime_error("add: shape mismatch");
    Tensor<T, Rank> result(a.shape());
    size_t n = a.size();
    if (a.isContiguous() && b.isContiguous() && result.isContiguous()) {
        const T* pa = a.data();
        const T* pb = b.data();
        T* pr = result.data();
        for (size_t i = 0; i < n; ++i) pr[i] = pa[i] + pb[i];
    } else {
        std::array<int64_t, Rank> idx{};
        do { result.at(idx) = a.at(idx) + b.at(idx); } while (incrementIndex(idx, a.shape()));
    }
    return result;
}

template<typename T, size_t Rank>
Tensor<T, Rank> sub(const Tensor<T, Rank>& a, const Tensor<T, Rank>& b) {
    if (a.shape() != b.shape()) throw std::runtime_error("sub: shape mismatch");
    Tensor<T, Rank> result(a.shape());
    size_t n = a.size();
    if (a.isContiguous() && b.isContiguous() && result.isContiguous()) {
        const T* pa = a.data();
        const T* pb = b.data();
        T* pr = result.data();
        for (size_t i = 0; i < n; ++i) pr[i] = pa[i] - pb[i];
    } else {
        std::array<int64_t, Rank> idx{};
        do { result.at(idx) = a.at(idx) - b.at(idx); } while (incrementIndex(idx, a.shape()));
    }
    return result;
}

template<typename T, size_t Rank>
Tensor<T, Rank> mul(const Tensor<T, Rank>& a, const Tensor<T, Rank>& b) {
    if (a.shape() != b.shape()) throw std::runtime_error("mul: shape mismatch");
    Tensor<T, Rank> result(a.shape());
    size_t n = a.size();
    if (a.isContiguous() && b.isContiguous() && result.isContiguous()) {
        const T* pa = a.data();
        const T* pb = b.data();
        T* pr = result.data();
        for (size_t i = 0; i < n; ++i) pr[i] = pa[i] * pb[i];
    } else {
        std::array<int64_t, Rank> idx{};
        do { result.at(idx) = a.at(idx) * b.at(idx); } while (incrementIndex(idx, a.shape()));
    }
    return result;
}

template<typename T, size_t Rank>
Tensor<T, Rank> div(const Tensor<T, Rank>& a, const Tensor<T, Rank>& b) {
    if (a.shape() != b.shape()) throw std::runtime_error("div: shape mismatch");
    Tensor<T, Rank> result(a.shape());
    size_t n = a.size();
    if (a.isContiguous() && b.isContiguous() && result.isContiguous()) {
        const T* pa = a.data();
        const T* pb = b.data();
        T* pr = result.data();
        for (size_t i = 0; i < n; ++i) pr[i] = pa[i] / pb[i];
    } else {
        std::array<int64_t, Rank> idx{};
        do { result.at(idx) = a.at(idx) / b.at(idx); } while (incrementIndex(idx, a.shape()));
    }
    return result;
}

template<typename T, size_t Rank>
Tensor<T, Rank> relu(const Tensor<T, Rank>& a) {
    Tensor<T, Rank> result(a.shape());
    size_t n = a.size();
    if (a.isContiguous() && result.isContiguous()) {
        const T* pa = a.data();
        T* pr = result.data();
        for (size_t i = 0; i < n; ++i) pr[i] = pa[i] > T{0} ? pa[i] : T{0};
    } else {
        std::array<int64_t, Rank> idx{};
        do {
            T val = a.at(idx);
            result.at(idx) = val > T{0} ? val : T{0};
        } while (incrementIndex(idx, a.shape()));
    }
    return result;
}

template<typename T, size_t Rank>
Tensor<T, Rank> sigmoid(const Tensor<T, Rank>& a) {
    Tensor<T, Rank> result(a.shape());
    size_t n = a.size();
    if (a.isContiguous() && result.isContiguous()) {
        const T* pa = a.data();
        T* pr = result.data();
        for (size_t i = 0; i < n; ++i) pr[i] = T{1} / (T{1} + std::exp(-pa[i]));
    } else {
        std::array<int64_t, Rank> idx{};
        do {
            result.at(idx) = T{1} / (T{1} + std::exp(-a.at(idx)));
        } while (incrementIndex(idx, a.shape()));
    }
    return result;
}

template<typename T>
Tensor<T, 2> matmul(const Tensor<T, 2>& a, const Tensor<T, 2>& b) {
    if (a.shape(1) != b.shape(0)) {
        throw std::runtime_error("matmul: inner dimension mismatch");
    }
    int64_t M = a.shape(0);
    int64_t K = a.shape(1);
    int64_t N = b.shape(1);

    Tensor<T, 2> result({M, N}, T{0});
    for (int64_t i = 0; i < M; ++i) {
        for (int64_t k = 0; k < K; ++k) {
            T aik = a(i, k);
            for (int64_t j = 0; j < N; ++j) {
                result(i, j) += aik * b(k, j);
            }
        }
    }
    return result;
}

template<typename T, size_t Rank>
Tensor<T, Rank> softmax(const Tensor<T, Rank>& a) {
    Tensor<T, Rank> result(a.shape());
    int64_t lastDim = a.shape(Rank - 1);
    int64_t outerSize = static_cast<int64_t>(a.size()) / lastDim;

    for (int64_t outer = 0; outer < outerSize; ++outer) {
        T maxVal = -std::numeric_limits<T>::max();
        for (int64_t j = 0; j < lastDim; ++j) {
            std::array<int64_t, Rank> idx{};
            int64_t rem = outer * lastDim + j;
            for (size_t d = Rank - 1; ; --d) {
                idx[d] = rem % a.shape(d);
                rem /= a.shape(d);
                if (d == 0) break;
            }
            T val = a.at(idx);
            if (val > maxVal) maxVal = val;
        }
        T sum = T{0};
        for (int64_t j = 0; j < lastDim; ++j) {
            std::array<int64_t, Rank> idx{};
            int64_t rem = outer * lastDim + j;
            for (size_t d = Rank - 1; ; --d) {
                idx[d] = rem % a.shape(d);
                rem /= a.shape(d);
                if (d == 0) break;
            }
            T val = std::exp(a.at(idx) - maxVal);
            result.at(idx) = val;
            sum += val;
        }
        for (int64_t j = 0; j < lastDim; ++j) {
            std::array<int64_t, Rank> idx{};
            int64_t rem = outer * lastDim + j;
            for (size_t d = Rank - 1; ; --d) {
                idx[d] = rem % a.shape(d);
                rem /= a.shape(d);
                if (d == 0) break;
            }
            result.at(idx) /= sum;
        }
    }
    return result;
}

template<typename T, size_t Rank>
T sum(const Tensor<T, Rank>& a) {
    T total = T{0};
    if (a.isContiguous()) {
        const T* p = a.data();
        for (size_t i = 0; i < a.size(); ++i) total += p[i];
    } else {
        std::array<int64_t, Rank> idx{};
        do { total += a.at(idx); } while (incrementIndex(idx, a.shape()));
    }
    return total;
}

template<typename T, size_t Rank>
T mean(const Tensor<T, Rank>& a) {
    return sum(a) / static_cast<T>(a.size());
}

template<typename T, size_t Rank>
T max(const Tensor<T, Rank>& a) {
    T maxVal = -std::numeric_limits<T>::max();
    if (a.isContiguous()) {
        const T* p = a.data();
        for (size_t i = 0; i < a.size(); ++i) {
            if (p[i] > maxVal) maxVal = p[i];
        }
    } else {
        std::array<int64_t, Rank> idx{};
        do {
            T val = a.at(idx);
            if (val > maxVal) maxVal = val;
        } while (incrementIndex(idx, a.shape()));
    }
    return maxVal;
}

template<typename T, size_t Rank>
Tensor<T, Rank> neg(const Tensor<T, Rank>& a) {
    Tensor<T, Rank> result(a.shape());
    size_t n = a.size();
    if (a.isContiguous() && result.isContiguous()) {
        const T* pa = a.data();
        T* pr = result.data();
        for (size_t i = 0; i < n; ++i) pr[i] = -pa[i];
    } else {
        std::array<int64_t, Rank> idx{};
        do { result.at(idx) = -a.at(idx); } while (incrementIndex(idx, a.shape()));
    }
    return result;
}

template<typename T, size_t Rank>
Tensor<T, Rank> operator+(const Tensor<T, Rank>& a, const Tensor<T, Rank>& b) { return add(a, b); }
template<typename T, size_t Rank>
Tensor<T, Rank> operator-(const Tensor<T, Rank>& a, const Tensor<T, Rank>& b) { return sub(a, b); }
template<typename T, size_t Rank>
Tensor<T, Rank> operator*(const Tensor<T, Rank>& a, const Tensor<T, Rank>& b) { return mul(a, b); }
template<typename T, size_t Rank>
Tensor<T, Rank> operator/(const Tensor<T, Rank>& a, const Tensor<T, Rank>& b) { return div(a, b); }

} // namespace micro
