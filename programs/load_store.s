.text
.globl _start
_start:
    # set base address
    addi x1, x0, 0x200

    # SW / LW
    addi x2, x0, 0x2A
    sw   x2, 0(x1)
    lw   x3, 0(x1)
    mv   a0, x3
    addi a7, zero, 1
    ecall

    # SB / LB / LBU
    addi x4, x0, -1
    sb   x4, 4(x1)
    lb   x5, 4(x1)
    lbu  x6, 4(x1)
    mv   a0, x5
    addi a7, zero, 1
    ecall
    mv   a0, x6
    addi a7, zero, 1
    ecall

    # SH / LH / LHU
    lui  x7, 0xFFFFF
    sh   x7, 8(x1)
    lh   x8, 8(x1)
    lhu  x9, 8(x1)
    mv   a0, x8
    addi a7, zero, 1
    ecall
    mv   a0, x9
    addi a7, zero, 1
    ecall

    addi a7, zero, 10
    ecall