#pragma once

#include <cstdint>

class ALU {
public:
    ALU();
    // Arithmetic operations
    inline uint32_t add(const uint32_t a, const uint32_t b);
    inline uint32_t sub(const uint32_t a, const uint32_t b);
    inline uint32_t inc(uint32_t a); // inc(), dec() not implemented, will do if useful 
    inline uint32_t dec(uint32_t a);

    // Logical operations
    inline uint32_t and_op(const uint32_t a, const uint32_t b);
    inline uint32_t or_op(const uint32_t a, const uint32_t b);
    inline uint32_t xor_op(const uint32_t a, const uint32_t b);
    inline uint32_t not_op(const uint32_t a);

    // Shift operations
    uint32_t sll(const uint32_t a, const uint32_t b);
    uint32_t srl(const uint32_t a, const uint32_t b);
    uint32_t sra(const uint32_t a, const uint32_t b);
    uint32_t slt(const uint32_t a, const uint32_t b);
    uint32_t sltu(const uint32_t a, const uint32_t b);
};