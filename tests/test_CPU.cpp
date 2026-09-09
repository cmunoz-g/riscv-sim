#include <gtest/gtest.h>

#include "CPU.h"
#include "load_file.h"

class CPUWriteReadTest : public ::testing::Test
{
protected:
    uint8_t data[4] = {0x12, 0x34, 0x56, 0x78};
    riscvsim::CPU cpu{data, sizeof(data)};
};

TEST_F(CPUWriteReadTest, WriteThenReadRegister)
{
    uint8_t write_value{0x99};
    uint8_t index{1};
    cpu.write_reg(index, write_value);
    uint32_t read_value = cpu.read_reg(index);
    EXPECT_EQ(read_value, write_value);
}

TEST_F(CPUWriteReadTest, IgnoreWriteToZero)
{
    uint8_t write_value{0x99};
    uint8_t index{};
    cpu.write_reg(index, write_value);
    uint32_t read_value = cpu.read_reg(index);
    EXPECT_EQ(read_value, 0);
}

TEST_F(CPUWriteReadTest, RejectsInvalidRead)
{
    EXPECT_DEATH(std::ignore = cpu.read_reg(33), "");
}

TEST_F(CPUWriteReadTest, RejectsInvalidWrite)
{
    EXPECT_DEATH(cpu.write_reg(33, 0x99), "");
}

TEST(CPUTest, ExecuteLUI)
{
    auto program = riscvsim::load_file(PROGRAM_BINARY_DIR "/lui.bin");
    riscvsim::CPU cpu(program.data(), program.size());
    cpu.write_reg(riscvsim::a7, 10);
    uint8_t rd{1};
    ASSERT_EQ(cpu.read_reg(rd), 0);

    cpu.run();

    EXPECT_EQ(cpu.read_reg(rd), 0x1000);
}

TEST(CPU_TEST, ExecuteAUIPC)
{
    auto program = riscvsim::load_file(PROGRAM_BINARY_DIR "/auipc.bin");
    riscvsim::CPU cpu(program.data(), program.size());
    cpu.write_reg(riscvsim::a7, 10);
    uint8_t rd{1};
    ASSERT_EQ(cpu.read_reg(rd), 0);

    cpu.run();

    EXPECT_EQ(cpu.read_reg(rd), 0x1000);
}

TEST(CPU_TEST, ExecuteJAL)
{
    auto program = riscvsim::load_file(PROGRAM_BINARY_DIR "/jal.bin");
    riscvsim::CPU cpu(program.data(), program.size());
    cpu.write_reg(riscvsim::a7, 10);
    uint8_t rd{1};
    ASSERT_EQ(cpu.read_reg(rd), 0);

    cpu.run();

    EXPECT_EQ(cpu.read_reg(rd), 0x4);
    EXPECT_EQ(cpu.read_reg(2), 0x1000);
}

TEST(CPU_TEST, ExecuteJALR)
{
    auto program = riscvsim::load_file(PROGRAM_BINARY_DIR "/jalr.bin");
    riscvsim::CPU cpu(program.data(), program.size());
    cpu.write_reg(riscvsim::a7, 10);
    uint8_t rd{2};

    cpu.run();

    EXPECT_EQ(cpu.read_reg(rd), 8);
    EXPECT_EQ(cpu.read_reg(3), 0x1000);
}

TEST(CPU_TEST, ExecuteBRANCH)
{
    auto program = riscvsim::load_file(PROGRAM_BINARY_DIR "/branch.bin");
    riscvsim::CPU cpu(program.data(), program.size());
    cpu.write_reg(riscvsim::a7, 10);
    cpu.write_reg(3, 0xFFFFFFFF);
    cpu.write_reg(4, 1);
    ASSERT_EQ(cpu.read_reg(1), 0);
    ASSERT_EQ(cpu.read_reg(2), 0);
    ASSERT_EQ(cpu.read_reg(3), 0xFFFFFFFF);
    ASSERT_EQ(cpu.read_reg(4), 1);
    ASSERT_EQ(cpu.read_reg(5), 0);

    cpu.run();

    EXPECT_EQ(cpu.read_reg(5), 0x1000);
}

TEST(CPU_TEST, ExecuteSTOREThenLOAD)
{
    auto program = riscvsim::load_file(PROGRAM_BINARY_DIR "/store_load.bin");
    riscvsim::CPU cpu(program.data(), program.size());
    cpu.write_reg(riscvsim::a7, 10);
    cpu.write_reg(1, 100);
    cpu.write_reg(2, 0x1234F6F8);

    cpu.run();

    EXPECT_EQ(cpu.read_reg(3), 0xFFFFFFF8);
    EXPECT_EQ(cpu.read_reg(4), 0x000000F8);
    EXPECT_EQ(cpu.read_reg(5), 0xFFFFF6F8);
    EXPECT_EQ(cpu.read_reg(6), 0x0000F6F8);
    EXPECT_EQ(cpu.read_reg(7), 0x1234F6F8);
}

TEST(CPU_TEST, ExecuteADDI)
{
    auto program = riscvsim::load_file(PROGRAM_BINARY_DIR "/opimm.bin");
    riscvsim::CPU cpu(program.data(), program.size());
    cpu.write_reg(riscvsim::a7, 10);
    cpu.write_reg(10, 0x1);
    cpu.write_reg(11, 0xFFFFFFFF);
    cpu.write_reg(12, 0x80000000);

    cpu.run();

    EXPECT_EQ(cpu.read_reg(1), 0x11);
    EXPECT_EQ(cpu.read_reg(2), 0x1);
    EXPECT_EQ(cpu.read_reg(3), 0);
    EXPECT_EQ(cpu.read_reg(4), 0x1);
    EXPECT_EQ(cpu.read_reg(5), 0x3);
    EXPECT_EQ(cpu.read_reg(6), 0x3);
    EXPECT_EQ(cpu.read_reg(7), 0x2);
    EXPECT_EQ(cpu.read_reg(8), 0x40000000);
    EXPECT_EQ(cpu.read_reg(9), 0xC0000000);
}

TEST(CPU_TEST, ExecuteADD)
{
    auto program = riscvsim::load_file(PROGRAM_BINARY_DIR "/op.bin");
    riscvsim::CPU cpu(program.data(), program.size());
    cpu.write_reg(riscvsim::a7, 10);
    cpu.write_reg(11, 1);
    cpu.write_reg(12, 2);
    cpu.write_reg(13, 0xFFFFFFFF);

    cpu.run();

    EXPECT_EQ(cpu.read_reg(1), 0x3);
    EXPECT_EQ(cpu.read_reg(2), 0x1);
    EXPECT_EQ(cpu.read_reg(3), 0x4);
    EXPECT_EQ(cpu.read_reg(4), 0x1);
    EXPECT_EQ(cpu.read_reg(5), 0);
    EXPECT_EQ(cpu.read_reg(6), 0x1);
    EXPECT_EQ(cpu.read_reg(7), 0xFFFFFFFF);
    EXPECT_EQ(cpu.read_reg(8), 0x3);
    EXPECT_EQ(cpu.read_reg(9), 0x3);
    EXPECT_EQ(cpu.read_reg(10), 0);
}

TEST(CPU_TEST, ExecuteSYS)
{
    auto program = riscvsim::load_file(PROGRAM_BINARY_DIR "/sys.bin");
    riscvsim::CPU cpu(program.data(), program.size());
    testing::internal::CaptureStdout();

    cpu.run();

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "a1test");
    EXPECT_EQ(cpu.exit_code(), 0);
}

TEST(CPU_TEST, ExecuteSYSExitWithCode)
{
    auto program = riscvsim::load_file(PROGRAM_BINARY_DIR "/sys_exit_code.bin");
    riscvsim::CPU cpu(program.data(), program.size());
    testing::internal::CaptureStdout();

    cpu.run();

    EXPECT_EQ(cpu.exit_code(), 1);
}