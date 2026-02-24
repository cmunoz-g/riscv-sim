#pragma once
#include <cstdint>

enum class Opcode {
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

struct Instruction {
    Opcode opcode;
    uint8_t rd;
    uint8_t funct3;
    uint8_t funct7;
    uint8_t rs1;
    uint8_t rs2;
    int32_t imm;
};

uint32_t extract_bits(uint32_t inst, uint8_t hi, uint8_t lo);
int32_t sign_extend(uint32_t val, uint32_t sign_bit);