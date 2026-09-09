#include <gtest/gtest.h>

#include "bit_manipulation.h"

TEST(BitManipulationTest, ExtractBitsRejectsOutOfBoundsHi)
{
    uint32_t bits{0};
    EXPECT_DEATH(std::ignore = riscvsim::extract_bits(bits, 33, 0), "");
}

TEST(BitManipulationTest, ExtractBitsRejectsOutOfBoundsLo)
{
    uint32_t bits{0};
    EXPECT_DEATH(std::ignore = riscvsim::extract_bits(bits, 0, 33), "");
}

TEST(BitManipulationTest, ExtractBitsRejectsInvertedHiLo)
{
    uint32_t bits{0};
    EXPECT_DEATH(std::ignore = riscvsim::extract_bits(bits, 0, 1), "");
}

TEST(BitManipulationTest, ExtractBitsSingleBit)
{
    uint32_t bits{0};
    bits = bits | (1U << 15);
    auto extracted_bit = riscvsim::extract_bits(bits, 15, 15);
    EXPECT_EQ(extracted_bit, 0b1);
}

TEST(BitManipulationTest, ExtractBitsMultipleBits)
{
    uint32_t bits{0};
    bits = bits | (1U << 15) | (1U << 16);
    auto extracted_bits = riscvsim::extract_bits(bits, 16, 15);
    EXPECT_EQ(extracted_bits, 0b11);
}

TEST(BitManipulationTest, AppendBits)
{
    uint32_t a{0};
    uint32_t b{1};
    auto appended_bits = riscvsim::append_bits(a, b, 1);
    EXPECT_EQ(appended_bits, 0b10);
}

TEST(BitManipulationTest, SignExtend)
{
    uint32_t a{0b10};
    auto sign_extended = riscvsim::sign_extend(a, 2);
    EXPECT_EQ(sign_extended, -2);
}

TEST(BitManipulationTest, Sll)
{
    uint32_t a{0x80000000};
    auto shifted_left_logical = riscvsim::sll(a, 1);
    EXPECT_EQ(shifted_left_logical, 0);
}

TEST(BitManipulationTest, Srl)
{
    uint32_t a{0b1};
    auto shifted_right_logical = riscvsim::srl(a, 1);
    EXPECT_EQ(shifted_right_logical, 0);
}

TEST(BitManipulationTest, Sra)
{
    uint32_t a{0x80000001};
    auto shifted_right_arithmetic = riscvsim::sra(a, 1);
    EXPECT_EQ(shifted_right_arithmetic, 0xC0000000);
}