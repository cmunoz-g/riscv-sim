.text
.globl _start
_start:
    auipc x1, 0
    jalr x2, x1, 12
    ecall
    lui x3, 0x1
    ecall