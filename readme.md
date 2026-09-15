# RISC-V Simulator

A C++17 simulator that emulates the RISC-V CPU architecture to execute compiled binaries. Implements (most of) the RV32I Base Integer Instruction Set.

## Implementation Details

* The simulator follows a basic fetch-decode-execute cycle (`CPU::run()`)
* Instructions and data are loaded into simulated memory
* Each instruction is decoded according to the instruction formats
* Execution is dispatched by the CPU

Architecture:

* CPU: Manages the PC, register set, memory and cycle
* Memory: Provides storage and read/write methods
* Instruction: Provides instruction definition and decoding
* bit_manipulation: Helper functions

## Requirements

* C++17
* CMake 3.14+
* RISC-V GNU Toolchain (`riscv64-unknown-elf`) for running the tests

## Build

### `riscvsim` binary

```bash
cmake -S . -B build
cmake --build build
```

### Assembly binaries

The project provides `tools/compile_bin.sh` to generate `.bin` files from RISC-V assembly (`.s`) files.

```bash
./tools/compile_bin.sh <input.s> <output dir>
```

## Run

```bash
./build/bin/riscvsim <path-to-bin-file>
```
