#pragma once

#include <array>
#include <cstdint>

class Memory {
public:
    static constexpr std::size_t kMemSize = 0x10000; // 64KB
    Memory();

private:
    std::array<std::byte, kMemSize> mem_;


};