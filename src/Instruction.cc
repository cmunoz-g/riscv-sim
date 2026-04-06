#include "Instruction.h"
#include "bit_manipulation.h"

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

Instruction decode_r_type(uint32_t inst_raw, Opcode opc) {
    Instruction inst;
    inst.opcode = opc;
    inst.rd = extract_bits(inst_raw, 12, 7);
    inst.funct3 = extract_bits(inst_raw, 15, 12);
    inst.rs1 = extract_bits(inst_raw, 20, 15);
    inst.rs2 = extract_bits(inst_raw, 25, 20);
    inst.funct7 = extract_bits(inst_raw, 32, 25);
    return inst;
}

Instruction decode_i_type(uint32_t inst_raw, Opcode opc) {
    Instruction inst;
    inst.opcode = opc;
    inst.rd = extract_bits(inst_raw, 12, 7);
    inst.funct3 = extract_bits(inst_raw, 15, 12);
    inst.rs1 = extract_bits(inst_raw, 20, 15);
    inst.imm = sign_extend(extract_bits(inst_raw, 32, 20), 11);
    return inst;
}

Instruction decode_s_type(uint32_t inst_raw, Opcode opc) {
    Instruction inst;
    inst.opcode = opc;
    
    uint32_t lower_imm = extract_bits(inst_raw, 12, 7);
    uint32_t upper_imm = extract_bits(inst_raw, 32, 20);
    inst.imm = sign_extend(append_bits(lower_imm, upper_imm, 5), 11);

    inst.funct3 = extract_bits(inst_raw, 15, 12);
    inst.rs1 = extract_bits(inst_raw, 20, 15);
    inst.rs2 = extract_bits(inst_raw, 25, 20);
    return inst;
}

Instruction decode_b_type(uint32_t inst_raw, Opcode opc) {
    Instruction inst;
    inst.opcode = opc;
    
    uint32_t bits_1_to_4_imm = extract_bits(inst_raw, 12, 8);
    uint32_t bits_5_to_10_imm = extract_bits(inst_raw, 31, 25);
    uint32_t bits_11_12_imm = (inst_raw >> 7) | (inst_raw >> 31);
    uint32_t bits_1_to_10_imm = append_bits(bits_1_to_4_imm, bits_5_to_10_imm, 4);
    inst.imm = sign_extend(append_bits(bits_1_to_10_imm, bits_11_12_imm, 10), 12);

    inst.funct3 = extract_bits(inst_raw, 15, 12);
    inst.rs1 = extract_bits(inst_raw, 20, 15);
    inst.rs2 = extract_bits(inst_raw, 25, 20);
    return inst;
}

Instruction decode_u_type(uint32_t inst_raw, Opcode opc) {
    Instruction inst;
    inst.opcode = opc;
    inst.rd = extract_bits(inst_raw, 12, 7);
    inst.imm = extract_bits(inst_raw, 32, 12); //todo: forgot if this should be sign extended, review
    return inst;
}

Instruction decode_j_type(uint32_t inst_raw, Opcode opc) {
    Instruction inst;
    inst.opcode = opc;
    inst.rd = extract_bits(inst_raw, 12, 7);
    
    uint32_t bits_1_to_11_imm = extract_bits(inst_raw, 31, 21) | (inst_raw >> 10);
    uint32_t bits_12_to_20_imm = extract_bits(inst_raw, 20, 12) | (inst_raw >> 23);

    inst.imm = sign_extend(append_bits(bits_1_to_11_imm, bits_12_to_20_imm, 10), 20);
    return inst;
}

inline char get_opcode_type(const Opcode opc) {
    switch (opc) {
        case Opcode::LUI: return 'U';
        case Opcode::AUIPC: return 'U';
        case Opcode::JAL: return 'J';
        case Opcode::JALR: return 'I';
        case Opcode::LOAD: return 'I';
        case Opcode::OP_IMM: return 'I';
        case Opcode::SYS: return 'I';
        case Opcode::BRANCH: return 'B';
        case Opcode::STORE: return 'S';
        case Opcode::OP: return 'R';
        default: return '\0';
    }
}

std::optional<Instruction> decode(const uint32_t inst_raw) {
    Opcode opc = decode_opcode(inst_raw);
    if (opc == Opcode::INVALID || opc == Opcode::FENCE) {
        return std::nullopt;
    }
    char opc_type = get_opcode_type(opc);
    if (opc_type) {
        return decodeFuncts.at(opc_type)(inst_raw, opc);
    }
    return std::nullopt;
}