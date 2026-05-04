#include "CPU.h"
#include "Instruction.h"
#include "log.h"
#include <cstdio> //todo: remove
#include <iostream>
#include <memory>
#include <cassert>

CPU::CPU(const uint8_t *mem_data, const std::size_t mem_size) : mem_(mem_data, mem_size) {}

// Read / Write
uint32_t CPU::read_reg(const uint32_t index) const {
    return registers_[index];
}

void CPU::write_reg(const uint32_t index, const uint32_t val) { 
    if (index > 0) { // Silently ignores write requests to 0x0
        registers_[index] = val;
    }
}

// Execution
bool CPU::execute_lui(const Instruction &inst) {
    // LUI places the 32-bit U-immediate value into the destination register rd, filling in the lowest 12 bits with zeros.
    uint32_t val = sll(inst.imm, 12);
    write_reg(inst.rd, val);
    return true;
}

bool CPU::execute_auipc(const Instruction &inst) {
    // AUIPC forms a 32-bit offset from the U-immediate, filling in the lowest 12 bits with zeros,
    // adds this offset to the address of the AUIPC instruction, then places the result in register rd.
    uint32_t offset = sll(inst.imm, 12);
    write_reg(inst.rd, pc_ + offset);
    return true;
}

bool CPU::execute_jal(const Instruction &inst) {
    uint32_t return_addr = pc_ + 4;
    branched_ = true;
    branch_target_ = pc_ + inst.imm;
    write_reg(inst.rd, return_addr);
    return true;
}

bool CPU::execute_jalr(const Instruction &inst) {
    // The target address is obtained by adding the sign-extended 12-bit I-immediate to the register rs1, then setting the least-significant bit
    // of the result to zero. The address of the instruction following the jump (pc+4) is written to register rd.
    uint32_t return_addr = pc_ + 4;
    branched_ = true;
    branch_target_ = (inst.rs1 + inst.imm) & ~1;
    write_reg(inst.rd, return_addr);
    return true;
}

bool CPU::execute_branch(const Instruction &inst) {
    uint32_t rs1_val = read_reg(inst.rs1);
    uint32_t rs2_val = read_reg(inst.rs2);
    
    switch (static_cast<BranchFunct3>(inst.funct3)) {
        case BranchFunct3::BEQ: branched_ = (rs1_val == rs2_val); break;
        case BranchFunct3::BGE: branched_ = (static_cast<int32_t>(rs1_val) >= static_cast<int32_t>(rs2_val)); break;
        case BranchFunct3::BGEU: branched_ = (rs1_val >= rs2_val); break;
        case BranchFunct3::BLT: branched_ = (static_cast<int32_t>(rs1_val) < static_cast<int32_t>(rs2_val)); break;
        case BranchFunct3::BLTU: branched_ = (rs1_val < rs2_val); break;
        case BranchFunct3::BNE: branched_ = rs1_val != rs2_val; break;
    }
    
    if (branched_) {
        branch_target_ = pc_ + inst.imm;
    }
    return true;
}

bool CPU::execute_load(const Instruction &inst) {
    uint32_t addr = read_reg(inst.rs1) + inst.imm;
    uint32_t val{};
    bool read_res = false;

    switch (static_cast<LoadFunct3>(inst.funct3)) {
        case LoadFunct3::LB: {
            uint8_t tmp{};
            read_res = mem_.read8(addr, tmp);
            val = sign_extend(tmp, 8);
            break;
        }
        case LoadFunct3::LBU: {
            uint8_t tmp{};
            read_res = mem_.read8(addr, tmp);
            val = tmp;
            break;
        }
        case LoadFunct3::LH: {
            uint16_t tmp{};
            read_res = mem_.read16(addr, tmp);
            val = sign_extend(tmp, 16);
            break;
        
        }
        case LoadFunct3::LHU: {
            uint16_t tmp{};
            read_res = mem_.read16(addr, tmp);
            val = tmp;
            break;
        }
        case LoadFunct3::LW: read_res = mem_.read32(addr, val); break;
    }
    if (read_res) {
        write_reg(inst.rd, val);
    }
    else {
        err_addr_ = addr;
    }
    return read_res;
}

bool CPU::execute_store(const Instruction &inst) {
    uint32_t addr = read_reg(inst.rs1) + inst.imm;
    uint32_t rs2_val = read_reg(inst.rs2);
    bool write_res = false;

    switch (static_cast<StoreFunct3>(inst.funct3)) {
        case StoreFunct3::SB:
            write_res = mem_.write8(addr, extract_bits(rs2_val, 7, 0));
            break;
        case StoreFunct3::SH:
            write_res = mem_.write16(addr, extract_bits(rs2_val, 15, 0));
            break;
        case StoreFunct3::SW:
            write_res = mem_.write32(addr, rs2_val);
            break;
    }
    if (!write_res) {
        err_addr_ = addr;
    }
    return write_res;
}

bool CPU::execute_op_imm(const Instruction &inst) {
    uint32_t rs1_val = read_reg(inst.rs1);
    uint32_t val_to_write = 0;

    switch (static_cast<OpImmFunct3>(inst.funct3)) {
        case OpImmFunct3::ADDI: val_to_write = rs1_val + inst.imm; break;
        case OpImmFunct3::SLTI: val_to_write = (static_cast<int32_t>(rs1_val) < static_cast<int32_t>(inst.imm)); break;
        case OpImmFunct3::SLTIU: val_to_write = (rs1_val < inst.imm); break;
        case OpImmFunct3::ANDI: val_to_write = (rs1_val & inst.imm); break;
        case OpImmFunct3::ORI: val_to_write = (rs1_val | inst.imm); break;
        case OpImmFunct3::XORI: val_to_write = (rs1_val ^ inst.imm); break;
        case OpImmFunct3::SLLI: {
            uint32_t shamt = extract_bits(inst.imm, 4, 0);
            val_to_write = sll(rs1_val, shamt);
            break;
        }
        case OpImmFunct3::SRLI_SRAI: {
            uint32_t shamt = extract_bits(inst.imm, 4, 0);
            uint32_t funct7 = extract_bits(inst.imm, 30, 30);
            uint32_t shifted_val = (funct7 == 0) ? srl(rs1_val, shamt) : sra(rs1_val, shamt);
            val_to_write = shifted_val;
            break;
        }
    }
    write_reg(inst.rd, val_to_write);
    return true;
}

bool CPU::execute_op(const Instruction &inst) {
    uint32_t rs1_val = read_reg(inst.rs1);
    uint32_t rs2_val = read_reg(inst.rs2);
    uint32_t val_to_write = 0;

    switch (static_cast<OpFunct3>(inst.funct3)) {
        case OpFunct3::ADD_SUB: {
            uint32_t alt_op = extract_bits(inst.funct7, 5, 5);
            val_to_write = (alt_op == 0) ? (rs1_val + rs2_val) : (rs1_val - rs2_val);
            break;
        }
        case OpFunct3::SLL: val_to_write = sll(rs1_val, rs2_val); break;
        case OpFunct3::SLT: val_to_write = (static_cast<int32_t>(rs1_val) < static_cast<int32_t>(rs2_val)); break;
        case OpFunct3::SLTU: val_to_write = (rs1_val < rs2_val); break;
        case OpFunct3::SRL_SRA: {
            uint32_t alt_op = extract_bits(inst.funct7, 5, 5);
            uint32_t val_to_write = (alt_op == 0) ? srl(rs1_val, rs2_val) : sra(rs1_val, rs2_val);
            break;
        }
        case OpFunct3::XOR: val_to_write = rs1_val ^ rs2_val; break;
        case OpFunct3::OR: val_to_write = rs1_val | rs2_val; break;
        case OpFunct3::AND: val_to_write = rs1_val & rs2_val; break;
    }
    write_reg(inst.rd, val_to_write);
    return true;
}

bool CPU::execute_sys(const Instruction &inst) {
    switch (static_cast<EcallCodes>(registers_[a7])) {
        case EcallCodes::PRINT_INTEGER: std::cout << static_cast<int32_t>(registers_[a0]); break;
        case EcallCodes::PRINT_STRING: return ecall_print_string();
        case EcallCodes::EXIT: running_ = false; break;
        case EcallCodes::PRINT_CHARACTER: {
            std::cout << static_cast<char>(registers_[a0]);
            break;
        }
        case EcallCodes::EXIT_WITH_CODE: {
            exit_code_ = registers_[a0];
            running_ = false;
            break;
        }
    }
    return true;
}

bool CPU::ecall_print_string() {
    uint32_t addr = registers_[a0];
    bool read_res = false;
    uint8_t val{};

    while ((read_res = mem_.read8(addr, val)) && val != 0) {
        std::cout << static_cast<char>(val);
        addr++;
    }
    if (!read_res) {
        err_addr_ = addr;
    }
    return read_res;
}

bool CPU::execute(const Instruction &inst) {
    switch (inst.opcode) {
        case Opcode::LUI:
            return execute_lui(inst);
        case Opcode::AUIPC:
            return execute_auipc(inst);
        case Opcode::JAL:
            return execute_jal(inst);
        case Opcode::JALR:
            return execute_jalr(inst);
        case Opcode::BRANCH:
            return execute_branch(inst);
        case Opcode::LOAD:
            return execute_load(inst);
        case Opcode::STORE:
            return execute_store(inst);
        case Opcode::OP_IMM:
            return execute_op_imm(inst);
        case Opcode::OP:
            return execute_op(inst);
        case Opcode::SYS:
            return execute_sys(inst);
        default:
            return false;
    }
}

uint32_t CPU::exit_code() {
    // LOG_INFO("program finished execution: exit_code=%d\n", exit_code_);
    return exit_code_;
}

void CPU::run() {
    for (;;) {
        uint32_t inst_word{}; 
        if (!mem_.read32(pc_, inst_word)) {
            LOG_ERR("read out of bounds for PC at 0x%08x\n", pc_);
            return;
        }
        auto inst = decode(inst_word);
        if (!inst) {
            LOG_ERR("opcode decode error\n");
            return;
        }
        bool execution_res = execute(inst.value());
        if (!execution_res) {
            const char *err_type = inst.value().opcode == Opcode::STORE ? "write" : "read";
            LOG_ERR("%s out of bounds at 0x%08x\n", err_type, err_addr_);
            return;
        }
        if (!running_) {
            break;
        }
        pc_ = branched_ ? branch_target_ : pc_ + 4;
        branched_ = false;
    }
}