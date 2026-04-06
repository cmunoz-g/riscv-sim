#include "bit_manipulation.h"

uint32_t extract_bits(uint32_t bits, uint8_t hi, uint8_t lo)
{
    if (hi > 31 || lo > 31 || (hi <= lo))
    {
        (void)0; // TODO: resolve
    }
    
    uint32_t mask{};
    for (uint8_t i = lo; i < hi; ++i)
    {
        mask = mask | (1 << i);
    }
    return (bits & mask) >> lo;
}

uint32_t append_bits(uint32_t a, uint32_t b, uint8_t shamt) {
    return a | (b << shamt);
}

int32_t sign_extend(uint32_t val, uint32_t sign_bit) { // TODO: review
    const uint32_t mask = 1U << (sign_bit - 1);
    val = val & ((1U << sign_bit) - 1);
    return (val ^ mask) - mask;
}

uint32_t sll(const uint32_t a, const uint32_t shamt) {
    return a << (shamt & 0x1F); 
}

uint32_t srl(const uint32_t a, const uint32_t shamt) {
    return a >> (shamt & 0x1F);
}

uint32_t sra(const uint32_t a, const uint32_t shamt) {
    uint32_t shifted = a >> (shamt & 0x1F);
    return sign_extend(shifted, 32 - shamt);
}
