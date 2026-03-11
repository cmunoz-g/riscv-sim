#include "ALU.h"
#include "Instruction.h"

ALU::ALU() {}

inline uint32_t ALU::add(const uint32_t a, const uint32_t b) {
    return a + b;
}

inline uint32_t ALU::sub(const uint32_t a, const uint32_t b) {
    return a - b;
}

// Logical operations
inline uint32_t ALU::and_op(const uint32_t a, const uint32_t b) {
    return a & b;
}

inline uint32_t ALU::or_op(const uint32_t a, const uint32_t b) {
    return a | b;
}

inline uint32_t ALU::xor_op(const uint32_t a, const uint32_t b) {
    return a ^ b;
}

inline uint32_t ALU::not_op(const uint32_t a) {
    return ~a;
}

// Shift operations
inline uint32_t ALU::sll(const uint32_t a, const uint32_t shamt) {
    return a << (shamt & 0x1F); 
}

inline uint32_t ALU::srl(const uint32_t a, const uint32_t shamt) {
    return a >> (shamt & 0x1F);
}

uint32_t ALU::sra(const uint32_t a, const uint32_t shamt) { // ? Make inline
    uint32_t shifted = a >> (shamt & 0x1F);
    sign_extend(shifted, 32 - shamt);
}

inline uint32_t ALU::slt(const uint32_t a, const uint32_t b) {
    return static_cast<int32_t>(a) < static_cast<int32_t>(b) ? 1 : 0;
}

inline uint32_t ALU::sltu(const uint32_t a, const uint32_t b) {
    return a < b ? 1 : 0;
}