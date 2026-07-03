#pragma once

#include "tensor.hpp"
#include "operators.hpp"
#include <functional>
#include <vector>
#include <memory>
#include <stdexcept>

namespace micro {

template<typename T, size_t Rank>
class Var {
public:
    Tensor<T, Rank> data;
    Tensor<T, Rank> grad;
    std::function<void()> backward_fn;
    std::vector<std::shared_ptr<Var<T, Rank>>> parents;
    bool requires_grad;

    Var() : requires_grad(false) {}

    explicit Var(const Tensor<T, Rank>& tensor, bool req_grad = false)
        : data(tensor)
        , grad(tensor.shape(), T{0})
        , requires_grad(req_grad) {}

    Var(const typename Tensor<T, Rank>::Shape& shape, T init, bool req_grad = false)
        : data(shape, init)
        , grad(shape, T{0})
        , requires_grad(req_grad) {}

    static std::shared_ptr<Var<T, Rank>> make(const Tensor<T, Rank>& tensor, bool req_grad = false) {
        return std::make_shared<Var<T, Rank>>(tensor, req_grad);
    }

    static std::shared_ptr<Var<T, Rank>> make(const typename Tensor<T, Rank>::Shape& shape, T init, bool req_grad = false) {
        return std::make_shared<Var<T, Rank>>(shape, init, req_grad);
    }

    void zeroGrad() {
        if (isContiguous()) {
            std::fill(grad.data(), grad.data() + grad.size(), T{0});
        } else {
            std::array<int64_t, Rank> idx{};
            do { grad.at(idx) = T{0}; } while (incrementIndex(idx, grad.shape()));
        }
    }

    void backward() {
        if (grad.size() > 0) {
            if (isContiguous()) {
                std::fill(grad.data(), grad.data() + grad.size(), T{1});
            } else {
                std::array<int64_t, Rank> idx{};
                do { grad.at(idx) = T{1}; } while (incrementIndex(idx, grad.shape()));
            }
        }
        backwardImpl();
    }

    void backwardImpl() {
        if (backward_fn) backward_fn();
    }

private:
    bool isContiguous() const { return grad.isContiguous(); }
};

template<typename T, size_t Rank>
std::shared_ptr<Var<T, Rank>> varAdd(
    const std::shared_ptr<Var<T, Rank>>& a,
    const std::shared_ptr<Var<T, Rank>>& b)
{
    auto result = Var<T, Rank>::make(add(a->data, b->data), true);

    result->parents = {a, b};
    result->backward_fn = [a, b, result]() {
        if (a->grad.isContiguous() && result->grad.isContiguous()) {
            const T* pg = result->grad.data();
            T* pa = a->grad.data();
            size_t n = a->data.size();
            for (size_t i = 0; i < n; ++i) pa[i] += pg[i];
        } else {
            std::array<int64_t, Rank> idx{};
            do { a->grad.at(idx) += result->grad.at(idx); } while (incrementIndex(idx, a->grad.shape()));
        }
        if (b->grad.isContiguous() && result->grad.isContiguous()) {
            const T* pg = result->grad.data();
            T* pb = b->grad.data();
            size_t n = b->data.size();
            for (size_t i = 0; i < n; ++i) pb[i] += pg[i];
        } else {
            std::array<int64_t, Rank> idx{};
            do { b->grad.at(idx) += result->grad.at(idx); } while (incrementIndex(idx, b->grad.shape()));
        }
    };
    return result;
}

template<typename T, size_t Rank>
std::shared_ptr<Var<T, Rank>> varMul(
    const std::shared_ptr<Var<T, Rank>>& a,
    const std::shared_ptr<Var<T, Rank>>& b)
{
    auto result = Var<T, Rank>::make(mul(a->data, b->data), true);

    result->parents = {a, b};
    result->backward_fn = [a, b, result]() {
        if (a->grad.isContiguous() && b->data.isContiguous() && result->grad.isContiguous()) {
            const T* pg = result->grad.data();
            const T* pb = b->data.data();
            T* pa = a->grad.data();
            size_t n = a->data.size();
            for (size_t i = 0; i < n; ++i) pa[i] += pg[i] * pb[i];
        } else {
            std::array<int64_t, Rank> idx{};
            do { a->grad.at(idx) += result->grad.at(idx) * b->data.at(idx); } while (incrementIndex(idx, a->grad.shape()));
        }
        if (b->grad.isContiguous() && a->data.isContiguous() && result->grad.isContiguous()) {
            const T* pg = result->grad.data();
            const T* pa = a->data.data();
            T* pb = b->grad.data();
            size_t n = b->data.size();
            for (size_t i = 0; i < n; ++i) pb[i] += pg[i] * pa[i];
        } else {
            std::array<int64_t, Rank> idx{};
            do { b->grad.at(idx) += result->grad.at(idx) * a->data.at(idx); } while (incrementIndex(idx, b->grad.shape()));
        }
    };
    return result;
}

template<typename T, size_t Rank>
std::shared_ptr<Var<T, Rank>> varRelu(
    const std::shared_ptr<Var<T, Rank>>& a)
{
    auto result = Var<T, Rank>::make(relu(a->data), true);

    result->parents = {a};
    result->backward_fn = [a, result]() {
        if (a->grad.isContiguous() && a->data.isContiguous() && result->grad.isContiguous()) {
            const T* pg = result->grad.data();
            const T* pd = a->data.data();
            T* pa = a->grad.data();
            size_t n = a->data.size();
            for (size_t i = 0; i < n; ++i) {
                pa[i] += pd[i] > T{0} ? pg[i] : T{0};
            }
        } else {
            std::array<int64_t, Rank> idx{};
            do {
                a->grad.at(idx) += a->data.at(idx) > T{0} ? result->grad.at(idx) : T{0};
            } while (incrementIndex(idx, a->grad.shape()));
        }
    };
    return result;
}

template<typename T>
std::shared_ptr<Var<T, 2>> varMatmul(
    const std::shared_ptr<Var<T, 2>>& a,
    const std::shared_ptr<Var<T, 2>>& b)
{
    auto result = Var<T, 2>::make(matmul(a->data, b->data), true);

    result->parents = {a, b};
    result->backward_fn = [a, b, result]() {
        int64_t M = a->data.shape(0);
        int64_t K = a->data.shape(1);
        int64_t N = b->data.shape(1);

        for (int64_t i = 0; i < M; ++i) {
            for (int64_t j = 0; j < N; ++j) {
                T g = result->grad(i, j);
                for (int64_t k = 0; k < K; ++k) {
                    a->grad(i, k) += g * b->data(k, j);
                }
            }
        }
        for (int64_t i = 0; i < M; ++i) {
            for (int64_t j = 0; j < N; ++j) {
                T g = result->grad(i, j);
                for (int64_t k = 0; k < K; ++k) {
                    b->grad(k, j) += a->data(i, k) * g;
                }
            }
        }
    };
    return result;
}

template<typename T, size_t Rank>
void backward(std::shared_ptr<Var<T, Rank>> var) {
    var->backward();
}

template<typename T, size_t Rank>
void topologicalSort(
    std::shared_ptr<Var<T, Rank>> node,
    std::vector<std::shared_ptr<Var<T, Rank>>>& sorted,
    std::vector<std::shared_ptr<Var<T, Rank>>>& visited)
{
    for (auto& v : visited) {
        if (v == node) return;
    }
    visited.push_back(node);
    for (auto& parent : node->parents) {
        topologicalSort(parent, sorted, visited);
    }
    sorted.push_back(node);
}

template<typename T, size_t Rank>
void backwardGraph(std::shared_ptr<Var<T, Rank>> node) {
    std::vector<std::shared_ptr<Var<T, Rank>>> sorted;
    std::vector<std::shared_ptr<Var<T, Rank>>> visited;
    topologicalSort(node, sorted, visited);

    for (auto& v : sorted) v->zeroGrad();

    if (node->grad.size() > 0) {
        if (node->grad.isContiguous()) {
            std::fill(node->grad.data(), node->grad.data() + node->grad.size(), T{1});
        } else {
            std::array<int64_t, Rank> idx{};
            do { node->grad.at(idx) = T{1}; } while (incrementIndex(idx, node->grad.shape()));
        }
    }

    for (auto it = sorted.rbegin(); it != sorted.rend(); ++it) {
        (*it)->backwardImpl();
    }
}

} // namespace micro
