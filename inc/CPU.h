#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include "ALU.h"
#include "Memory.h"
#include "Instruction.h"

//todo: move or keep here?
enum class BranchCond {
    BEQ = 0b000,
    BNE = 0b001,
    BLT = 0b100,
    BGE = 0b101,
    BLTU = 0b110,
    BGEU = 0b111
};

class CPU {
public:
    CPU(Memory *mem);
    std::optional<uint32_t> read_reg(const uint32_t index) const;
    bool write_reg(const uint32_t index, const uint32_t val);
    void run();
    
private:
    bool execute_lui(const Instruction &inst);
    bool execute_auipc(const Instruction &inst);
    bool execute_jal(const Instruction &inst);
    bool execute_jalr(const Instruction &inst);
    bool execute_branch(const Instruction &inst);

    std::array<uint32_t, 32> registers_{};
    uint32_t pc_ = 0x0;
    ALU alu_;
    Memory *mem_;
};