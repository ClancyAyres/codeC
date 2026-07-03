#include "autograd.hpp"

namespace micro {

template class Var<float, 1>;
template class Var<float, 2>;
template class Var<double, 1>;
template class Var<double, 2>;

template std::shared_ptr<Var<float, 1>> varAdd(const std::shared_ptr<Var<float, 1>>&, const std::shared_ptr<Var<float, 1>>&);
template std::shared_ptr<Var<float, 2>> varAdd(const std::shared_ptr<Var<float, 2>>&, const std::shared_ptr<Var<float, 2>>&);
template std::shared_ptr<Var<double, 1>> varAdd(const std::shared_ptr<Var<double, 1>>&, const std::shared_ptr<Var<double, 1>>&);
template std::shared_ptr<Var<double, 2>> varAdd(const std::shared_ptr<Var<double, 2>>&, const std::shared_ptr<Var<double, 2>>&);

template std::shared_ptr<Var<float, 1>> varMul(const std::shared_ptr<Var<float, 1>>&, const std::shared_ptr<Var<float, 1>>&);
template std::shared_ptr<Var<float, 2>> varMul(const std::shared_ptr<Var<float, 2>>&, const std::shared_ptr<Var<float, 2>>&);
template std::shared_ptr<Var<double, 1>> varMul(const std::shared_ptr<Var<double, 1>>&, const std::shared_ptr<Var<double, 1>>&);
template std::shared_ptr<Var<double, 2>> varMul(const std::shared_ptr<Var<double, 2>>&, const std::shared_ptr<Var<double, 2>>&);

template std::shared_ptr<Var<float, 1>> varRelu(const std::shared_ptr<Var<float, 1>>&);
template std::shared_ptr<Var<float, 2>> varRelu(const std::shared_ptr<Var<float, 2>>&);
template std::shared_ptr<Var<double, 1>> varRelu(const std::shared_ptr<Var<double, 1>>&);
template std::shared_ptr<Var<double, 2>> varRelu(const std::shared_ptr<Var<double, 2>>&);

template std::shared_ptr<Var<float, 2>> varMatmul(const std::shared_ptr<Var<float, 2>>&, const std::shared_ptr<Var<float, 2>>&);
template std::shared_ptr<Var<double, 2>> varMatmul(const std::shared_ptr<Var<double, 2>>&, const std::shared_ptr<Var<double, 2>>&);

} // namespace micro
