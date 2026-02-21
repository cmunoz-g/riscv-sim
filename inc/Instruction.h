#pragma once
#include <cstdint>

enum class Opcode {
    LUI = 0x0110111,
    AUIPC = 0x0010111,
    JAL = 0x1101111,
    JALR = 0x1100111,
    BRANCH = 0x1100011,
    LOAD = 0x0000011,
    STORE = 0x0100011,
    OP_IMM = 0x0010011,
    OP = 0x0110011,
    FENCE = 0x0001111,
    SYS = 0x1110011
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