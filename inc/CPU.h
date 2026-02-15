#pragma once

#include <array>
#include <cstdint>
#include <ALU.h>
#include <Memory.h>

class CPU {
public:

private:
    std::array<uint32_t, 32> registers_;
    uint32_t pc_;
    ALU alu_;
    Memory *mem_;
};