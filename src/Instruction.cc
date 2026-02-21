#include "Instruction.h"

uint32_t extract_bits(uint32_t inst, uint8_t hi, uint8_t lo) {
    if (hi > 31 || lo > 31) {
        (void)0; // resolve
    }
    
    uint32_t mask{};
    for (uint8_t i = lo; i < hi; ++i) {
        mask = mask | (1 << i);
    }
    return (inst & mask) >> lo;
}

int32_t sign_extend(uint32_t val, uint32_t sign_bit) { // TODO: review
    const uint32_t mask = 1U << (sign_bit - 1);
    val = val & ((1U << sign_bit) - 1);
    return (val ^ mask) - mask;
}
