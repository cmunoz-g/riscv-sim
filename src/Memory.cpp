#include "Memory.h"

#include <algorithm>
#include <cstddef>
#include <iostream>

namespace riscvsim
{

Memory::Memory(const uint8_t* data, const std::size_t data_size)
{
    if (data_size > mem_size)
    {
        std::cout << "error: image larger than RAM: " << data_size << "\n";
        std::exit(1);
    }
    std::copy_n(data, data_size, memory_.begin());
}

[[nodiscard]] bool Memory::read8(const uint32_t addr,
                                 uint8_t& value) const noexcept
{
    if (addr > (mem_size - sizeof(uint8_t)))
    {
        return false;
    }
    value = memory_[addr];
    return true;
}

[[nodiscard]] bool Memory::read16(const uint32_t addr,
                                  uint16_t& value) const noexcept
{
    if (addr > (mem_size - sizeof(uint16_t)))
    {
        return false;
    }
    value = memory_[addr] | (memory_[addr + 1] << 8);
    return true;
}

[[nodiscard]] bool Memory::read32(const uint32_t addr,
                                  uint32_t& value) const noexcept
{
    if (addr > (mem_size - sizeof(uint32_t)))
    {
        return false;
    }
    uint16_t lower_16{
        static_cast<uint16_t>(memory_[addr] | (memory_[addr + 1] << 8))};
    uint16_t upper_16{
        static_cast<uint16_t>(memory_[addr + 2] | (memory_[addr + 3] << 8))};
    value = lower_16 | (upper_16 << 16);
    return true;
}

[[nodiscard]] bool Memory::write8(const uint32_t addr,
                                  const uint8_t value) noexcept
{
    if (addr > (mem_size - sizeof(uint8_t)))
    {
        return false;
    }
    memory_[addr] = value;
    return true;
}

[[nodiscard]] bool Memory::write16(const uint32_t addr,
                                   const uint16_t value) noexcept
{
    if (addr > (mem_size - sizeof(uint16_t)))
    {
        return false;
    }
    memory_[addr] = value & 0xFF;
    memory_[addr + 1] = (value >> 8) & 0xFF;
    return true;
}

[[nodiscard]] bool Memory::write32(const uint32_t addr,
                                   const uint32_t value) noexcept
{
    if (addr > (mem_size - sizeof(uint32_t)))
    {
        return false;
    }
    memory_[addr] = value & 0xFF;
    memory_[addr + 1] = (value >> 8) & 0xFF;
    memory_[addr + 2] = (value >> 16) & 0xFF;
    memory_[addr + 3] = (value >> 24) & 0xFF;
    return true;
}

};  // namespace riscvsim