// Exercise 02: Stride Transpose — 不拷贝数据的转置
// TODO: 实现通过交换 strides 来零拷贝转置，并验证数据一致性

#include <iostream>
#include <vector>
#include <memory>
#include <cstdint>
#include <cassert>
#include <iomanip>
#include <stdexcept>
#include <string>

// ── Tensor2D（提供基础功能） ────────────────────────────
template<typename T>
class Tensor2D {
public:
    Tensor2D(int64_t rows, int64_t cols)
        : rows_(rows), cols_(cols), stride0_(cols), stride1_(1)
        , storage_(new T[rows * cols])
        , data_(storage_.get()), offset_(0)
    {
        std::fill(data_, data_ + rows * cols, T{});
    }

    // 内部构造（用于创建视图）
    Tensor2D(std::shared_ptr<T[]> stor, T* ptr,
             int64_t rows, int64_t cols,
             int64_t s0, int64_t s1, int64_t off)
        : rows_(rows), cols_(cols), stride0_(s0), stride1_(s1)
        , storage_(std::move(stor)), data_(ptr), offset_(off) {}

    T& at(int64_t i, int64_t j) {
        if (i < 0 || i >= rows_ || j < 0 || j >= cols_)
            throw std::out_of_range("Tensor2D index out of range");
        return data_[offset_ + i * stride0_ + j * stride1_];
    }

    const T& at(int64_t i, int64_t j) const {
        return const_cast<Tensor2D*>(this)->at(i, j);
    }

    int64_t rows() const { return rows_; }
    int64_t cols() const { return cols_; }
    int64_t stride0() const { return stride0_; }
    int64_t stride1() const { return stride1_; }

    // TODO: 实现零拷贝转置
    // 提示: 创建新 Tensor2D 视图，交换 rows/cols，交换 strides
    //       共享同一个 storage_，不拷贝数据
    Tensor2D transpose() const {
        // TODO
        return *this;  // 占位
    }

    // TODO: 实现零拷贝切片（沿行方向）
    // slice_rows(start, end): 取第 [start, end) 行
    Tensor2D sliceRows(int64_t start, int64_t end) const {
        // TODO
        return *this;  // 占位
    }

    // TODO: 实现零拷贝切片（沿列方向）
    // sliceCols(start, end): 取第 [start, end) 列
    Tensor2D sliceCols(int64_t start, int64_t end) const {
        // TODO
        return *this;  // 占位
    }

    void print(const std::string& label) const {
        std::cout << "== " << label << " ==\n";
        std::cout << "  shape=(" << rows_ << ", " << cols_
                  << ")  strides=(" << stride0_ << ", " << stride1_
                  << ")  offset=" << offset_ << "\n";
        for (int64_t i = 0; i < rows_; ++i) {
            std::cout << "  [";
            for (int64_t j = 0; j < cols_; ++j) {
                std::cout << std::setw(4) << at(i, j);
                if (j < cols_ - 1) std::cout << ", ";
            }
            std::cout << " ]\n";
        }
        std::cout << '\n';
    }

public:  // 允许 exercise 访问（便于验证）
    std::shared_ptr<T[]> storage_;
    T* data_;
    int64_t rows_, cols_;
    int64_t stride0_, stride1_;
    int64_t offset_;
};

// ── main ────────────────────────────────────────────────

int main() {
    std::cout << "========== Stride 转置练习 ==========\n\n";

    // 创建 3×5 矩阵
    Tensor2D<int> original(3, 5);
    for (int64_t i = 0; i < original.rows(); ++i)
        for (int64_t j = 0; j < original.cols(); ++j)
            original.at(i, j) = static_cast<int>(i * 10 + j);

    original.print("原始矩阵 (3×5)");

    // TODO: 零拷贝转置
    // auto transposed = original.transpose();
    // transposed.print("转置 (5×3)");

    // TODO: 验证: transposed(j,i) == original(i,j)

    // TODO: 修改转置视图，验证原始数据也被修改
    // transposed.at(0, 1) = 999;
    // 验证 original.at(1, 0) == 999

    // TODO: 切片测试
    // auto sliced = original.sliceRows(0, 2).sliceCols(1, 4);
    // 预期 shape=(2, 3)

    std::cout << "TODO: 实现 transpose(), sliceRows(), sliceCols()\n\n";

    std::cout << "提示:\n";
    std::cout << "  transpose:  rows_↔cols_, stride0_↔stride1_\n";
    std::cout << "  sliceRows:  rows_=end-start, offset_+=start*stride0_\n";
    std::cout << "  sliceCols:  cols_=end-start, offset_+=start*stride1_\n";

    return 0;
}
