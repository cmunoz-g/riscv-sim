#pragma once
#include <cstdint>
#include <unordered_map>

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

using decode_funct = Instruction (*)(uint32_t, Opcode);

static const std::unordered_map<char, decode_funct> decodeFuncts = {
    {'U', decode_u_type},
    {'J', decode_j_type},
    {'B', decode_b_type},
    {'S', decode_s_type},
    {'R', decode_r_type},
    {'I', decode_i_type}
};

uint32_t extract_bits(uint32_t inst, uint8_t hi, uint8_t lo);
int32_t sign_extend(uint32_t val, uint32_t sign_bit);
uint32_t append_bits(uint32_t a, uint32_t b, uint8_t shamt);