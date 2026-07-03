cmake_minimum_required(VERSION 3.20)
project(phase3-advanced LANGUAGES CXX)

add_subdirectory(week9-concurrency/examples/01-thread-sum)
add_subdirectory(week9-concurrency/examples/02-producer-consumer)
add_subdirectory(week9-concurrency/examples/03-async-tasks)
add_subdirectory(week9-concurrency/examples/04-thread-safe-singleton)

add_subdirectory(week11-performance/examples/01-aos-vs-soa)
add_subdirectory(week11-performance/examples/02-loop-unroll)
add_subdirectory(week11-performance/examples/03-google-benchmark)
add_subdirectory(week11-performance/examples/04-alignment-simd)

add_subdirectory(week10-exceptions/examples/01-exception-safe-stack)
add_subdirectory(week10-exceptions/examples/02-optional-pattern)
add_subdirectory(week10-exceptions/examples/03-variant-union)
add_subdirectory(week10-exceptions/examples/04-ctor-failure)
