#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include "Instruction.h"
#include "Memory.h"
#include "bit_manipulation.h"

namespace riscvsim
{

inline constexpr int a0 = 10;  // arg 0
inline constexpr int a7 = 17;  // syscall number

class CPU
{
public:
    explicit CPU(const uint8_t* memory_data, std::size_t memory_size);
    CPU(const CPU& cpu) = delete;
    CPU& operator=(const CPU& cpu) = delete;

    // Reads from a register, 'index' must be < 32
    [[nodiscard]] uint32_t read_reg(uint8_t index) const noexcept;

    // Writes 'value' to a register, 'index' must be < 32
    void write_reg(uint8_t index, uint32_t value) noexcept;

    // CPU operation loop
    void run();

    // Returns exit code set by performing syscall EXIT_WITH_CODE (registers_[a7] == 17)
    [[nodiscard]] uint32_t exit_code() const;

private:
    // Routes the instruction to its associated execute function
    void execute(const Instruction& instruction);

    // Places the 32-bit U-immediate value into the destination register rd, filling in the lowest 12 bits with zeros
    void execute_lui(const Instruction& instruction) noexcept;

    // Forms a 32-bit offset from the U-immediate, filling in the lowest 12 bits with zeros,
    // adds this offset to the address of the AUIPC instruction, then places the result in register rd.
    void execute_auipc(const Instruction& instruction) noexcept;

    // Jumps to a new memory address, saves the return address.
    void execute_jal(const Instruction& instruction) noexcept;

    // Jumps to a target address obtained by adding the sign-extended 12-bit I-immediate to the register rs1,
    // then setting the least-significant bit of the result to zero. The address of the instruction following
    // the jump (pc+4) is written to register rd.
    void execute_jalr(const Instruction& instruction) noexcept;

    // Evaluates a condition and executes a jump to pc+imm if its met
    void execute_branch(const Instruction& instruction);

    // Performs a read from memory and places the data in a register
    void execute_load(const Instruction& instruction);

    // Writes the value contained in a register to memory
    void execute_store(const Instruction& instruction);

    // Performs an arithmetic or logical operation between a register and a 12-bit immediate value
    void execute_op_imm(const Instruction& instruction);

    // Performs an arithmetic or logical operation between two register values
    void execute_op(const Instruction& instruction);

    // Executes a syscall
    void execute_sys(const Instruction& instruction);

    // Prints the string located on the address saved in register a0
    void ecall_print_string() const;

    std::array<uint32_t, 32> registers_{};
    Memory memory_;
    uint32_t pc_ = 0x0;
    bool running_ = true;
    uint8_t exit_code_ = 0;
    bool branched_ =
        false;  // Indicates whether the branch operation was successful
    uint32_t branch_target_ =
        0;  // Saves the address to move to after a branch operation
};

};  // namespace riscvsim