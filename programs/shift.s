.text
.globl _start
_start:
    addi x1, x0, 1
    slli x2, x1, 4 # 16
    addi x3, x0, -1 # 0xFFFFFFFF
    srli x4, x3, 4 # 0x0FFFFFFF
    srai x5, x3, 4 # 0xFFFFFFFF

    mv   a0, x2
    addi a7, zero, 1
    ecall
    mv   a0, x4
    addi a7, zero, 1
    ecall
    mv   a0, x5
    addi a7, zero, 1
    ecall

    # SLT, SLTU, SLTI, SLTIU
    addi x6, x0, -5
    addi x7, x0, 3
    slt  x8, x6, x7 # 1 (signed: -5 < 3)
    sltu x9, x6, x7 # 0 (unsigned: big > 3)
    slti x10, x6, 0 # 1 (-5 < 0)
    sltiu x11, x6, 3 # 0 (unsigned big > 3)

    mv   a0, x8
    addi a7, zero, 1
    ecall
    mv   a0, x9
    addi a7, zero, 1
    ecall
    mv   a0, x10
    addi a7, zero, 1
    ecall
    mv   a0, x11
    addi a7, zero, 1
    ecall

    # SLL, SRL, SRA
    addi x12, x0, 0x80
    addi x13, x0, 2
    sll  x14, x12, x13 # 0x200
    srl  x15, x12, x13 # 0x20
    addi x16, x0, -128 # 0xFFFFFF80
    sra  x17, x16, x13 # 0xFFFFFFE0

    mv   a0, x14
    addi a7, zero, 1
    ecall
    mv   a0, x15
    addi a7, zero, 1
    ecall
    mv   a0, x17
    addi a7, zero, 1
    ecall

    addi a7, zero, 10
    ecall