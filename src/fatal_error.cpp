#include "fatal_error.h"

#include <cstdlib>
#include <iostream>

namespace riscvsim
{

[[noreturn]] void fatal_error(std::string_view what, uint32_t pc)
{
    std::cout << "error: " << what << " at pc=" << pc << "\n";
    std::exit(1);
}

[[noreturn]] void fatal_error_mem(std::string_view what, uint32_t pc,
                                  uint32_t addr)
{
    std::cout << "error: " << what << " out of bounds at addr=0x" << addr
              << ", pc=" << pc << "\n";
    std::exit(1);
}

};  // namespace riscvsim