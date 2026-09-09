#include <cstdlib>
#include <cstring>
#include <iostream>

#include "CPU.h"
#include "Memory.h"
#include "load_file.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "usage: riscv-sim <path to .bin file>\n";
        std::exit(1);
    }
    if (std::strcmp(argv[1] + std::strlen(argv[1]) - 4, ".bin"))
    {
        std::cout << "error: file must be .bin\n";
        std::exit(1);
    }
    std::vector<uint8_t> buf;
    try
    {
        buf = riscvsim::load_file(argv[1]);
    }
    catch (const std::exception& e)
    {
        std::cout << "error: " << e.what() << "\n";
        std::exit(1);
    }
    riscvsim::CPU cpu(buf.data(), buf.size());
    cpu.run();
    return cpu.exit_code() & 0xFF;
}