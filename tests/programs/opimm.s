.text
.globl _start
_start:
    addi x1, x10, 0x10
    slti x2, x11, 0x1
    sltiu x3, x11, 0x1
    andi x4, x10, 0x3
    ori x5, x10, 0x3
    xori x6, x10, 0x2
    slli x7, x10, 0x1
    srli x8, x12, 0x1
    srai x9, x12, 0x1
    ecall