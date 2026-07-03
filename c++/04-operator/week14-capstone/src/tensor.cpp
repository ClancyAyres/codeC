#include "tensor.hpp"

namespace micro {

template class Tensor<float, 1>;
template class Tensor<float, 2>;
template class Tensor<float, 3>;
template class Tensor<double, 1>;
template class Tensor<double, 2>;
template class Tensor<double, 3>;

} // namespace micro
