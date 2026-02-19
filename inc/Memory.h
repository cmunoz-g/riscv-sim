#pragma once

#include <array>
#include <cstdint>

class Memory {
public:
    static constexpr std::size_t kMemSize = 0x10000; // 64KB
    
    Memory(const uint8_t buf[], const std::size_t buf_size);
    uint8_t read8(const uint32_t addr) const;
    uint16_t read16(const uint32_t addr) const;
    uint32_t read32(const uint32_t addr) const;
    bool write8(const uint32_t addr, const uint8_t val);
    bool write16(const uint32_t addr, const uint16_t val);
    bool write32(const uint32_t addr, const uint32_t val);

private:
    std::array<uint8_t, kMemSize> mem_;

};