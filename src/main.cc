#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <filesystem>
#include "CPU.h"
#include "Memory.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "usage: riscv-sim <path to .bin file>" << std::endl;
        return EXIT_FAILURE;
    }
    if (std::strcmp(argv[1] + std::strlen(argv[1]) - 4, ".bin")) {
        std::cout << "error: file must be .bin" << std::endl;
        return EXIT_FAILURE;
    }
    
    auto size = std::filesystem::file_size(argv[1]);
    uint8_t buf[size];
    std::fstream file;
    file.open(argv[1], std::ios::in | std::ios::binary);
    if (!file) {
        std::cout << "error: could not open " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }
    file.read(reinterpret_cast<char *>(buf), size);
    file.close();

    Memory mem(buf, static_cast<std::size_t>(size)); // inits Memory, copies .bin at base address
    //CPU cpu(mem); // inits CPU registers, holds pointer to memory 

    //cpu.run(); // how are halt/errors handled ? how is logging handled ?

    return EXIT_SUCCESS;
}