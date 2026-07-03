cmake_minimum_required(VERSION 3.20)
project(phase1-foundation LANGUAGES CXX)

add_subdirectory(week1-cpp-basics/examples/01-hello-project)
add_subdirectory(week1-cpp-basics/examples/02-type-limits)
add_subdirectory(week1-cpp-basics/examples/03-namespace-demo)
add_subdirectory(week1-cpp-basics/examples/04-header-guard)

add_subdirectory(week2-pointers-memory/examples/01-stack-vs-heap)
add_subdirectory(week2-pointers-memory/examples/02-swap-versions)
add_subdirectory(week2-pointers-memory/examples/03-array-reverse)
add_subdirectory(week2-pointers-memory/examples/04-const-positions)

add_subdirectory(week3-raii-smartptr/examples/01-raii-file)
add_subdirectory(week3-raii-smartptr/examples/02-smartptr-factory)
add_subdirectory(week3-raii-smartptr/examples/03-weakptr-cycle)
add_subdirectory(week3-raii-smartptr/examples/04-rule-of-five)
