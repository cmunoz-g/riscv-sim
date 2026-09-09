include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(riscvsim_tests
    test_bit_manipulation.cpp
    test_memory.cpp
    test_instruction.cpp
    test_CPU.cpp
)

set(PROGRAM_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/programs")
set(PROGRAM_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/programs")

target_compile_definitions(riscvsim_tests PRIVATE
    PROGRAM_BINARY_DIR="${PROGRAM_BINARY_DIR}"
)

file(MAKE_DIRECTORY "${PROGRAM_BINARY_DIR}")

file(GLOB PROGRAM_SOURCES CONFIGURE_DEPENDS
    "${PROGRAM_SOURCE_DIR}/*.s"
)

set(PROGRAM_BINS)

foreach(src ${PROGRAM_SOURCES})
    get_filename_component(name "${src}" NAME_WE)
    set(bin "${PROGRAM_BINARY_DIR}/${name}.bin")

    add_custom_command(
        OUTPUT "${bin}"
        COMMAND "${CMAKE_SOURCE_DIR}/tools/compile_bin.sh"
                "${src}"
                "${PROGRAM_BINARY_DIR}"
        DEPENDS
            "${src}"
            "${CMAKE_SOURCE_DIR}/tools/compile_bin.sh"   
    )

    list(APPEND PROGRAM_BINS "${bin}")
endforeach()

add_custom_target(test_programs DEPENDS ${PROGRAM_BINS})
add_dependencies(riscvsim_tests test_programs)

target_link_libraries(riscvsim_tests PRIVATE rvlib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(riscvsim_tests)