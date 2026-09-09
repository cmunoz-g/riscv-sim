#pragma once

#include <array>
#include <cstdint>

namespace riscvsim
{
class Memory
{
public:
    static constexpr std::size_t mem_size = 0x10000;  // 64 kB

    explicit Memory(const uint8_t* data, const std::size_t data_size);

    // Reads a single byte
    [[nodiscard]] bool read8(const uint32_t addr,
                             uint8_t& value) const noexcept;

    // Reads a half word
    [[nodiscard]] bool read16(const uint32_t addr,
                              uint16_t& value) const noexcept;

    // Reads a word
    [[nodiscard]] bool read32(const uint32_t addr,
                              uint32_t& value) const noexcept;

    // Writes a single byte
    [[nodiscard]] bool write8(const uint32_t addr,
                              const uint8_t value) noexcept;

    // Writes a half word
    [[nodiscard]] bool write16(const uint32_t addr,
                               const uint16_t value) noexcept;

    // Writes a word
    [[nodiscard]] bool write32(const uint32_t addr,
                               const uint32_t value) noexcept;

private:
    std::array<uint8_t, mem_size> memory_;
};

};  // namespace riscvsim