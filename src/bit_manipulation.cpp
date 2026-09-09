#include "bit_manipulation.h"

#include <cassert>
#include <cstdio>

namespace riscvsim
{

[[nodiscard]] uint32_t extract_bits(const uint32_t bits, const unsigned int hi,
                                    const unsigned int lo) noexcept
{
    assert(hi < 32 && lo < 32 && hi >= lo);
    uint32_t mask{};
    for (auto i{lo}; i <= hi; ++i)
    {
        mask = mask | (1u << i);
    }
    return (bits & mask) >> lo;
}

[[nodiscard]] uint32_t append_bits(const uint32_t a, const uint32_t b,
                                   const uint8_t shamt) noexcept
{
    return a | (b << shamt);
}

[[nodiscard]] int32_t sign_extend(uint32_t value,
                                  const uint32_t bit_width) noexcept
{
    const uint32_t mask{1U << (bit_width - 1)};
    value = value & ((1U << bit_width) - 1);
    return (value ^ mask) - mask;
}

[[nodiscard]] uint32_t sll(const uint32_t a, const uint32_t shamt) noexcept
{
    return a << (shamt & 0x1F);
}

[[nodiscard]] uint32_t srl(const uint32_t a, const uint32_t shamt) noexcept
{
    return a >> (shamt & 0x1F);
}

[[nodiscard]] uint32_t sra(const uint32_t a, const uint32_t shamt) noexcept
{
    uint32_t shifted{a >> (shamt & 0x1F)};
    return sign_extend(shifted, 32 - shamt);
}

};  // namespace riscvsim
