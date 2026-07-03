#include <cassert>
#include <iomanip>
#include <iostream>
#include <utility>

class Matrix {
    double* data_ = nullptr;
    size_t rows_ = 0, cols_ = 0;

public:
    Matrix(size_t rows, size_t cols)
        : data_(new double[rows * cols]{}), rows_(rows), cols_(cols) {
    }

    ~Matrix() { delete[] data_; }

    Matrix(const Matrix&) = delete;
    Matrix& operator=(const Matrix&) = delete;

    Matrix(Matrix&& other) noexcept
        : data_(std::exchange(other.data_, nullptr))
        , rows_(std::exchange(other.rows_, 0))
        , cols_(std::exchange(other.cols_, 0)) {
    }

    Matrix& operator=(Matrix&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = std::exchange(other.data_, nullptr);
            rows_ = std::exchange(other.rows_, 0);
            cols_ = std::exchange(other.cols_, 0);
        }
        return *this;
    }

    double& operator()(size_t row, size_t col) {
        return data_[row * cols_ + col];
    }

    const double& operator()(size_t row, size_t col) const {
        return data_[row * cols_ + col];
    }

    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }

    Matrix operator*(const Matrix& other) const {
        assert(cols_ == other.rows_ && "Matrix dimensions must agree");
        Matrix result(rows_, other.cols_);
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < other.cols_; ++j) {
                double sum = 0.0;
                for (size_t k = 0; k < cols_; ++k) {
                    sum += (*this)(i, k) * other(k, j);
                }
                result(i, j) = sum;
            }
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (size_t r = 0; r < m.rows_; ++r) {
            for (size_t c = 0; c < m.cols_; ++c) {
                os << std::setw(4) << m(r, c) << ' ';
            }
            os << '\n';
        }
        return os;
    }
};

int main() {
    std::cout << "=== Matrix Creation ===\n";
    Matrix mat(2, 3);
    double val = 1.0;
    for (size_t r = 0; r < mat.rows(); ++r) {
        for (size_t c = 0; c < mat.cols(); ++c) {
            mat(r, c) = val++;
        }
    }
    std::cout << mat;

    std::cout << "\n=== Element Access ===\n";
    std::cout << "mat(0,0) = " << mat(0, 0) << '\n';
    std::cout << "mat(1,2) = " << mat(1, 2) << '\n';

    std::cout << "\n=== Modify via operator() ===\n";
    mat(0, 0) = 10;
    std::cout << "mat(0,0) set to 10:\n" << mat;

    std::cout << "\n=== Matrix Multiplication ===\n";
    Matrix A(2, 3);
    double aVal = 1.0;
    for (size_t r = 0; r < A.rows(); ++r)
        for (size_t c = 0; c < A.cols(); ++c)
            A(r, c) = aVal++;

    Matrix B(3, 2);
    double bVal = 7.0;
    for (size_t r = 0; r < B.rows(); ++r)
        for (size_t c = 0; c < B.cols(); ++c)
            B(r, c) = bVal++;

    std::cout << "A (2x3):\n" << A;
    std::cout << "B (3x2):\n" << B;

    Matrix C = A * B;
    std::cout << "A * B (2x2):\n" << C;

    return 0;
}
