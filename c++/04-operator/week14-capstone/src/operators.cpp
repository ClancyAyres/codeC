#include "operators.hpp"

namespace micro {

template Tensor<float, 1> add(const Tensor<float, 1>&, const Tensor<float, 1>&);
template Tensor<float, 2> add(const Tensor<float, 2>&, const Tensor<float, 2>&);
template Tensor<double, 1> add(const Tensor<double, 1>&, const Tensor<double, 1>&);
template Tensor<double, 2> add(const Tensor<double, 2>&, const Tensor<double, 2>&);

template Tensor<float, 1> sub(const Tensor<float, 1>&, const Tensor<float, 1>&);
template Tensor<float, 2> sub(const Tensor<float, 2>&, const Tensor<float, 2>&);
template Tensor<double, 1> sub(const Tensor<double, 1>&, const Tensor<double, 1>&);
template Tensor<double, 2> sub(const Tensor<double, 2>&, const Tensor<double, 2>&);

template Tensor<float, 1> mul(const Tensor<float, 1>&, const Tensor<float, 1>&);
template Tensor<float, 2> mul(const Tensor<float, 2>&, const Tensor<float, 2>&);
template Tensor<double, 1> mul(const Tensor<double, 1>&, const Tensor<double, 1>&);
template Tensor<double, 2> mul(const Tensor<double, 2>&, const Tensor<double, 2>&);

template Tensor<float, 1> div(const Tensor<float, 1>&, const Tensor<float, 1>&);
template Tensor<float, 2> div(const Tensor<float, 2>&, const Tensor<float, 2>&);
template Tensor<double, 1> div(const Tensor<double, 1>&, const Tensor<double, 1>&);
template Tensor<double, 2> div(const Tensor<double, 2>&, const Tensor<double, 2>&);

template Tensor<float, 1> relu(const Tensor<float, 1>&);
template Tensor<float, 2> relu(const Tensor<float, 2>&);
template Tensor<double, 1> relu(const Tensor<double, 1>&);
template Tensor<double, 2> relu(const Tensor<double, 2>&);

template Tensor<float, 1> sigmoid(const Tensor<float, 1>&);
template Tensor<float, 2> sigmoid(const Tensor<float, 2>&);
template Tensor<double, 1> sigmoid(const Tensor<double, 1>&);
template Tensor<double, 2> sigmoid(const Tensor<double, 2>&);

template Tensor<float, 2> matmul(const Tensor<float, 2>&, const Tensor<float, 2>&);
template Tensor<double, 2> matmul(const Tensor<double, 2>&, const Tensor<double, 2>&);

template Tensor<float, 1> softmax(const Tensor<float, 1>&);
template Tensor<float, 2> softmax(const Tensor<float, 2>&);
template Tensor<double, 1> softmax(const Tensor<double, 1>&);
template Tensor<double, 2> softmax(const Tensor<double, 2>&);

template float sum(const Tensor<float, 1>&);
template float sum(const Tensor<float, 2>&);
template double sum(const Tensor<double, 1>&);
template double sum(const Tensor<double, 2>&);

template float mean(const Tensor<float, 1>&);
template float mean(const Tensor<float, 2>&);
template double mean(const Tensor<double, 1>&);
template double mean(const Tensor<double, 2>&);

template float max(const Tensor<float, 1>&);
template float max(const Tensor<float, 2>&);
template double max(const Tensor<double, 1>&);
template double max(const Tensor<double, 2>&);

template Tensor<float, 1> neg(const Tensor<float, 1>&);
template Tensor<float, 2> neg(const Tensor<float, 2>&);
template Tensor<double, 1> neg(const Tensor<double, 1>&);
template Tensor<double, 2> neg(const Tensor<double, 2>&);

} // namespace micro
