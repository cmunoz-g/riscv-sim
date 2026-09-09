#pragma once

#include <cstdint>

namespace riscvsim
{

// Returns bits 'lo' to 'hi' both inclusive. Requires hi >= lo and hi < 32, lo < 32
[[nodiscard]] uint32_t extract_bits(const uint32_t bits, const unsigned int hi,
                                    const unsigned int lo) noexcept;

// Appends bits given by 'a' and 'b', 'b' is shifted left 'shamt' bits
[[nodiscard]] uint32_t append_bits(const uint32_t a, const uint32_t b,
                                   const uint8_t shamt) noexcept;

// Sign extends the value from 'sign_bit' onwards
[[nodiscard]] int32_t sign_extend(uint32_t value,
                                  const uint32_t sign_bit) noexcept;

// Shifts 'a' left by 'shamt' bits, filling with zeros
[[nodiscard]] uint32_t sll(const uint32_t a, const uint32_t shamt) noexcept;

// Shifts 'b' right by 'shamt' bits, filling with zeros
[[nodiscard]] uint32_t srl(const uint32_t a, const uint32_t shamt) noexcept;

// Shifts 'a' right by 'shamt' bits, replicating bit 31
[[nodiscard]] uint32_t sra(const uint32_t a, const uint32_t shamt) noexcept;

};  // namespace riscvsim