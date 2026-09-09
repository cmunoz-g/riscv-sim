#!/bin/bash

set -e

input="$1"
output_dir="${2:-.}"

name=$(basename "$input" .s)

riscv64-unknown-elf-as -march=rv32i -mabi=ilp32 -o "${output_dir}/${name}.o" "$input" > /dev/null
riscv64-unknown-elf-ld -m elf32lriscv -o "${output_dir}/${name}.elf" "${output_dir}/${name}.o" > /dev/null
riscv64-unknown-elf-objcopy -O binary "${output_dir}/${name}.elf" "${output_dir}/${name}.bin" > /dev/null

rm -f "${output_dir}/${name}".o
rm -f "${output_dir}/${name}".elf

echo "compiled ${output_dir}/${name}.bin"
