#pragma once
#include <cstdint>

uint32_t extract_bits(uint32_t bits, uint8_t hi, uint8_t lo);
uint32_t append_bits(uint32_t a, uint32_t b, uint8_t shamt);
int32_t sign_extend(uint32_t val, uint32_t sign_bit);
uint32_t sll(const uint32_t a, const uint32_t shamt);
uint32_t srl(const uint32_t a, const uint32_t shamt);
uint32_t sra(const uint32_t a, const uint32_t shamt);