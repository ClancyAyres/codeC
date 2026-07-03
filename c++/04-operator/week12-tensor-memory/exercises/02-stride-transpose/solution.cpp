// Exercise 02: Stride Transpose — 完整解法

#include <iostream>
#include <vector>
#include <memory>
#include <cstdint>
#include <cassert>
#include <iomanip>
#include <stdexcept>
#include <string>

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

    Tensor2D transpose() const {
        return Tensor2D(storage_, data_,
                        cols_, rows_,
                        stride1_, stride0_, offset_);
    }

    Tensor2D sliceRows(int64_t start, int64_t end) const {
        if (start < 0 || end > rows_ || start >= end)
            throw std::out_of_range("sliceRows out of range");
        return Tensor2D(storage_, data_,
                        end - start, cols_,
                        stride0_, stride1_,
                        offset_ + start * stride0_);
    }

    Tensor2D sliceCols(int64_t start, int64_t end) const {
        if (start < 0 || end > cols_ || start >= end)
            throw std::out_of_range("sliceCols out of range");
        return Tensor2D(storage_, data_,
                        rows_, end - start,
                        stride0_, stride1_,
                        offset_ + start * stride1_);
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

public:
    int64_t rows_, cols_;
    int64_t stride0_, stride1_;
    std::shared_ptr<T[]> storage_;
    T* data_;
    int64_t offset_;
};

int main() {
    std::cout << "========== Stride 转置练习 ==========\n\n";

    Tensor2D<int> original(3, 5);
    for (int64_t i = 0; i < original.rows(); ++i)
        for (int64_t j = 0; j < original.cols(); ++j)
            original.at(i, j) = static_cast<int>(i * 10 + j);

    original.print("原始矩阵 (3×5)");

    auto transposed = original.transpose();
    transposed.print("转置 (5×3) — 零拷贝!");

    std::cout << "── 验证转置正确性 ──\n";
    bool transposeOk = true;
    for (int64_t i = 0; i < original.rows(); ++i) {
        for (int64_t j = 0; j < original.cols(); ++j) {
            if (transposed.at(j, i) != original.at(i, j)) {
                std::cerr << "MISMATCH at (" << i << "," << j << ")\n";
                transposeOk = false;
            }
        }
    }
    std::cout << (transposeOk ? "  transposed(j,i) == original(i,j) 通过!\n" : "  失败!\n");

    std::cout << "\n── 验证共享内存 ──\n";
    std::cout << "  original.data()  == transposed.data(): "
              << (original.data_ == transposed.data_ ? "YES (共享!)\n" : "NO\n");
    std::cout << "  original.storage == transposed.storage: "
              << (original.storage_ == transposed.storage_ ? "YES (共享!)\n" : "NO\n");

    std::cout << "\n── 修改转置视图 → 查看原始数据 ──\n";
    transposed.at(0, 1) = 999;
    std::cout << "  修改 transposed(0, 1) = 999\n";
    std::cout << "  original(1, 0) = " << original.at(1, 0)
              << " (应为 999) → "
              << (original.at(1, 0) == 999 ? "正确! 共享内存!\n" : "WRONG!\n");

    // 恢复
    transposed.at(0, 1) = 10;

    std::cout << "\n── 切片测试 ──\n";
    auto sliced = original.sliceRows(0, 2).sliceCols(1, 4);
    sliced.print("切片 (2×3): rows[0,2), cols[1,4)");
    std::cout << "  预期: 第0-1行, 第1-3列 = [01, 02, 03], [11, 12, 13]\n";

    std::cout << "\n── 验证切片值 ──\n";
    bool sliceOk = true;
    for (int64_t i = 0; i < sliced.rows(); ++i) {
        for (int64_t j = 0; j < sliced.cols(); ++j) {
            int expected = static_cast<int>(i * 10 + (j + 1));
            if (sliced.at(i, j) != expected) {
                std::cerr << "MISMATCH at slice(" << i << "," << j << ") = "
                          << sliced.at(i, j) << " expected " << expected << "\n";
                sliceOk = false;
            }
        }
    }
    std::cout << (sliceOk ? "  切片值正确!\n" : "  存在问题!\n");

    std::cout << "\n── 切片零拷贝验证 ──\n";
    std::cout << "  sliced.storage == original.storage: "
              << (sliced.storage_ == original.storage_ ? "YES (共享!)\n" : "NO\n");
    std::cout << "  sliced.data() == original.data(): "
              << (sliced.data_ == original.data_ ? "YES (共享数据指针!)\n" : "NO\n");
    std::cout << "  sliced offset = " << sliced.offset_ << " (跳过第一行4个元素)\n";

    std::cout << "\n========== 关键理解 ==========\n";
    std::cout << "1. transpose: 只交换 shape/strides，数据不动\n";
    std::cout << "2. slice: 只改 shape 和 offset，共享数据\n";
    std::cout << "3. 所有视图共享 storage (shared_ptr)\n";
    std::cout << "4. 修改一个视图 → 所有视图可见\n";
    std::cout << "5. 全 O(1) 操作，无任何数据拷贝!\n";

    return 0;
}
