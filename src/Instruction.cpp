#include "Instruction.h"
#include "bit_manipulation.h"

namespace riscvsim
{

[[nodiscard]] static Opcode decode_opcode(const uint32_t instruction)
{
    const Opcode opcode{extract_bits(instruction, 6, 0)};
    // Compiler won't generate invalid opcodes, but the extra check
    // is kept for added sanity (e.g.: to reject corrupted/modified files)
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

[[nodiscard]] Instruction decode_r_type(uint32_t inst_raw, Opcode opc)
{
    Instruction instruction;
    instruction.opcode = opc;
    instruction.rd = extract_bits(inst_raw, 11, 7);
    instruction.funct3 = extract_bits(inst_raw, 14, 12);
    instruction.rs1 = extract_bits(inst_raw, 19, 15);
    instruction.rs2 = extract_bits(inst_raw, 24, 20);
    instruction.funct7 = extract_bits(inst_raw, 31, 25);
    return instruction;
}

[[nodiscard]] Instruction decode_i_type(uint32_t inst_raw, Opcode opc)
{
    Instruction instruction;
    instruction.opcode = opc;
    instruction.rd = extract_bits(inst_raw, 11, 7);
    instruction.funct3 = extract_bits(inst_raw, 14, 12);
    instruction.rs1 = extract_bits(inst_raw, 19, 15);
    instruction.imm = sign_extend(extract_bits(inst_raw, 31, 20), 11);
    return instruction;
}

[[nodiscard]] Instruction decode_s_type(uint32_t inst_raw, Opcode opc)
{
    Instruction instruction;
    instruction.opcode = opc;

    const uint32_t lower_imm{extract_bits(inst_raw, 11, 7)};
    const uint32_t upper_imm{extract_bits(inst_raw, 31, 25)};
    instruction.imm = sign_extend(append_bits(lower_imm, upper_imm, 5), 11);

    instruction.funct3 = extract_bits(inst_raw, 14, 12);
    instruction.rs1 = extract_bits(inst_raw, 19, 15);
    instruction.rs2 = extract_bits(inst_raw, 24, 20);
    return instruction;
}

[[nodiscard]] Instruction decode_b_type(uint32_t inst_raw, Opcode opc)
{
    Instruction instruction;
    instruction.opcode = opc;

    const uint32_t bit_12{extract_bits(inst_raw, 7, 7)};
    const uint32_t bits_1_4{extract_bits(inst_raw, 11, 8)};
    const uint32_t bits_5_10{extract_bits(inst_raw, 30, 25)};
    const uint32_t bit_11{extract_bits(inst_raw, 31, 31)};
    const uint32_t imm_not_extended{(bit_12 << 12) | (bit_11 << 11) |
                                    (bits_5_10 << 5) | (bits_1_4 << 1)};
    instruction.imm = sign_extend(imm_not_extended, 13);

    instruction.funct3 = extract_bits(inst_raw, 14, 12);
    instruction.rs1 = extract_bits(inst_raw, 19, 15);
    instruction.rs2 = extract_bits(inst_raw, 24, 20);
    return instruction;
}

[[nodiscard]] Instruction decode_u_type(uint32_t inst_raw, Opcode opc)
{
    Instruction instruction;
    instruction.opcode = opc;
    instruction.rd = extract_bits(inst_raw, 11, 7);
    instruction.imm = extract_bits(inst_raw, 31, 12);
    return instruction;
}

[[nodiscard]] Instruction decode_j_type(uint32_t inst_raw, Opcode opc)
{
    Instruction instruction;
    instruction.opcode = opc;
    instruction.rd = extract_bits(inst_raw, 11, 7);

    const uint32_t bits_12_19{extract_bits(inst_raw, 19, 12)};
    const uint32_t bit_11{extract_bits(inst_raw, 20, 20)};
    const uint32_t bits_1_10{extract_bits(inst_raw, 30, 21)};
    const uint32_t bit_20{extract_bits(inst_raw, 31, 31)};
    const uint32_t imm_not_extended{(bit_20 << 20) | (bits_12_19 << 12) |
                                    (bit_11 << 11) | (bits_1_10 << 1)};

    instruction.imm = sign_extend(imm_not_extended, 21);
    return instruction;
}

[[nodiscard]] std::optional<Instruction> decode(const uint32_t inst_raw)
{
    const Opcode opcode{decode_opcode(inst_raw)};
    switch (opcode)
    {
        case Opcode::LUI:
        case Opcode::AUIPC:
            return decode_u_type(inst_raw, opcode);
        case Opcode::JAL:
            return decode_j_type(inst_raw, opcode);
        case Opcode::JALR:
        case Opcode::LOAD:
        case Opcode::OP_IMM:
        case Opcode::SYS:
            return decode_i_type(inst_raw, opcode);
        case Opcode::BRANCH:
            return decode_b_type(inst_raw, opcode);
        case Opcode::STORE:
            return decode_s_type(inst_raw, opcode);
        case Opcode::OP:
            return decode_r_type(inst_raw, opcode);
        case Opcode::FENCE:
        case Opcode::INVALID:
            return std::nullopt;
    }
    return std::nullopt;
}

};  // namespace riscvsim