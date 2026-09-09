#include "CPU.h"

#include <cassert>
#include <iostream>
#include <memory>

#include "Instruction.h"
#include "fatal_error.h"

namespace riscvsim
{

CPU::CPU(const uint8_t* memory_data, std::size_t memory_size)
    : memory_(memory_data, memory_size)
{}

[[nodiscard]] uint32_t CPU::read_reg(uint8_t index) const noexcept
{
    assert(index < 32);
    return registers_[index];
}

void CPU::write_reg(uint8_t index, uint32_t value) noexcept
{
    assert(index < 32);
    if (index > 0)  // Silently ignores write requests to 0x0
    {
        registers_[index] = value;
    }
}

void CPU::execute_lui(const Instruction& instruction) noexcept
{
    const uint32_t value{sll(instruction.imm, 12)};
    write_reg(instruction.rd, value);
}

void CPU::execute_auipc(const Instruction& instruction) noexcept
{
    const uint32_t offset{sll(instruction.imm, 12)};
    write_reg(instruction.rd, pc_ + offset);
}

void CPU::execute_jal(const Instruction& instruction) noexcept
{
    const uint32_t return_addr{pc_ + 4};
    branched_ = true;
    branch_target_ = pc_ + instruction.imm;
    write_reg(instruction.rd, return_addr);
}

void CPU::execute_jalr(const Instruction& instruction) noexcept
{
    const uint32_t return_addr{pc_ + 4};
    branched_ = true;
    branch_target_ = (read_reg(instruction.rs1) + instruction.imm) & ~1;
    write_reg(instruction.rd, return_addr);
}

void CPU::execute_branch(const Instruction& instruction)
{
    const uint32_t rs1_value{read_reg(instruction.rs1)};
    const uint32_t rs2_value{read_reg(instruction.rs2)};

    switch (static_cast<BranchFunct3>(instruction.funct3))
    {
        case BranchFunct3::BEQ:
            branched_ = (rs1_value == rs2_value);
            break;
        case BranchFunct3::BGE:
            branched_ = (static_cast<int32_t>(rs1_value) >=
                         static_cast<int32_t>(rs2_value));
            break;
        case BranchFunct3::BGEU:
            branched_ = (rs1_value >= rs2_value);
            break;
        case BranchFunct3::BLT:
            branched_ = (static_cast<int32_t>(rs1_value) <
                         static_cast<int32_t>(rs2_value));
            break;
        case BranchFunct3::BLTU:
            branched_ = (rs1_value < rs2_value);
            break;
        case BranchFunct3::BNE:
            branched_ = rs1_value != rs2_value;
            break;
        default:
            fatal_error("invalid funct3 for BRANCH instruction", pc_);
    }

    if (branched_)
    {
        branch_target_ = pc_ + instruction.imm;
    }
}

void CPU::execute_load(const Instruction& instruction)
{
    const uint32_t addr{read_reg(instruction.rs1) + instruction.imm};
    uint32_t value{};
    bool read_ok{false};

    switch (static_cast<LoadFunct3>(instruction.funct3))
    {
        case LoadFunct3::LB:
        {
            uint8_t tmp{};
            read_ok = memory_.read8(addr, tmp);
            value = sign_extend(tmp, 8);
            break;
        }
        case LoadFunct3::LBU:
        {
            uint8_t tmp{};
            read_ok = memory_.read8(addr, tmp);
            value = tmp;
            break;
        }
        case LoadFunct3::LH:
        {
            uint16_t tmp{};
            read_ok = memory_.read16(addr, tmp);
            value = sign_extend(tmp, 16);
            break;
        }
        case LoadFunct3::LHU:
        {
            uint16_t tmp{};
            read_ok = memory_.read16(addr, tmp);
            value = tmp;
            break;
        }
        case LoadFunct3::LW:
            read_ok = memory_.read32(addr, value);
            break;
        default:
            fatal_error("invalid funct3 for LOAD instruction", pc_);
    }
    if (!read_ok)
    {
        fatal_error_mem("read", pc_, addr);
    }
    write_reg(instruction.rd, value);
}

void CPU::execute_store(const Instruction& instruction)
{
    const uint32_t addr{read_reg(instruction.rs1) + instruction.imm};
    const uint32_t rs2_value{read_reg(instruction.rs2)};
    bool write_ok{false};

    switch (static_cast<StoreFunct3>(instruction.funct3))
    {
        case StoreFunct3::SB:
            write_ok = memory_.write8(addr, extract_bits(rs2_value, 7, 0));
            break;
        case StoreFunct3::SH:
            write_ok = memory_.write16(addr, extract_bits(rs2_value, 15, 0));
            break;
        case StoreFunct3::SW:
            write_ok = memory_.write32(addr, rs2_value);
            break;
        default:
            fatal_error("invalid funct3 for STORE instruction", pc_);
    }
    if (!write_ok)
    {
        fatal_error_mem("write", pc_, addr);
    }
}

void CPU::execute_op_imm(const Instruction& instruction)
{
    const uint32_t rs1_value{read_reg(instruction.rs1)};
    uint32_t value_to_write{};

    switch (static_cast<OpImmFunct3>(instruction.funct3))
    {
        case OpImmFunct3::ADDI:
            value_to_write = rs1_value + instruction.imm;
            break;
        case OpImmFunct3::SLTI:
            value_to_write = (static_cast<int32_t>(rs1_value) <
                              static_cast<int32_t>(instruction.imm));
            break;
        case OpImmFunct3::SLTIU:
            value_to_write = (rs1_value < instruction.imm);
            break;
        case OpImmFunct3::ANDI:
            value_to_write = (rs1_value & instruction.imm);
            break;
        case OpImmFunct3::ORI:
            value_to_write = (rs1_value | instruction.imm);
            break;
        case OpImmFunct3::XORI:
            value_to_write = (rs1_value ^ instruction.imm);
            break;
        case OpImmFunct3::SLLI:
        {
            const uint32_t shamt{extract_bits(instruction.imm, 4, 0)};
            value_to_write = sll(rs1_value, shamt);
            break;
        }
        case OpImmFunct3::SRLI_SRAI:
        {
            const uint32_t shamt{extract_bits(instruction.imm, 4, 0)};
            const uint32_t funct7{extract_bits(instruction.imm, 30, 30)};
            const uint32_t shifted_value{(funct7 == 0) ? srl(rs1_value, shamt)
                                                       : sra(rs1_value, shamt)};
            value_to_write = shifted_value;
            break;
        }
        default:
            fatal_error("invalid funct3 for OP_IMM instruction", pc_);
    }
    write_reg(instruction.rd, value_to_write);
}

void CPU::execute_op(const Instruction& instruction)
{
    const uint32_t rs1_value{read_reg(instruction.rs1)};
    const uint32_t rs2_value{read_reg(instruction.rs2)};
    uint32_t value_to_write{};

    switch (static_cast<OpFunct3>(instruction.funct3))
    {
        case OpFunct3::ADD_SUB:
        {
            const uint32_t operation{extract_bits(instruction.funct7, 5, 5)};
            value_to_write = (operation == 0) ? (rs1_value + rs2_value)
                                              : (rs1_value - rs2_value);
            break;
        }
        case OpFunct3::SLL:
            value_to_write = sll(rs1_value, rs2_value);
            break;
        case OpFunct3::SLT:
            value_to_write = (static_cast<int32_t>(rs1_value) <
                              static_cast<int32_t>(rs2_value));
            break;
        case OpFunct3::SLTU:
            value_to_write = (rs1_value < rs2_value);
            break;
        case OpFunct3::SRL_SRA:
        {
            const uint32_t operation{extract_bits(instruction.funct7, 5, 5)};
            value_to_write = (operation == 0) ? srl(rs1_value, rs2_value)
                                              : sra(rs1_value, rs2_value);
            break;
        }
        case OpFunct3::XOR:
            value_to_write = rs1_value ^ rs2_value;
            break;
        case OpFunct3::OR:
            value_to_write = rs1_value | rs2_value;
            break;
        case OpFunct3::AND:
            value_to_write = rs1_value & rs2_value;
            break;
        default:
            fatal_error("invalid funct3 for OP instruction", pc_);
    }
    write_reg(instruction.rd, value_to_write);
}

void CPU::execute_sys(const Instruction& instruction)
{
    switch (static_cast<EcallCodes>(registers_[a7]))
    {
        case EcallCodes::PRINT_INTEGER:
            std::cout << static_cast<int32_t>(registers_[a0]);
            break;
        case EcallCodes::PRINT_STRING:
            ecall_print_string();
            break;
        case EcallCodes::EXIT:
            running_ = false;
            break;
        case EcallCodes::PRINT_CHARACTER:
        {
            std::cout << static_cast<char>(registers_[a0]);
            break;
        }
        case EcallCodes::EXIT_WITH_CODE:
        {
            exit_code_ = registers_[a0];
            running_ = false;
            break;
        }
        default:
            fatal_error("unknown syscall", pc_);
    }
}

void CPU::ecall_print_string() const
{
    uint32_t addr{registers_[a0]};
    bool read_ok{false};
    uint8_t value{};

    while ((read_ok = memory_.read8(addr, value)) && value != 0)
    {
        std::cout << static_cast<char>(value);
        addr++;
    }
    if (!read_ok)
    {
        fatal_error_mem("read", pc_, addr);
    }
}

void CPU::execute(const Instruction& instruction)
{
    switch (instruction.opcode)
    {
        case Opcode::LUI:
            execute_lui(instruction);
            break;
        case Opcode::AUIPC:
            execute_auipc(instruction);
            break;
        case Opcode::JAL:
            execute_jal(instruction);
            break;
        case Opcode::JALR:
            execute_jalr(instruction);
            break;
        case Opcode::BRANCH:
            execute_branch(instruction);
            break;
        case Opcode::LOAD:
            execute_load(instruction);
            break;
        case Opcode::STORE:
            execute_store(instruction);
            break;
        case Opcode::OP_IMM:
            execute_op_imm(instruction);
            break;
        case Opcode::OP:
            execute_op(instruction);
            break;
        case Opcode::SYS:
            execute_sys(instruction);
            break;
        default:
            fatal_error("unknown instruction", pc_);
    }
}

[[nodiscard]] uint32_t CPU::exit_code() const
{
    return exit_code_;
}

void CPU::run()
{
    for (;;)
    {
        uint32_t instruction_word{};
        if (!memory_.read32(pc_, instruction_word))
        {
            fatal_error("read out of bounds", pc_);
        }
        const auto instruction = decode(instruction_word);
        if (!instruction)
        {
            fatal_error("opcode decode error", pc_);
        }
        execute(instruction.value());
        if (!running_)
        {
            break;
        }
        pc_ = branched_ ? branch_target_ : pc_ + 4;
        branched_ = false;
    }
}

};  // namespace riscvsim