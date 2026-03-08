#include "ALU.h"

ALU::ALU() {}

inline uint32_t ALU::add(const uint32_t a, const uint32_t b) {
    return a + b;
}

inline uint32_t ALU::sub(const uint32_t a, const uint32_t b) {
    return a - b;
}

// Logical operations
inline uint32_t and_op(const uint32_t a, const uint32_t b) {
    return a & b;
}

inline uint32_t or_op(const uint32_t a, const uint32_t b) {
    return a | b;
}

inline uint32_t xor_op(const uint32_t a, const uint32_t b) {
    return a ^ b;
}

inline uint32_t not_op(const uint32_t a) {
    return ~a;
}

// Shift operations
uint32_t sll(const uint32_t a, const uint32_t b);
uint32_t srl(const uint32_t a, const uint32_t b);
uint32_t sra(const uint32_t a, const uint32_t b);
uint32_t slt(const uint32_t a, const uint32_t b);
uint32_t sltu(const uint32_t a, const uint32_t b);