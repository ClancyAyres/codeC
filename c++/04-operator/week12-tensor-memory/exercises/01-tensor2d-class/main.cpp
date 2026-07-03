// Exercise 01: Tensor2D Class — 创建/索引/打印
// TODO: 实现一个支持 row-major stride 的 2D 张量类

#include <iostream>
#include <vector>
#include <memory>
#include <cstdint>
#include <cassert>
#include <iomanip>
#include <stdexcept>

// TODO: 实现 Tensor2D 类
// 要求:
//   1. 构造函数: Tensor2D(int64_t rows, int64_t cols)
//      - 分配 rows*cols 个元素的内存
//      - 计算 row-major strides (cols, 1)
//   2. at(i, j): 读写访问元素
//      - 公式: linear = offset + i*stride0 + j*stride1
//      - 越界检查 (输出 std::cerr + return 或 throw)
//   3. shape(): 返回 {rows, cols}
//   4. strides(): 返回 {stride0, stride1}
//   5. print(label): 打印张量内容
//      - 格式: 等宽、行列清晰
//   6. data(): 返回内部数据指针
//   7. size(): 返回 rows() * cols()

template<typename T>
class Tensor2D {
public:
    // TODO: 添加构造函数
    // Tensor2D(int64_t rows, int64_t cols);

    // TODO: 添加元素访问 (读写)
    //       const 版本用于只读访问
    // T& at(int64_t i, int64_t j);
    // const T& at(int64_t i, int64_t j) const;

    // TODO: 添加 shape/strides 查询
    // int64_t rows() const;
    // int64_t cols() const;
    // std::vector<int64_t> shape() const;
    // std::vector<int64_t> strides() const;

    // TODO: 添加 data(), size()
    // T* data();
    // const T* data() const;
    // int64_t size() const;

    // TODO: 添加 print(label)
    // void print(const std::string& label) const;

private:
    // TODO: 添加成员变量
    // int64_t rows_, cols_;
    // int64_t stride0_, stride1_;
    // std::shared_ptr<T[]> storage_;
    // T* data_;
};

// ── main（验证实现） ────────────────────────────────────

int main() {
    std::cout << "========== Tensor2D 类练习 ==========\n\n";

    // TODO: 创建 3×4 张量并填充
    // Tensor2D<int> tensor(3, 4);
    // 填充: tensor(i,j) = i*10 + j

    // TODO: 打印形状和 strides
    // std::cout << "shape: (" << rows << ", " << cols << ")\n";
    // std::cout << "strides: (" << stride0 << ", " << stride1 << ")\n";

    // TODO: 打印张量内容
    // tensor.print("3×4 张量");

    // TODO: 测试越界访问
    // tensor.at(100, 100);  // 应输出错误信息

    // TODO: 测试 data() 返回的数据是否 row-major 连续
    // 验证 data()[i*cols + j] == tensor.at(i,j)

    std::cout << "\nTODO: 实现 Tensor2D 类后解除注释\n";
    std::cout << "验证点:\n";
    std::cout << "  1. at(i,j) 正确映射到 row-major 线性索引\n";
    std::cout << "  2. shape/strides 正确\n";
    std::cout << "  3. print() 输出清晰\n";
    std::cout << "  4. 越界处理得当\n";
    std::cout << "  5. data() 指针可直接用于 row-major 线性索引\n";

    return 0;
}
