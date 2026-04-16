.text
.globl _start
_start:
    # ADD, SUB, ADDI
    addi x1, x0, 10
    addi x2, x0, 20
    add x3, x1, x2  # 30
    sub x4, x2, x1 # 10

    mv a0, x3
    addi a7, zero, 1
    ecall

    mv a0, x4
    addi a7, zero, 1
    ecall

    # AND, OR, XOR
    addi x5, x0, 0xFF
    addi x6, x0, 0x0F
    and x7, x5, x6 # 0x0F
    or x8, x5, x6 # 0xFF
    xor x9, x5, x6 # 0xF0

    mv a0, x7
    addi a7, zero, 1
    ecall
    mv a0, x8
    addi a7, zero, 1
    ecall
    mv a0, x9
    addi a7, zero, 1
    ecall

    # ANDI, ORI, XORI
    andi x10, x5, 0x0F # 0x0F
    ori x11, x6, 0xF0 # 0xFF
    xori x12, x5, 0xFF # 0x00

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
