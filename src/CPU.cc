#include "CPU.h"

CPU::CPU(Memory *mem) : mem_(mem) {
    alu_ = ALU();
}

std::optional<uint32_t> CPU::read_reg(const uint32_t index) const {
    if (index > 31) {
        return std::nullopt;
    }
    return registers_[index];
}

// ? write_reg : Should an invalid write request return false and be managed in the caller ?
void CPU::write_reg(const uint32_t index, const uint32_t val) {
    if (index > 0 && index < 32) { 
        registers_[index] = val;
    }
}

void CPU::run() {}