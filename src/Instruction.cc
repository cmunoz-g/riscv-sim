#include "Instruction.h"

uint32_t extract_bits(uint32_t inst, uint8_t hi, uint8_t lo)
{
    if (hi > 31 || lo > 31 || (hi <= lo))
    {
        (void)0; // TODO: resolve
    }

    uint32_t mask{};
    for (uint8_t i = lo; i < hi; ++i)
    {
        mask = mask | (1 << i);
    }
    return (inst & mask) >> lo;
}

uint32_t append_bits(uint32_t a, uint32_t b, uint8_t shift) {
    return a | (b << shift);
}

int32_t sign_extend(uint32_t val, uint32_t sign_bit)
{ // TODO: review
    const uint32_t mask = 1U << (sign_bit - 1);
    val = val & ((1U << sign_bit) - 1);
    return (val ^ mask) - mask;
}

Opcode decode_opcode(uint32_t inst)
{
    Opcode opcode = static_cast<Opcode>(extract_bits(inst, 7, 0));
    // Kind of a redundant check, since the compiler won't generate invalid opcodes
    // It's however kept for added sanity (e.g.: to reject corrupted/modified files)
    switch (opcode)
    {
    case Opcode::LUI:
    case Opcode::AUIPC:
    case Opcode::JAL:
    case Opcode::JALR:
    case Opcode::BRANCH:
    case Opcode::LOAD:
    case Opcode::STORE:
    case Opcode::OP_IMM:
    case Opcode::OP:
    case Opcode::FENCE:
    case Opcode::SYS:
        return opcode;
    default:
        return Opcode::INVALID;
    };
}

/*
struct Instruction {
    Opcode opcode;
    uint8_t rd;
    uint8_t funct3;
    uint8_t funct7;
    uint8_t rs1;
    uint8_t rs2;
    int32_t imm;
};
*/

Instruction decode_r_type(uint32_t inst_raw, Opcode opc) {
    Instruction ins;
    ins.opcode = opc;
    ins.rd = extract_bits(inst_raw, 12, 7);
    ins.funct3 = extract_bits(inst_raw, 15, 12);
    ins.rs1 = extract_bits(inst_raw, 20, 15);
    ins.rs2 = extract_bits(inst_raw, 25, 20);
    ins.funct7 = extract_bits(inst_raw, 32, 25);
    return ins;
}

Instruction decode_i_type(uint32_t inst_raw, Opcode opc) {
    Instruction ins;
    ins.opcode = opc;
    ins.rd = extract_bits(inst_raw, 12, 7);
    ins.funct3 = extract_bits(inst_raw, 15, 12);
    ins.rs1 = extract_bits(inst_raw, 20, 15);
    ins.imm = sign_extend(extract_bits(inst_raw, 32, 20), 11);
    return ins;
}

Instruction decode_s_type(uint32_t inst_raw, Opcode opc) {
    Instruction ins;
    ins.opcode = opc;
    
    uint32_t lower_imm = extract_bits(inst_raw, 12, 7);
    uint32_t upper_imm = extract_bits(inst_raw, 32, 20);
    ins.imm = sign_extend(append_bits(lower_imm, upper_imm, 5), 11);

    ins.funct3 = extract_bits(inst_raw, 15, 12);
    ins.rs1 = extract_bits(inst_raw, 20, 15);
    ins.rs2 = extract_bits(inst_raw, 25, 20);
    return ins;
}

Instruction decode_b_type(uint32_t inst_raw, Opcode opc) {
    Instruction ins;
    ins.opcode = opc;
    
    uint32_t lower_imm = extract_bits(inst_raw, 12, 8);
    uint32_t upper_imm = extract_bits(inst_raw, 31, 25);
    ins.imm = sign_extend(append_bits(lower_imm, upper_imm, 4), 10); // TODO: fix sign bit

    ins.funct3 = extract_bits(inst_raw, 15, 12);
    ins.rs1 = extract_bits(inst_raw, 20, 15);
    ins.rs2 = extract_bits(inst_raw, 25, 20);
    return ins;
}

Instruction decode_u_type(uint32_t inst_raw, Opcode opc) {
    Instruction ins;
    ins.opcode = opc;
    ins.rd = extract_bits(inst_raw, 12, 7);
    ins.imm = extract_bits(inst_raw, 32, 12);
    return ins;
}

Instruction decode_j_type(uint32_t inst_raw, Opcode opc) {
    Instruction ins;
    ins.opcode = opc;
    ins.rd = extract_bits(inst_raw, 12, 7);
    
    uint32_t lower_imm = extract_bits(inst_raw, 31, 21);
    uint32_t upper_imm = extract_bits(inst_raw, 20, 12);
    ins.imm = sign_extend(append_bits(lower_imm, upper_imm, 10), 10); // TODO: fix sign bit

}