cmake_minimum_required(VERSION 3.20)
project(cpp-learning VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_subdirectory(phase1-foundation)
add_subdirectory(phase2-core)
add_subdirectory(phase3-advanced)
add_subdirectory(phase4-operator)
