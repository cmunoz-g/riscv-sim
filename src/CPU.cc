#include "CPU.h"
#include "Instruction.h"
#include <cstdio> //todo: remove
#include <iostream>

CPU::CPU(Memory *mem) : mem_(mem), running_(true), exit_code_(0) {}

// Read / Write
// ? Need to check bounds in read/write ?
uint32_t CPU::read_reg(const uint32_t index) const {
    return registers_[index];
}

bool CPU::write_reg(const uint32_t index, const uint32_t val) {
    if (index > 0 && index < 32) {
        registers_[index] = val;
        return true;
    }
    return false;
}

//todo: review what if rd is 0x0 ? write_reg returns false but should not end execution

// Execution
bool CPU::execute_lui(const Instruction &inst) {
    // LUI places the 32-bit U-immediate value into the destination register rd, filling in the lowest 12 bits with zeros.
    uint32_t val = sll(inst.imm, 12);
    return write_reg(inst.rd, val);
}

bool CPU::execute_auipc(const Instruction &inst) {
    // AUIPC forms a 32-bit offset from the U-immediate, filling in the lowest 12 bits with zeros,
    // adds this offset to the address of the AUIPC instruction, then places the result in register rd.
    uint32_t offset = sll(inst.imm, 12);
    return write_reg(inst.rd, pc_ + offset);
}

// ? Plain unconditional jumps (assembler pseudoinstruction J) are encoded as a JAL with rd=x0. ?
bool CPU::execute_jal(const Instruction &inst) {
    uint32_t return_addr = pc_ + 4;
    pc_ += inst.imm;
    return write_reg(inst.rd, return_addr);
}

bool CPU::execute_jalr(const Instruction &inst) {
    // The target address is obtained by adding the sign-extended 12-bit I-immediate to the register rs1, then setting the least-significant bit
    // of the result to zero. The address of the instruction following the jump (pc+4) is written to register rd. 
    uint32_t return_addr = pc_ + 4;
    pc_ = (inst.rs1 + inst.imm) & ~1;
    return write_reg(inst.rd, return_addr);
}

bool CPU::execute_branch(const Instruction &inst) {
    bool comp = false;
    uint32_t rs1_val = read_reg(inst.rs1);
    uint32_t rs2_val = read_reg(inst.rs2);

    switch (static_cast<BranchFunct3>(inst.funct3)) {
        case BranchFunct3::BEQ:   comp = (rs1_val == rs2_val); break;
        case BranchFunct3::BGE:   comp = (static_cast<int32_t>(rs1_val) >= static_cast<int32_t>(rs2_val)); break;
        case BranchFunct3::BGEU:  comp = (rs1_val >= rs2_val); break;
        case BranchFunct3::BLT:   comp = (static_cast<int32_t>(rs1_val) < static_cast<int32_t>(rs2_val)); break;
        case BranchFunct3::BLTU:  comp = (rs1_val < rs2_val); break;
        case BranchFunct3::BNE:   comp = rs1_val != rs2_val; break;
        // ? default case 
    }

    pc_ += comp ? inst.imm : 4; 
    return comp;
}

bool CPU::execute_load(const Instruction &inst) {
    uint32_t addr = inst.rs1 + inst.imm;
    uint32_t val{};
    
    switch (static_cast<LoadFunct3>(inst.funct3)) {
        case LoadFunct3::LB: val = mem_->read8(addr); break;
        case LoadFunct3::LBU: val = sign_extend(mem_->read8(addr), 8); break;
        case LoadFunct3::LH: val = sign_extend(mem_->read16(addr), 16); break;
        case LoadFunct3::LHU: val = mem_->read16(addr); break;
        case LoadFunct3::LW: val = mem_->read32(addr); break;
        // ? default case
    }
    // todo: once read safeguards are fixed in Memory, add checking step here

    return write_reg(inst.rd, val);
}

bool CPU::execute_store(const Instruction &inst) {
    uint32_t addr = inst.rs1 + inst.imm;
    uint32_t rs2_val = read_reg(inst.rs2);
    bool write_res = false;

    switch (static_cast<StoreFunct3>(inst.funct3)) {
        case StoreFunct3::SB: write_res = mem_->write8(addr, extract_bits(rs2_val, 7, 0)); break;
        case StoreFunct3::SH: write_res = mem_->write16(addr, extract_bits(rs2_val, 15, 0)); break;
        case StoreFunct3::SW: write_res = mem_->write32(addr, rs2_val); break;
    }

    return write_res;
}

bool CPU::execute_op_imm(const Instruction &inst) {
    uint32_t rs1_val = read_reg(inst.rs1);
    bool write_res = false;
    
    switch (static_cast<OpImmFunct3>(inst.funct3)) {
        // todo: use alu methods or keep this way
        case OpImmFunct3::ADDI: write_res = write_reg(inst.rd, rs1_val + inst.imm); break;
        case OpImmFunct3::SLTI: write_res = write_reg(inst.rd, (static_cast<int32_t>(rs1_val) < static_cast<int32_t>(inst.imm))); break;
        case OpImmFunct3::SLTIU: write_res = write_reg(inst.rd, (rs1_val < inst.imm)); break;
        case OpImmFunct3::ANDI: write_res = write_reg(inst.rd, (rs1_val & inst.imm)); break;
        case OpImmFunct3::ORI: write_res = write_reg(inst.rd, (rs1_val | inst.imm)); break;
        case OpImmFunct3::XORI: write_res = write_reg(inst.rd, (rs1_val ^ inst.imm)); break;
        case OpImmFunct3::SLLI: {
            uint32_t shamt = extract_bits(inst.imm, 5, 0);
            write_res = write_reg(inst.rd, sll(rs1_val, shamt));
            break;
        }
        case OpImmFunct3::SRLI_SRAI: {
            uint32_t shamt = extract_bits(inst.imm, 5, 0);
            uint32_t funct7 = extract_bits(inst.imm, 31, 30);
            uint32_t shifted_val = (funct7 == 0) ? srl(rs1_val, shamt) : sra(rs1_val, shamt);
            write_res = write_reg(inst.rd, shifted_val);
            break;  
        }
    }

    return write_res;
}

bool CPU::execute_op(const Instruction &inst) {
    uint32_t rs1_val = read_reg(inst.rs1);
    uint32_t rs2_val = read_reg(inst.rs2);
    bool write_res = false;

    switch (static_cast<OpFunct3>(inst.funct3)) {
        case OpFunct3::ADD_SUB: {
            uint32_t funct7 = extract_bits(inst.imm, 31, 30);
            uint32_t val_to_write = (funct7 == 0) ? rs1_val + rs2_val : rs1_val - rs2_val;
            write_res = write_reg(inst.rd, val_to_write);
            break;
        }
        case OpFunct3::SLL: write_res = write_reg(inst.rd, sll(rs1_val, rs2_val)); break;
        case OpFunct3::SLT: write_res = write_reg(inst.rd, (static_cast<int32_t>(rs1_val) < static_cast<int32_t>(rs2_val))); break;
        case OpFunct3::SLTU: write_res = write_reg(inst.rd, (rs1_val < rs2_val)); break;
        case OpFunct3::SRL_SRA: {
            uint32_t val_to_write = inst.funct7 == 0 ? srl(rs1_val, rs2_val) : sra(rs1_val, rs2_val);
            write_res = write_reg(inst.rd, val_to_write);
            break;
        }
        case OpFunct3::XOR: write_res = write_reg(inst.rd, inst.rs1 ^ inst.rs2); break;
    }
    return write_res;
} 

bool CPU::execute_sys(const Instruction &inst) {
    switch (static_cast<EcallCodes>(registers_[a7])) {
        case EcallCodes::PRINT_INTEGER: std::cout << registers_[a0]; break;
        case EcallCodes::PRINT_STRING: ecall_print_string(); break;
        case EcallCodes::EXIT: running_ = false; break;
        case EcallCodes::PRINT_CHARACTER: {
            std::cout << static_cast<char>(mem_->read8(registers_[a0])); // ? could fail 
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
    while (char read_char = static_cast<char>(mem_->read8(addr))) {
        std::cout << read_char; // todo: fix issue. if returns -1, won't terminate loop
        addr++;
    }
    return true;
}

bool CPU::execute(const Instruction &inst) {
    switch (inst.opcode) {
        case Opcode::LUI: return execute_lui(inst);
        case Opcode::AUIPC: return execute_auipc(inst);
        case Opcode::JAL: return execute_jal(inst);
        case Opcode::JALR: return execute_jalr(inst);
        case Opcode::BRANCH: return execute_branch(inst);
        case Opcode::LOAD: return execute_load(inst);
        case Opcode::STORE: return execute_store(inst);
        case Opcode::OP_IMM: return execute_op_imm(inst);
        case Opcode::OP: return execute_op(inst);
        case Opcode::SYS: return execute_sys(inst);
        case Opcode::INVALID: return false;
        // ? default
    }
    return false; // ?
}

void CPU::run() {
    for (;;) {
        uint32_t inst_word = mem_->read32(pc_);
        auto inst = decode(inst_word);
        if (!inst) {
            printf("error placeholder - 1\n");
            return;
        }
        bool status = execute(inst.value()); 
        if (!status) { // ? what does error here mean ? bc it depends on the instruction type
            // ? (i.e.) a branch returning false != error 
            printf("error placeholder - 2\n");
            return;
        }
        if (!running_) {
            printf("exit placeholder (%d)\n", exit_code_);
            break;
        }
        pc_ = pc_ + 4; // todo: this is not taking into account if last instruction was jump/branch, it should
    }
}