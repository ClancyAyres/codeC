// Exercise 01: Tensor2D Class — 完整解法

#include <iostream>
#include <vector>
#include <memory>
#include <cstdint>
#include <cassert>
#include <iomanip>
#include <stdexcept>

template<typename T>
class Tensor2D {
public:
    Tensor2D(int64_t rows, int64_t cols)
        : rows_(rows), cols_(cols)
        , stride0_(cols), stride1_(1)
        , storage_(new T[rows * cols])
        , data_(storage_.get())
    {
        std::fill(data_, data_ + rows * cols, T{});
    }

    T& at(int64_t i, int64_t j) {
        if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
            std::cerr << "Tensor2D::at(" << i << ", " << j << ") 越界! shape=("
                      << rows_ << ", " << cols_ << ")\n";
            throw std::out_of_range("Tensor2D index out of range");
        }
        return data_[i * stride0_ + j * stride1_];
    }

    const T& at(int64_t i, int64_t j) const {
        return const_cast<Tensor2D*>(this)->at(i, j);
    }

    int64_t rows() const { return rows_; }
    int64_t cols() const { return cols_; }

    std::vector<int64_t> shape() const { return {rows_, cols_}; }
    std::vector<int64_t> strides() const { return {stride0_, stride1_}; }

    T* data() { return data_; }
    const T* data() const { return data_; }
    int64_t size() const { return rows_ * cols_; }

    void print(const std::string& label) const {
        std::cout << "== " << label << " ==\n";
        std::cout << "  shape=(" << rows_ << ", " << cols_
                  << ")  strides=(" << stride0_ << ", " << stride1_ << ")\n";
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

private:
    int64_t rows_, cols_;
    int64_t stride0_, stride1_;
    std::shared_ptr<T[]> storage_;
    T* data_;
};

int main() {
    std::cout << "========== Tensor2D 类练习 ==========\n\n";

    Tensor2D<int> tensor(3, 4);
    for (int64_t i = 0; i < tensor.rows(); ++i)
        for (int64_t j = 0; j < tensor.cols(); ++j)
            tensor.at(i, j) = static_cast<int>(i * 10 + j);

    auto shp = tensor.shape();
    auto str = tensor.strides();
    std::cout << "shape: (" << shp[0] << ", " << shp[1] << ")\n";
    std::cout << "strides: (" << str[0] << ", " << str[1] << ")\n";

    tensor.print("3×4 张量");

    std::cout << "── 测试越界 ──\n";
    try {
        tensor.at(100, 100);
    } catch (const std::out_of_range& e) {
        std::cout << "  正确抛出异常: " << e.what() << "\n\n";
    }

    std::cout << "── 验证 data() 线性映射 ──\n";
    bool ok = true;
    for (int64_t i = 0; i < tensor.rows(); ++i) {
        for (int64_t j = 0; j < tensor.cols(); ++j) {
            if (tensor.data()[i * tensor.cols() + j] != tensor.at(i, j)) {
                std::cerr << "MISMATCH at (" << i << "," << j << ")\n";
                ok = false;
            }
        }
    }
    std::cout << (ok ? "  data() 线性索引与 at() 一致!\n" : "  存在问题!\n");

    std::cout << "\n========== 完成 ==========\n";
    std::cout << "关键点:\n";
    std::cout << "  at(i,j) = data_[i * stride0_ + j * stride1_]\n";
    std::cout << "  row-major: stride0_ = cols, stride1_ = 1\n";
    std::cout << "  shared_ptr 管理内存生命周期\n";

    return 0;
}
