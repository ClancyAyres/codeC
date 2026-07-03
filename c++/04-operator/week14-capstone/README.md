# Week 14 Capstone — Micro Tensor Library

A micro tensor computation library with automatic differentiation, written in C++17.

## Features

- **Tensor**: `Tensor<T, Rank>` template class with compile-time rank, row-major storage, `std::shared_ptr<std::vector<T>>` backing, `operator()` indexing, zero-copy slice views (shared_ptr + stride/offset), and reshape
- **Operators**: element-wise add/sub/mul/div, relu, sigmoid, matmul (triple-loop ikj), softmax (numerically stable with subtract-max), reduce (sum/mean/max), neg
- **Autograd**: forward operations record a computation graph, backward propagation via `backward_fn` lambdas, topological sort for multi-hop graph reversal

## Building

```bash
mkdir build && cd build
cmake .. -DBUILD_TESTS=ON
cmake --build .
```

## Running Tests

```bash
cd build
ctest --output-on-failure
```

## Running the Demo

```bash
./build/examples/demo
```

## Library Structure

```
include/
  tensor.hpp     — Tensor<T, Rank> template class
  operators.hpp  — add/sub/mul/div/relu/sigmoid/matmul/softmax/sum/mean/max
  autograd.hpp   — Var<T,Rank> computation graph node / forward + backward
src/
  tensor.cpp     — explicit template instantiations
  operators.cpp  — explicit template instantiations
  autograd.cpp   — explicit template instantiations
tests/
  test_tensor.cpp    — creation, indexing, slice, reshape
  test_operators.cpp — each operator correctness
  test_autograd.cpp  — gradient verification
examples/
  demo.cpp — tensor creation, matmul+relu, autograd
```
