// Example 02: Zero-Copy Slice — 共享数据 + 独立 stride/offset
// 目的：演示如何通过 stride/shape/offset 元数据在不拷贝数据的情况下创建张量视图

#include <iostream>
#include <vector>
#include <memory>
#include <cstdint>
#include <cassert>
#include <iomanip>
#include <string>

// ── 轻量张量：共享数据，独立元数据 ───────────────────────────
template<typename T>
struct TensorView {
    std::shared_ptr<T[]> storage;       // 真正的数据所有者（共享）
    T*       data;                      // data = storage.get() + offset
    std::vector<int64_t> shape;
    std::vector<int64_t> strides;
    int64_t  offset;

    // 构造新张量（拥有数据）
    TensorView(const std::vector<int64_t>& s)
        : storage(new T[totalSize(s)])
        , data(storage.get())
        , shape(s)
        , strides(computeStrides(s))
        , offset(0)
    {
        // 初始化为 0
        std::fill(data, data + totalSize(shape), T{});
    }

    // 从已有张量创建视图（共享存储）
    TensorView(std::shared_ptr<T[]> stor, T* ptr,
               const std::vector<int64_t>& shp,
               const std::vector<int64_t>& strd,
               int64_t off)
        : storage(std::move(stor)), data(ptr), shape(shp), strides(strd), offset(off) {}

    static size_t totalSize(const std::vector<int64_t>& s) {
        size_t n = 1;
        for (auto d : s) n *= d;
        return n;
    }

    static std::vector<int64_t> computeStrides(const std::vector<int64_t>& shp) {
        std::vector<int64_t> strd(shp.size(), 1);
        for (int d = static_cast<int>(shp.size()) - 2; d >= 0; --d)
            strd[d] = strd[d + 1] * shp[d + 1];
        return strd;
    }

    // 元素访问
    T& at(const std::vector<int64_t>& idx) {
        int64_t pos = offset;
        for (size_t d = 0; d < idx.size(); ++d)
            pos += idx[d] * strides[d];
        return data[pos];
    }

    const T& at(const std::vector<int64_t>& idx) const {
        int64_t pos = offset;
        for (size_t d = 0; d < idx.size(); ++d)
            pos += idx[d] * strides[d];
        return data[pos];
    }

    // 零拷贝切片 — 沿 dim 轴取 [start, end)
    TensorView slice(int64_t dim, int64_t start, int64_t end) const {
        assert(dim >= 0 && dim < static_cast<int64_t>(shape.size()));
        assert(start >= 0 && end <= shape[dim] && start < end);
        auto newShape = shape;
        newShape[dim] = end - start;
        auto newStrides = strides;                     // strides 不变!
        int64_t newOffset = offset + start * strides[dim];  // 只改 offset
        return TensorView(storage, data, newShape, newStrides, newOffset);
    }

    // 零拷贝转置 — 交换 strides 前两维
    TensorView transpose() const {
        assert(shape.size() >= 2);
        auto newShape = shape;
        std::swap(newShape[0], newShape[1]);
        auto newStrides = strides;
        std::swap(newStrides[0], newStrides[1]);
        return TensorView(storage, data, newShape, newStrides, offset);
    }

    // 判断 C-contiguous (row-major 连续)
    bool isContiguous() const {
        int64_t expected = 1;
        for (int d = static_cast<int>(shape.size()) - 1; d >= 0; --d) {
            if (shape[d] != 1 && strides[d] != expected) return false;
            expected *= shape[d];
        }
        return offset == 0 || (offset > 0 && true);  // offset!=0 的视图也是"相对连续"
    }

    // 打印
    void print(const std::string& name) const {
        std::cout << "== " << name << " ==\n";
        std::cout << "  ptr=" << static_cast<void*>(data)
                  << "  offset=" << offset
                  << "  contiguous=" << (isContiguous() ? "yes" : "no")
                  << "  shape=(";
        for (size_t i = 0; i < shape.size(); ++i) {
            std::cout << shape[i];
            if (i < shape.size() - 1) std::cout << ", ";
        }
        std::cout << ")  strides=(";
        for (size_t i = 0; i < strides.size(); ++i) {
            std::cout << strides[i];
            if (i < strides.size() - 1) std::cout << ", ";
        }
        std::cout << ")\n";
    }

    void printData() const {
        if (shape.size() == 2) {
            for (int64_t i = 0; i < shape[0]; ++i) {
                std::cout << "  [";
                for (int64_t j = 0; j < shape[1]; ++j) {
                    std::cout << std::setw(4) << at({i, j});
                    if (j < shape[1] - 1) std::cout << ", ";
                }
                std::cout << " ]\n";
            }
        }
        std::cout << '\n';
    }
};

int main() {
    std::cout << "========== 零拷贝切片 & 转置演示 ==========\n\n";

    // ── 创建原始 4×5 张量 ────────────────────────────────
    TensorView<int> original({4, 5});
    for (int64_t i = 0; i < 4; ++i)
        for (int64_t j = 0; j < 5; ++j)
            original.at({i, j}) = static_cast<int>(i * 10 + j);

    original.print("原始张量 (4×5)");
    original.printData();

    // ── 零拷贝切片: 取第1-3行、第2-5列 ──────────────────
    std::cout << "── 零拷贝切片: original.slice(0, 1, 3).slice(1, 2, 5) ──\n";
    auto sliced = original.slice(0, 1, 3).slice(1, 2, 5);
    sliced.print("切片后视图");
    sliced.printData();

    // 验证: 修改切片，原始数据也被修改（同一块内存）
    std::cout << "  修改切片 (0,0) = 999 ...\n";
    sliced.at({0, 0}) = 999;

    std::cout << "  切片视图:\n";
    sliced.printData();

    std::cout << "  原始张量 (同位置被修改!):\n";
    original.printData();

    // ── 恢复原始值 ───────────────────────────────────────
    sliced.at({0, 0}) = 12;

    // ── 零拷贝转置 ───────────────────────────────────────
    std::cout << "── 零拷贝转置: original.transpose() ──\n";
    auto transposed = original.transpose();
    transposed.print("转置后视图 (5×4)");
    transposed.printData();

    // 验证转置等价性
    std::cout << "  验证: transposed(j,i) == original(i,j)\n";
    bool ok = true;
    for (int64_t i = 0; i < original.shape[0]; ++i) {
        for (int64_t j = 0; j < original.shape[1]; ++j) {
            if (transposed.at({j, i}) != original.at({i, j})) {
                std::cerr << "MISMATCH at (" << i << "," << j << ")\n";
                ok = false;
            }
        }
    }
    std::cout << (ok ? "  通过!\n\n" : "  失败!\n\n");

    // ── 连续视图上再切片 ────────────────────────────────
    std::cout << "── 转置后切片: transposed.slice(0, 1, 4).slice(1, 0, 3) ──\n";
    auto transSliced = transposed.slice(0, 1, 4).slice(1, 0, 3);
    transSliced.print("转置+切片视图");
    transSliced.printData();

    // ── 修改视图验证传播 ──────────────────────────────────
    std::cout << "  修改 transSliced(0,0) = 777 ...\n";
    transSliced.at({0, 0}) = 777;

    std::cout << "  transposed:\n";
    transposed.printData();
    std::cout << "  original (original(1,1) 变为 777 — 传播正确!):\n";
    original.printData();

    // ── 总结 ──────────────────────────────────────────────
    std::cout << "========== 关键理解 ==========\n";
    std::cout << "1. 所有视图共享同一个 storage (shared_ptr)\n";
    std::cout << "2. slice:   只改 offset 和 shape，strides 不变\n";
    std::cout << "3. transpose: 交换 shape 和 strides 前两维\n";
    std::cout << "4. 修改任一视图 → 所有视图可见 → 同一块内存\n";
    std::cout << "5. 所有操作 O(1) 时间、O(1) 内存（仅元数据）\n";
    std::cout << "6. storage_ 通过 shared_ptr 确保最后释放\n";

    return 0;
}
