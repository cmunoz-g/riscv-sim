#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include "Memory.h"
#include "Instruction.h"
#include "bit_manipulation.h"

//todo: move or keep here?
enum class BranchFunct3 {
    BEQ = 0b000,
    BNE = 0b001,
    BLT = 0b100,
    BGE = 0b101,
    BLTU = 0b110,
    BGEU = 0b111
};

enum class LoadFunct3 {
    LB = 0b000,
    LH = 0b001,
    LW = 0b010,
    LBU = 0b100,
    LHU = 0b101
};

enum class StoreFunct3 {
    SB = 0b000,
    SH = 0b001,
    SW = 0b010
};

enum class OpImmFunct3 {
    ADDI = 0b000,
    SLTI = 0b010,
    SLTIU = 0b011,
    XORI = 0b100,
    ORI = 0b110,
    ANDI = 0b111,
    SLLI = 0b001,
    SRLI_SRAI = 0b101
};

enum class OpFunct3 {
    ADD_SUB = 0b000,
    SLL = 0b001,
    SLT = 0b010,
    SLTU = 0b011,
    XOR = 0b100,
    SRL_SRA = 0b101,
    OR = 0b110,
    AND = 0b111
};

class CPU {
public:
    CPU(Memory *mem);
    uint32_t read_reg(const uint32_t index) const;
    bool write_reg(const uint32_t index, const uint32_t val);
    void run();
    
private:
    bool execute_lui(const Instruction &inst);
    bool execute_auipc(const Instruction &inst);
    bool execute_jal(const Instruction &inst);
    bool execute_jalr(const Instruction &inst);
    bool execute_branch(const Instruction &inst);
    bool execute_load(const Instruction &inst);
    bool execute_store(const Instruction &inst);
    bool execute_op_imm(const Instruction &inst);
    bool execute_op(const Instruction &inst);
    bool execute_sys(const Instruction &inst);
    bool execute(const Instruction &inst);

    std::array<uint32_t, 32> registers_{};
    uint32_t pc_ = 0x0;
    Memory *mem_;
};