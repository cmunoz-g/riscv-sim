cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(riscv-sim VERSION 1.0)
add_executable(riscvsim src/main.cc src/Memory.cc src/Instruction.cc src/bit_manipulation.cc src/CPU.cc)
include_directories(inc)