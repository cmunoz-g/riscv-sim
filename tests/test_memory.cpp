#include <gtest/gtest.h>

#include "Memory.h"

class MemoryTest : public ::testing::Test
{
protected:
    uint8_t data[4] = {0x12, 0x34, 0x56, 0x78};
    riscvsim::Memory memory{data, sizeof(data)};
};

TEST(IncorrectMemoryConstructionTest, InvalidDataSize)
{
    uint8_t data[4] = {0x12, 0x34, 0x56, 0x78};
    EXPECT_EXIT((void)riscvsim::Memory(data, riscvsim::Memory::mem_size + 1),
                ::testing::ExitedWithCode(1), "");
}

TEST_F(MemoryTest, Read8ValidAddr)
{
    uint8_t value{0};
    ASSERT_TRUE(memory.read8(0, value));
    EXPECT_EQ(value, 0x12);
}

TEST_F(MemoryTest, Read8InvalidAddr)
{
    uint8_t value{0};
    ASSERT_FALSE(memory.read8(memory.mem_size, value));
}

TEST_F(MemoryTest, Read16ValidAddr)
{
    uint16_t value{0};
    ASSERT_TRUE(memory.read16(0, value));
    EXPECT_EQ(value, 0x3412);
}

TEST_F(MemoryTest, Read16InvalidAddr)
{
    uint16_t value{0};
    ASSERT_FALSE(memory.read16(memory.mem_size, value));
}

TEST_F(MemoryTest, Read32ValidAddr)
{
    uint32_t value{0};
    ASSERT_TRUE(memory.read32(0, value));
    EXPECT_EQ(value, 0x78563412);
}

TEST_F(MemoryTest, Read32InvalidAddr)
{
    uint32_t value{0};
    ASSERT_FALSE(memory.read32(memory.mem_size, value));
}

TEST_F(MemoryTest, Write8ValidAddr)
{
    uint8_t value{1};
    uint8_t read_value{0};
    ASSERT_TRUE(memory.write8(0, value));
    ASSERT_TRUE(memory.read8(0, read_value));
    EXPECT_EQ(read_value, 1);
}

TEST_F(MemoryTest, Write8InvalidAddr)
{
    uint8_t value{1};
    ASSERT_FALSE(memory.write8(memory.mem_size, value));
}

TEST_F(MemoryTest, Write16ValidAddr)
{
    uint16_t value{1};
    uint16_t read_value{0};
    ASSERT_TRUE(memory.write16(0, value));
    ASSERT_TRUE(memory.read16(0, read_value));
    EXPECT_EQ(read_value, 1);
}

TEST_F(MemoryTest, Write16InvalidAddr)
{
    uint16_t value{1};
    ASSERT_FALSE(memory.write16(memory.mem_size, value));
}

TEST_F(MemoryTest, Write32ValidAddr)
{
    uint32_t value{1};
    uint32_t read_value{0};
    ASSERT_TRUE(memory.write32(0, value));
    ASSERT_TRUE(memory.read32(0, read_value));
    EXPECT_EQ(read_value, 1);
}

TEST_F(MemoryTest, Write32InvalidAddr)
{
    uint32_t value{1};
    ASSERT_FALSE(memory.write32(memory.mem_size, value));
}