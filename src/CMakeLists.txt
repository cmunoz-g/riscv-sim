add_library(rvlib
    bit_manipulation.cpp
    CPU.cpp
    fatal_error.cpp
    Instruction.cpp
    load_file.cpp
    Memory.cpp
)

target_include_directories(rvlib PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(riscvsim main.cpp)
target_link_libraries(riscvsim PRIVATE rvlib)