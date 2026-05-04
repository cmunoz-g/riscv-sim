#include "Instruction.h"
#include "bit_manipulation.h"

Opcode decode_opcode(uint32_t inst)
{
    Opcode opcode = static_cast<Opcode>(extract_bits(inst, 6, 0));
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
    inst.rd = extract_bits(inst_raw, 11, 7);
    inst.funct3 = extract_bits(inst_raw, 14, 12);
    inst.rs1 = extract_bits(inst_raw, 19, 15);
    inst.rs2 = extract_bits(inst_raw, 24, 20);
    inst.funct7 = extract_bits(inst_raw, 31, 25);
    return inst;
}

Instruction decode_i_type(uint32_t inst_raw, Opcode opc) {
    Instruction inst;
    inst.opcode = opc;
    inst.rd = extract_bits(inst_raw, 11, 7);
    inst.funct3 = extract_bits(inst_raw, 14, 12);
    inst.rs1 = extract_bits(inst_raw, 19, 15);
    inst.imm = sign_extend(extract_bits(inst_raw, 31, 20), 11);
    return inst;
}

Instruction decode_s_type(uint32_t inst_raw, Opcode opc) {
    Instruction inst;
    inst.opcode = opc;
    
    uint32_t lower_imm = extract_bits(inst_raw, 11, 7);
    uint32_t upper_imm = extract_bits(inst_raw, 31, 20);
    inst.imm = sign_extend(append_bits(lower_imm, upper_imm, 5), 11);

    inst.funct3 = extract_bits(inst_raw, 14, 12);
    inst.rs1 = extract_bits(inst_raw, 19, 15);
    inst.rs2 = extract_bits(inst_raw, 24, 20);
    return inst;
}

Instruction decode_b_type(uint32_t inst_raw, Opcode opc) {
    Instruction inst;
    inst.opcode = opc;

    uint32_t bit_12 = extract_bits(inst_raw, 7, 7);
    uint32_t bits_1_4 = extract_bits(inst_raw, 11, 8);
    uint32_t bits_5_10 = extract_bits(inst_raw, 30, 25);
    uint32_t bit_11 = extract_bits(inst_raw, 31, 31);
    uint32_t imm_not_extended = (bit_12 << 12) | (bit_11 << 11)
        | (bits_5_10 << 5) | (bits_1_4 << 1);
    inst.imm = sign_extend(imm_not_extended, 13);
    
    inst.funct3 = extract_bits(inst_raw, 14, 12);
    inst.rs1 = extract_bits(inst_raw, 19, 15);
    inst.rs2 = extract_bits(inst_raw, 24, 20);
    return inst;
}

Instruction decode_u_type(uint32_t inst_raw, Opcode opc) {
    Instruction inst;
    inst.opcode = opc;
    inst.rd = extract_bits(inst_raw, 11, 7);
    inst.imm = extract_bits(inst_raw, 31, 12);
    return inst;
}

Instruction decode_j_type(uint32_t inst_raw, Opcode opc) {
    Instruction inst;
    inst.opcode = opc;
    inst.rd = extract_bits(inst_raw, 11, 7);

    uint32_t bits_12_19 = extract_bits(inst_raw, 19, 12);
    uint32_t bit_11 = extract_bits(inst_raw, 20, 20);
    uint32_t bits_1_10 = extract_bits(inst_raw, 30, 21);
    uint32_t bit_20 = extract_bits(inst_raw, 31, 31);
    uint32_t imm_not_extended = (bit_20 << 20) | (bits_12_19 << 12)
        | (bit_11 << 11) | (bits_1_10 << 1);

    inst.imm = sign_extend(imm_not_extended, 21);
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