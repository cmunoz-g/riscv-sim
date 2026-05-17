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
    slt  x28, x6, x7 # 1
    sltu x29, x6, x7 # 0 
    slti x30, x6, 0 # 1
    sltiu x31, x6, 3 # 0 

    mv a0, x28
    addi a7, zero, 1
    ecall
    mv a0, x29
    addi a7, zero, 1
    ecall
    mv a0, x30
    addi a7, zero, 1
    ecall
    mv a0, x31
    addi a7, zero, 1
    ecall

    # SLL, SRL, SRA
    addi x12, x0, 0x80
    addi x13, x0, 2
    sll  x14, x12, x13 # 0x200
    srl  x15, x12, x13 # 0x20
    addi x16, x0, -128 # 0xFFFFFF80
    sra  x17, x16, x13 # 0xFFFFFFE0

    mv a0, x14
    addi a7, zero, 1
    ecall
    mv a0, x15
    addi a7, zero, 1
    ecall
    mv   a0, x17
    addi a7, zero, 1
    ecall

    addi a7, zero, 10
    ecall