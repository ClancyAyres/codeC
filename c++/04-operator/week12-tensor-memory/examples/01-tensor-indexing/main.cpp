// Example 01: Tensor Indexing — 手写2D/3D张量线性索引计算 (row-major)
// 目的：理解多维索引到一维内存的映射，以及 stride 的推导过程

#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <iomanip>
#include <string>

// ── 通用 N 维 row-major stride 计算 ─────────────────────────
std::vector<int64_t> computeStrides(const std::vector<int64_t>& shape) {
    std::vector<int64_t> strides(shape.size(), 1);
    for (int d = static_cast<int>(shape.size()) - 2; d >= 0; --d) {
        strides[d] = strides[d + 1] * shape[d + 1];
    }
    return strides;
}

// ── N 维 row-major 线性索引 ─────────────────────────────────
int64_t linearIndex(const std::vector<int64_t>& index,
                    const std::vector<int64_t>& strides,
                    int64_t offset = 0) {
    assert(index.size() == strides.size());
    int64_t idx = offset;
    for (size_t d = 0; d < index.size(); ++d) {
        idx += index[d] * strides[d];
    }
    return idx;
}

// ── 将线性索引展开回多维索引 ────────────────────────────────
std::vector<int64_t> unravelIndex(int64_t linear,
                                  const std::vector<int64_t>& shape,
                                  const std::vector<int64_t>& strides) {
    std::vector<int64_t> index(shape.size());
    for (size_t d = 0; d < shape.size(); ++d) {
        index[d] = (linear / strides[d]) % shape[d];
    }
    return index;
}

// ── 打印 2D 张量 ────────────────────────────────────────────
void print2D(const std::string& name,
             const std::vector<int64_t>& shape,
             const std::vector<int64_t>& strides,
             const std::vector<int>& data) {
    std::cout << "=== " << name << " ===\n";
    std::cout << "shape=(" << shape[0] << ", " << shape[1] << ") "
              << "strides=(" << strides[0] << ", " << strides[1] << ")\n";
    for (int64_t i = 0; i < shape[0]; ++i) {
        std::cout << "  [";
        for (int64_t j = 0; j < shape[1]; ++j) {
            int64_t idx = linearIndex({i, j}, strides);
            std::cout << std::setw(3) << data[idx];
            if (j < shape[1] - 1) std::cout << ", ";
        }
        std::cout << " ]\n";
    }
    std::cout << '\n';
}

// ── 打印 3D 张量 ────────────────────────────────────────────
void print3D(const std::string& name,
             const std::vector<int64_t>& shape,
             const std::vector<int64_t>& strides,
             const std::vector<int>& data) {
    std::cout << "=== " << name << " ===\n";
    std::cout << "shape=(" << shape[0] << ", " << shape[1] << ", " << shape[2] << ") "
              << "strides=(" << strides[0] << ", " << strides[1] << ", " << strides[2] << ")\n";
    for (int64_t i = 0; i < shape[0]; ++i) {
        std::cout << "Depth " << i << ":\n";
        for (int64_t j = 0; j < shape[1]; ++j) {
            std::cout << "  [";
            for (int64_t k = 0; k < shape[2]; ++k) {
                int64_t idx = linearIndex({i, j, k}, strides);
                std::cout << std::setw(3) << data[idx];
                if (k < shape[2] - 1) std::cout << ", ";
            }
            std::cout << " ]\n";
        }
    }
    std::cout << '\n';
}

// ── raw 2D 索引 (显式公式) ───────────────────────────────────
void demoRaw2DIndexing() {
    const int64_t rows = 3, cols = 4;
    std::vector<int> data(rows * cols);
    // 填充: data[i][j] = i*10 + j
    for (int64_t i = 0; i < rows; ++i) {
        for (int64_t j = 0; j < cols; ++j) {
            data[i * cols + j] = static_cast<int>(i * 10 + j);  // row-major 公式
        }
    }

    std::cout << "── 2D Row-Major 索引 (显式公式) ──\n";
    std::cout << "公式: linear = i * " << cols << " + j\n";
    for (int64_t i = 0; i < rows; ++i) {
        std::cout << "  Row " << i << ": ";
        for (int64_t j = 0; j < cols; ++j) {
            std::cout << data[i * cols + j] << " ";
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

// ── raw 3D 索引 (显式公式) ───────────────────────────────────
void demoRaw3DIndexing() {
    const int64_t depth = 2, rows = 3, cols = 4;
    std::vector<int> data(depth * rows * cols);
    // 填充: data[i][j][k] = i*100 + j*10 + k
    for (int64_t i = 0; i < depth; ++i) {
        for (int64_t j = 0; j < rows; ++j) {
            for (int64_t k = 0; k < cols; ++k) {
                data[(i * rows + j) * cols + k] = static_cast<int>(i * 100 + j * 10 + k);
            }
        }
    }

    std::cout << "── 3D Row-Major 索引 (显式公式) ──\n";
    std::cout << "公式: linear = (i * " << rows << " + j) * " << cols << " + k\n";
    for (int64_t i = 0; i < depth; ++i) {
        std::cout << "Depth " << i << ":\n";
        for (int64_t j = 0; j < rows; ++j) {
            std::cout << "  [";
            for (int64_t k = 0; k < cols; ++k) {
                int64_t idx = (i * rows + j) * cols + k;
                std::cout << std::setw(4) << data[idx];
            }
            std::cout << " ]\n";
        }
    }
    std::cout << '\n';
}

int main() {
    std::cout << "========== 张量线性索引计算 (Row-Major) ==========\n\n";

    // ── 演示1: 显式公式 2D / 3D 索引 ─────────────────────
    demoRaw2DIndexing();
    demoRaw3DIndexing();

    // ── 演示2: 通用 stride 法 ────────────────────────────
    {
        std::vector<int64_t> shape = {3, 4};
        std::vector<int64_t> strides = computeStrides(shape);
        std::vector<int> data(12);
        for (int64_t i = 0; i < shape[0]; ++i)
            for (int64_t j = 0; j < shape[1]; ++j)
                data[linearIndex({i, j}, strides)] = static_cast<int>(i * 10 + j);

        print2D("2D Stride 法", shape, strides, data);
    }

    // ── 演示3: 3D stride 索引 ────────────────────────────
    {
        std::vector<int64_t> shape = {2, 3, 4};
        std::vector<int64_t> strides = computeStrides(shape);
        std::vector<int> data(2 * 3 * 4);
        for (int64_t i = 0; i < shape[0]; ++i)
            for (int64_t j = 0; j < shape[1]; ++j)
                for (int64_t k = 0; k < shape[2]; ++k)
                    data[linearIndex({i, j, k}, strides)] = static_cast<int>(i * 100 + j * 10 + k);

        print3D("3D Stride 法", shape, strides, data);
    }

    // ── 演示4: 索引 ⇄ 线性 互转验证 ──────────────────────
    std::cout << "── 多维索引 ⇄ 线性索引 互转验证 ──\n";
    {
        std::vector<int64_t> shape = {2, 3, 4};
        std::vector<int64_t> strides = computeStrides(shape);

        for (int64_t i = 0; i < shape[0]; ++i) {
            for (int64_t j = 0; j < shape[1]; ++j) {
                for (int64_t k = 0; k < shape[2]; ++k) {
                    std::vector<int64_t> idx = {i, j, k};
                    int64_t lin = linearIndex(idx, strides);
                    std::vector<int64_t> recovered = unravelIndex(lin, shape, strides);
                    bool ok = (recovered[0] == i && recovered[1] == j && recovered[2] == k);
                    if (!ok) {
                        std::cerr << "MISMATCH at (" << i << "," << j << "," << k
                                  << ") → linear=" << lin
                                  << " → (" << recovered[0] << "," << recovered[1] << "," << recovered[2] << ")\n";
                    }
                }
            }
        }
        std::cout << "  所有索引互转验证通过!\n\n";
    }

    // ── 演示5: 非标准 stride (有空洞的张量) ───────────────
    std::cout << "── 非标准 stride (空洞/跨步张量) ──\n";
    {
        // 逻辑 2×3，但物理 strides=[6, 2] → 内存中有"间隙"
        std::vector<int> data = {10, 11, 0, 20, 21, 0, 30, 31, 0, 0, 0, 0};
        std::vector<int64_t> shape = {2, 3};
        std::vector<int64_t> strides = {6, 2};

        std::cout << "shape=(2,3), strides=(6,2) — 每行数据后有一个空闲槽\n";
        std::cout << "原始内存布局: ";
        for (size_t i = 0; i < data.size(); ++i) std::cout << data[i] << " ";
        std::cout << "\n逻辑视图:\n";
        for (int64_t i = 0; i < shape[0]; ++i) {
            std::cout << "  [";
            for (int64_t j = 0; j < shape[1]; ++j) {
                std::cout << std::setw(3) << data[i * strides[0] + j * strides[1]];
                if (j < shape[1] - 1) std::cout << ", ";
            }
            std::cout << " ]\n";
        }
        std::cout << "(物理内存有12个元素，逻辑只用6个 — stride 允许空洞)\n\n";
    }

    std::cout << "========== 关键理解 ==========\n";
    std::cout << "1. N维索引 → 1维内存: linear = sum(index[d] * stride[d])\n";
    std::cout << "2. Row-major stride: last dim = 1, 往前逐级乘以 shape\n";
    std::cout << "3. stride 与 shape 解耦 → 零拷贝切片/转置/广播的基础\n";
    std::cout << "4. 1维内存 → N维索引: index[d] = (linear / stride[d]) % shape[d]\n";

    return 0;
}
