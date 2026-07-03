# 习题 03：矩阵类 — operator() 元素访问

## 目标

实现一个矩阵类，使用 `operator()` 进行元素访问（支持双下标语法）。

## 要求

实现 `Matrix` 类：

```cpp
class Matrix {
    double* data_;
    size_t rows_, cols_;
public:
    Matrix(size_t rows, size_t cols);
    ~Matrix();

    // 禁止拷贝，允许移动（简化实现）
    Matrix(const Matrix&) = delete;
    Matrix& operator=(const Matrix&) = delete;
    Matrix(Matrix&& other) noexcept;
    Matrix& operator=(Matrix&& other) noexcept;

    // operator() 用于元素访问：matrix(row, col)
    double& operator()(size_t row, size_t col);
    const double& operator()(size_t row, size_t col) const;

    size_t rows() const;
    size_t cols() const;

    // 矩阵乘法
    Matrix operator*(const Matrix& other) const;

    // 流输出
    friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
};
```

## 提示

- 使用一维数组存储二维数据：`data_[row * cols_ + col]`
- `operator()` 可以接受多个参数（不同于 `operator[]` 只能接受一个）
- 提供 const 和非 const 两个版本
- 矩阵乘法公式：`C[i][j] = sum(A[i][k] * B[k][j])`

## 预期输出

```
=== Matrix Creation ===
1 2 3
4 5 6

=== Element Access ===
mat(0,0) = 1
mat(1,2) = 6

=== Modify via operator() ===
mat(0,0) set to 10:
10 2 3
4 5 6

=== Matrix Multiplication ===
A (2x3):
1 2 3
4 5 6

B (3x2):
7 8
9 10
11 12

A * B (2x2):
58 64
139 154
```
