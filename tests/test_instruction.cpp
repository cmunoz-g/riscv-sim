#include <gtest/gtest.h>

#include "Instruction.h"

TEST(Instruction, DecodeIncorrectOpcode)
{
    uint32_t invalid_instruction{0};
    auto decoded = riscvsim::decode(invalid_instruction);
    ASSERT_EQ(decoded, std::nullopt);
}

TEST(Instruction, DecodeFenceOpcode)
{
    uint32_t fence_instruction{0x0FF0000F};
    auto decoded = riscvsim::decode(fence_instruction);
    ASSERT_EQ(decoded, std::nullopt);
}

TEST(Instruction, DecodeRType)
{
    // 0000000 00111 00110 000 00101 0110011 add x5, x6, x7
    uint32_t instruction{0x007302B3};
    auto decoded = riscvsim::decode(instruction);
    ASSERT_NE(decoded, std::nullopt);
    riscvsim::Instruction inst = decoded.value();
    ASSERT_EQ(inst.opcode, riscvsim::Opcode::OP);
    ASSERT_EQ(inst.rd, 0b00101);
    ASSERT_EQ(inst.funct3, 0b000);
    ASSERT_EQ(inst.rs1, 0b00110);
    ASSERT_EQ(inst.rs2, 0b00111);
    ASSERT_EQ(inst.funct7, 0b0000000);
}

TEST(Instruction, DecodeIType)
{
    // 000000001010 00110 000 00101 0010011 addi x5, x6, 10
    uint32_t instruction{0x00A30293};
    auto decoded = riscvsim::decode(instruction);
    ASSERT_NE(decoded, std::nullopt);
    riscvsim::Instruction inst = decoded.value();
    ASSERT_EQ(inst.opcode, riscvsim::Opcode::OP_IMM);
    ASSERT_EQ(inst.rd, 0b00101);
    ASSERT_EQ(inst.funct3, 0b000);
    ASSERT_EQ(inst.rs1, 0b00110);
    ASSERT_EQ(inst.imm, 0b000000001010);
}

TEST(Instruction, DecodeSType)
{
    // 0000000 00111 00110 010 01100 0100011 sw x7, 12(x6)
    uint32_t instruction{0x00732623};
    auto decoded = riscvsim::decode(instruction);
    ASSERT_NE(decoded, std::nullopt);
    riscvsim::Instruction inst = decoded.value();
    ASSERT_EQ(inst.opcode, riscvsim::Opcode::STORE);
    ASSERT_EQ(inst.imm, 0b01100);
    ASSERT_EQ(inst.funct3, 0b010);
    ASSERT_EQ(inst.rs1, 0b00110);
    ASSERT_EQ(inst.rs2, 0b00111);
}

TEST(Instruction, DecodeBType)
{
    // 0 000000 00110 00101 000 1000 0 1100011 beq x5, x6, 16
    uint32_t instruction{0x00628863};
    auto decoded = riscvsim::decode(instruction);
    ASSERT_NE(decoded, std::nullopt);
    riscvsim::Instruction inst = decoded.value();
    ASSERT_EQ(inst.opcode, riscvsim::Opcode::BRANCH);
    ASSERT_EQ(inst.imm, 0b0000000010000);
    ASSERT_EQ(inst.funct3, 0b000);
    ASSERT_EQ(inst.rs1, 0b00101);
    ASSERT_EQ(inst.rs2, 0b00110);
}

TEST(Instruction, DecodeUType)
{
    // 00010010001101000101 00101 0110111 lui x5, 0x12345
    uint32_t instruction{0x123452B7};
    auto decoded = riscvsim::decode(instruction);
    ASSERT_NE(decoded, std::nullopt);
    riscvsim::Instruction inst = decoded.value();
    ASSERT_EQ(inst.opcode, riscvsim::Opcode::LUI);
    ASSERT_EQ(inst.imm, 0x12345);
    ASSERT_EQ(inst.rd, 0b00101);
}

TEST(Instruction, DecodeJType)
{
    // 0 0000001000 0 00000000 00001 1101111 jal x1, 16
    uint32_t instruction{0x010000EF};
    auto decoded = riscvsim::decode(instruction);
    ASSERT_NE(decoded, std::nullopt);
    riscvsim::Instruction inst = decoded.value();
    ASSERT_EQ(inst.opcode, riscvsim::Opcode::JAL);
    ASSERT_EQ(inst.rd, 0b00001);
    ASSERT_EQ(inst.imm, 0b000000000000000010000);
}