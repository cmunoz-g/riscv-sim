#pragma once

#include <cstdint>
#include <string_view>

namespace riscvsim
{

// Prints reason and program counter to stderr and exits program
[[noreturn]] void fatal_error(std::string_view what, uint32_t pc);

// Prints reason, program counter and offending address, and exits program
[[noreturn]] void fatal_error_mem(std::string_view what, uint32_t pc,
                                  uint32_t addr);

};  // namespace riscvsim