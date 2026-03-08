#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <ALU.h>
#include <Memory.h>

class CPU {
public:
    CPU(Memory *mem);
    std::optional<uint32_t> read_reg(const uint32_t index) const;
    void write_reg(const uint32_t index, const uint32_t val);
    void run();

private:
    std::array<uint32_t, 32> registers_{};
    uint32_t pc_ = 0x0;
    ALU alu_;
    Memory *mem_;
};