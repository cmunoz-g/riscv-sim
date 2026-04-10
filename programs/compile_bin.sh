#!/bin/bash

name="${1:0:-2}"
riscv64-unknown-elf-as -march=rv32i -mabi=ilp32 -o "${name}.o" "${name}.s" > /dev/null 2>&1
riscv64-unknown-elf-ld -m elf32lriscv -o "${name}.elf" "${name}.o" > /dev/null 2>&1
riscv64-unknown-elf-objcopy -O binary "${name}.elf" "${name}.bin" > /dev/null
rm "${name}".o
rm "${name}".elf
echo "compiled ${name}"