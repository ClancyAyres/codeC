include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)

FetchContent_MakeAvailable(googletest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_tensor test_tensor.cpp)
target_link_libraries(test_tensor PRIVATE micro_tensor gtest_main)

add_executable(test_operators test_operators.cpp)
target_link_libraries(test_operators PRIVATE micro_tensor gtest_main)

add_executable(test_autograd test_autograd.cpp)
target_link_libraries(test_autograd PRIVATE micro_tensor gtest_main)

add_test(NAME tensor     COMMAND test_tensor)
add_test(NAME operators  COMMAND test_operators)
add_test(NAME autograd   COMMAND test_autograd)
