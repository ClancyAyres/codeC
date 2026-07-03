cmake_minimum_required(VERSION 3.20)
project(week14-capstone LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "Build tests with Google Test" ON)

add_library(micro_tensor STATIC
    src/tensor.cpp
    src/operators.cpp
    src/autograd.cpp
)

target_include_directories(micro_tensor PUBLIC include)

add_subdirectory(examples)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
