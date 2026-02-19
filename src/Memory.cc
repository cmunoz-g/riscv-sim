#include "Memory.h"
#include <algorithm>
#include <cstdio>
#include <cstddef>

Memory::Memory(const uint8_t buf[], const std::size_t buf_size) {
    std::copy_n(buf, buf_size, mem_.begin());
}

uint8_t Memory::read8(const uint32_t addr) const {
    if (addr > (kMemSize - sizeof(uint8_t))) {
        return -1;
    }
    return mem_[addr];
}

uint16_t Memory::read16(const uint32_t addr) const {
    if (addr > (kMemSize - sizeof(uint16_t))) {
        return -1;
    }
    return mem_[addr] | (mem_[addr + 1] << 8);
}

uint32_t Memory::read32(const uint32_t addr) const {
    if (addr > (kMemSize - sizeof(uint32_t))) {
        return -1;
    }
    uint16_t lower_16 = mem_[addr] | (mem_[addr + 1] << 8);
    uint16_t upper_16 = mem_[addr + 2] | (mem_[addr + 3] << 8);
    return lower_16 | (upper_16 << 16);
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