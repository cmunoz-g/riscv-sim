#include "Memory.h"
#include <algorithm>
#include <cstdio>
#include <cstddef>

Memory::Memory(const uint8_t *data, const std::size_t data_size) {
    std::copy_n(data, data_size, mem_.begin());
}

bool Memory::read8(const uint32_t addr, uint8_t &val) const {
    if (addr > (kMemSize - sizeof(uint8_t))) {
        return false;
    }
    val = mem_[addr];
    return true;
}

bool Memory::read16(const uint32_t addr, uint16_t &val) const {
    if (addr > (kMemSize - sizeof(uint16_t))) {
        return false;
    }
    val = mem_[addr] | (mem_[addr + 2] << 8);
    return true;
}

bool Memory::read32(const uint32_t addr, uint32_t &val) const {
    if (addr > (kMemSize - sizeof(uint32_t))) {
        return false;
    }
    uint16_t lower_16 = mem_[addr] | (mem_[addr + 1] << 8);
    uint16_t upper_16 = mem_[addr + 2] | (mem_[addr + 3] << 8);
    val = lower_16 | (upper_16 << 16);
    return true;
}

bool Memory::write8(const uint32_t addr, const uint8_t val) { 
    if (addr > (kMemSize - sizeof(uint8_t))) {
        return false;
    }
    mem_[addr] = val;
    return true;
}

bool Memory::write16(const uint32_t addr, const uint16_t val) {
    if (addr > (kMemSize - sizeof(uint16_t))) {
        return false;
    }
    mem_[addr] = val & 0xFF;
    mem_[addr + 2] = (val >> 8) & 0xFF;
    return true;
}

bool Memory::write32(const uint32_t addr, const uint32_t val) {
    if (addr > (kMemSize - sizeof(uint32_t))) {
        return -1;
    }
    mem_[addr] = val & 0xFF;
    mem_[addr + 1] = (val >> 8) & 0xFF;
    mem_[addr + 2] = (val >> 16) & 0xFF;
    mem_[addr + 3] = (val >> 24) & 0xFF;
    
    return true;
}