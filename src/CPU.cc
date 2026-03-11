#include "CPU.h"

CPU::CPU(Memory *mem) : mem_(mem) {
    alu_ = ALU();
}

// Read / Write
std::optional<uint32_t> CPU::read_reg(const uint32_t index) const {
    if (index > 31) {
        return std::nullopt;
    }
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
    uint32_t val = alu_.sll(inst.imm, 12);
    return write_reg(inst.rd, val);
}

bool CPU::execute_auipc(const Instruction &inst) {
    // AUIPC forms a 32-bit offset from the U-immediate, filling in the lowest 12 bits with zeros,
    // adds this offset to the address of the AUIPC instruction, then places the result in register rd.
    uint32_t offset = alu_.sll(inst.imm, 12);
    return write_reg(inst.rd, alu_.add(pc_, offset));
}

// ? Plain unconditional jumps (assembler pseudoinstruction J) are encoded as a JAL with rd=x0. ?
bool CPU::execute_jal(const Instruction &inst) {
    uint32_t return_addr = pc_ + 4;
    pc_ = alu_.add(pc_, inst.imm);
    return write_reg(inst.rd, return_addr);
}

bool CPU::execute_jalr(const Instruction &inst) {
    // The target address is obtained by adding the sign-extended 12-bit I-immediate to the register rs1, then setting the least-significant bit
    // of the result to zero. The address of the instruction following the jump (pc+4) is written to register rd. 
    uint32_t return_addr = pc_ + 4;
    pc_ = alu_.add(inst.rs1, inst.imm) & ~1;
    return write_reg(inst.rd, return_addr);
}

bool CPU::execute_branch(const Instruction &inst) {
    bool comp = false;
    uint32_t rs1_val, rs2_val;
    // todo: think if removing std::optional from read_reg. should the check (if it's necessary) be done at decode step ?

    switch (static_cast<BranchCond>(inst.funct3)) {
        case BranchCond::BEQ:   comp = (rs1_val == rs2_val); break;
        case BranchCond::BGE:   comp = (static_cast<int8_t>(rs1_val) >= static_cast<int8_t>(rs2_val)); break;
        case BranchCond::BGEU:  comp = (rs1_val > rs2_val); break;
        case BranchCond::BLT:   comp = (static_cast<int8_t>(rs1_val) < static_cast<int8_t>(rs2_val)); break;
        case BranchCond::BLTU:  comp = (rs1_val < rs2_val); break;
        case BranchCond::BNE:   comp = rs1_val != rs2_val; break;
    }

    pc_ += comp ? inst.imm : 4; 
    return comp;
}

void CPU::run() {}