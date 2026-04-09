#pragma once

#include <array>
#include <cstdint>

class Memory {
public:
    static constexpr std::size_t kMemSize = 0x10000; // 64KB
    
    Memory(const uint8_t *data, const std::size_t data_size);
    bool read8(const uint32_t addr, uint8_t &val) const;
    bool read16(const uint32_t addr, uint16_t &val) const;
    bool read32(const uint32_t addr, uint32_t &val) const;
    bool write8(const uint32_t addr, const uint8_t val);
    bool write16(const uint32_t addr, const uint16_t val);
    bool write32(const uint32_t addr, const uint32_t val);

private:
    std::array<uint8_t, kMemSize> mem_;
};