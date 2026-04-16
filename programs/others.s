.text
.globl _start
_start:
    # LUI
    lui  x1, 0xDEADB
    mv   a0, x1
    addi a7, zero, 1
    ecall

    # AUIPC
    auipc x2, 0
    mv   a0, x2
    addi a7, zero, 1
    ecall

    # JAL
    jal  x3, jal_target
    addi a0, zero, -1
    addi a7, zero, 1
    ecall
jal_target:
    mv   a0, x3
    addi a7, zero, 1
    ecall

    # JALR
    la   x4, jalr_target
    jalr x5, x4, 0
    addi a0, zero, -1
    addi a7, zero, 1
    ecall
jalr_target:
    mv   a0, x5
    addi a7, zero, 1
    ecall

    addi a7, zero, 10
    ecall