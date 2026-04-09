#pragma once
#include <cstdint>
#include <unordered_map>
#include <optional>

enum class Opcode
{
    LUI = 0b0110111,
    AUIPC = 0b0010111,
    JAL = 0b1101111,
    JALR = 0b1100111,
    BRANCH = 0b1100011,
    LOAD = 0b0000011,
    STORE = 0b0100011,
    OP_IMM = 0b0010011,
    OP = 0b0110011,
    FENCE = 0b0001111,
    SYS = 0b1110011,
    INVALID = 0b0000000
};

struct Instruction
{
    Opcode opcode;
    uint8_t rd;
    uint8_t funct3;
    uint8_t funct7;
    uint8_t rs1;
    uint8_t rs2;
    int32_t imm;
};

enum class BranchFunct3 {
    BEQ = 0b000,
    BNE = 0b001,
    BLT = 0b100,
    BGE = 0b101,
    BLTU = 0b110,
    BGEU = 0b111
};

enum class LoadFunct3 {
    LB = 0b000,
    LH = 0b001,
    LW = 0b010,
    LBU = 0b100,
    LHU = 0b101
};

enum class StoreFunct3 {
    SB = 0b000,
    SH = 0b001,
    SW = 0b010
};

enum class OpImmFunct3 {
    ADDI = 0b000,
    SLTI = 0b010,
    SLTIU = 0b011,
    XORI = 0b100,
    ORI = 0b110,
    ANDI = 0b111,
    SLLI = 0b001,
    SRLI_SRAI = 0b101
};

enum class OpFunct3 {
    ADD_SUB = 0b000,
    SLL = 0b001,
    SLT = 0b010,
    SLTU = 0b011,
    XOR = 0b100,
    SRL_SRA = 0b101,
    OR = 0b110,
    AND = 0b111
};

enum class EcallCodes {
    // Scope of the project is correct fetching, decoding and executing of instructions, so only a few ecalls are included
    PRINT_INTEGER = 1,
    PRINT_STRING = 4,
    EXIT = 10,
    PRINT_CHARACTER = 11,
    EXIT_WITH_CODE = 17
};

Instruction decode_r_type(uint32_t inst_raw, Opcode opc);
Instruction decode_i_type(uint32_t inst_raw, Opcode opc);
Instruction decode_s_type(uint32_t inst_raw, Opcode opc);
Instruction decode_b_type(uint32_t inst_raw, Opcode opc);
Instruction decode_u_type(uint32_t inst_raw, Opcode opc);
Instruction decode_j_type(uint32_t inst_raw, Opcode opc);

using decode_funct = Instruction (*)(uint32_t, Opcode);
static const std::unordered_map<char, decode_funct> decodeFuncts = {
    {'U', decode_u_type},
    {'J', decode_j_type},
    {'B', decode_b_type},
    {'S', decode_s_type},
    {'R', decode_r_type},
    {'I', decode_i_type}
};

std::optional<Instruction> decode(const uint32_t inst_raw);