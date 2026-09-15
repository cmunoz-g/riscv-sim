cmake_minimum_required(VERSION 3.14)
project(riscv-sim VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)

option(RISCVSIM_TESTS "Build tests" ON)
if (RISCVSIM_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()