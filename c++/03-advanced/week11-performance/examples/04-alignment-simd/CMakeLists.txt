set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    add_executable(example-04-alignment-simd main.cpp)
else()
    add_executable(example-04-alignment-simd main.cpp)
endif()
