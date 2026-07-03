#pragma once

#include <array>
#include <vector>
#include <memory>
#include <cstdint>
#include <stdexcept>
#include <numeric>
#include <algorithm>
#include <iostream>

namespace micro {

template<typename T, size_t Rank>
class Tensor {
public:
    using Shape = std::array<int64_t, Rank>;
    using Strides = std::array<int64_t, Rank>;

    Tensor() : offset_(0) {
        shape_.fill(0);
        computeStrides();
    }

    explicit Tensor(const Shape& shape, T init = T{})
        : storage_(std::make_shared<std::vector<T>>(computeSize(shape), init))
        , offset_(0)
        , shape_(shape)
    {
        computeStrides();
    }

    Tensor(const Shape& shape, const std::vector<T>& values)
        : storage_(std::make_shared<std::vector<T>>(values))
        , offset_(0)
        , shape_(shape)
    {
        if (static_cast<int64_t>(values.size()) != computeSize(shape)) {
            throw std::runtime_error("Tensor: values size does not match shape");
        }
        computeStrides();
    }

    template<typename... Indices>
    T& operator()(Indices... indices) {
        static_assert(sizeof...(Indices) == Rank, "index count must match Rank");
        return (*storage_)[offset_ + linearIndex(std::array<int64_t, Rank>{static_cast<int64_t>(indices)...})];
    }

    template<typename... Indices>
    const T& operator()(Indices... indices) const {
        static_assert(sizeof...(Indices) == Rank, "index count must match Rank");
        return (*storage_)[offset_ + linearIndex(std::array<int64_t, Rank>{static_cast<int64_t>(indices)...})];
    }

    T& at(const std::array<int64_t, Rank>& idx) {
        return (*storage_)[offset_ + linearIndex(idx)];
    }

    const T& at(const std::array<int64_t, Rank>& idx) const {
        return (*storage_)[offset_ + linearIndex(idx)];
    }

    Tensor slice(const Shape& start, const Shape& extents) const {
        for (size_t d = 0; d < Rank; ++d) {
            if (start[d] < 0 || start[d] + extents[d] > shape_[d]) {
                throw std::runtime_error("slice: index out of bounds");
            }
        }
        Tensor result;
        result.storage_ = storage_;
        result.offset_ = offset_ + linearIndex(start);
        result.shape_ = extents;
        result.strides_ = strides_;
        return result;
    }

    Tensor reshape(const Shape& new_shape) const {
        if (computeSize(new_shape) != computeSize(shape_)) {
            throw std::runtime_error("reshape: total element count mismatch");
        }
        if (!isContiguous()) {
            throw std::runtime_error("reshape: tensor must be contiguous");
        }
        Tensor result;
        result.storage_ = storage_;
        result.offset_ = offset_;
        result.shape_ = new_shape;
        result.computeStrides();
        return result;
    }

    Tensor copy() const {
        Tensor result(shape_);
        if (isContiguous() && offset_ == 0) {
            std::copy(storage_->begin(), storage_->end(), result.storage_->begin());
        } else {
            std::array<int64_t, Rank> idx{};
            copyElements(*this, result, idx, 0);
        }
        return result;
    }

    bool isContiguous() const {
        Strides expected;
        expected[Rank - 1] = 1;
        for (size_t d = Rank - 1; d > 0; --d) {
            expected[d - 1] = expected[d] * shape_[d];
        }
        return strides_ == expected;
    }

    const Shape& shape() const { return shape_; }
    int64_t shape(size_t dim) const { return shape_[dim]; }
    size_t size() const { return static_cast<size_t>(computeSize(shape_)); }
    size_t rank() const { return Rank; }

    T* data() { return storage_->data() + offset_; }
    const T* data() const { return storage_->data() + offset_; }

    std::shared_ptr<std::vector<T>>& storage() { return storage_; }
    const std::shared_ptr<std::vector<T>>& storage() const { return storage_; }
    int64_t offset() const { return offset_; }
    const Strides& strides() const { return strides_; }

private:
    Tensor(std::shared_ptr<std::vector<T>> storage, int64_t offset,
           const Shape& shape, const Strides& strides)
        : storage_(std::move(storage)), offset_(offset), shape_(shape), strides_(strides) {}

    static int64_t computeSize(const Shape& s) {
        int64_t total = 1;
        for (auto d : s) total *= d;
        return total;
    }

    void computeStrides() {
        strides_[Rank - 1] = 1;
        for (size_t d = Rank - 1; d > 0; --d) {
            strides_[d - 1] = strides_[d] * shape_[d];
        }
    }

    int64_t linearIndex(const std::array<int64_t, Rank>& idx) const {
        int64_t result = 0;
        for (size_t d = 0; d < Rank; ++d) {
            result += idx[d] * strides_[d];
        }
        return result;
    }

    static void copyElements(const Tensor& src, Tensor& dst,
                             std::array<int64_t, Rank>& idx, size_t dim) {
        if (dim == Rank) {
            dst.at(idx) = src.at(idx);
            return;
        }
        for (idx[dim] = 0; idx[dim] < src.shape_[dim]; ++idx[dim]) {
            copyElements(src, dst, idx, dim + 1);
        }
    }

    std::shared_ptr<std::vector<T>> storage_;
    int64_t offset_ = 0;
    Shape shape_;
    Strides strides_;
};

template<typename T>
using Tensor1D = Tensor<T, 1>;

template<typename T>
using Tensor2D = Tensor<T, 2>;

template<typename T>
using Tensor3D = Tensor<T, 3>;

} // namespace micro
