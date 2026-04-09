#pragma once
#include <array>
#include <cstdint>
#include <optional>
#include "Memory.h"
#include "Instruction.h"
#include "bit_manipulation.h"

class CPU {
public:
    CPU(const uint8_t *data, const std::size_t size);
    uint32_t read_reg(const uint32_t index) const;
    void write_reg(const uint32_t index, const uint32_t val);
    void run();
    uint32_t exit_code();
    
    static constexpr int a7 = 17;
    static constexpr int a0 = 10;

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
    bool ecall_print_string();
    bool execute(const Instruction &inst);

    std::array<uint32_t, 32> registers_{};
    Memory mem_;
    uint32_t pc_ = 0x0;
    bool running_ = true;
    uint8_t exit_code_ = 0;
    bool branched_ = false;
    uint32_t branch_target_ = 0;
    uint32_t err_addr_ = 0;
};