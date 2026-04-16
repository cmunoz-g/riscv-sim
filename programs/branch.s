.text
.globl _start
_start:
    addi x1, x0, 5
    addi x2, x0, 5
    addi x3, x0, 10
    addi x4, x0, -3

    # BEQ, should branch
    beq  x1, x2, beq_pass
    addi a0, zero, 0
    addi a7, zero, 1
    ecall
beq_pass:
    addi a0, zero, 1
    addi a7, zero, 1
    ecall

    # BNE, should branch
    bne  x1, x3, bne_pass
    addi a0, zero, 0
    addi a7, zero, 1
    ecall
bne_pass:
    addi a0, zero, 1
    addi a7, zero, 1
    ecall

    # BLT signed — -3 < 5, should branch
    blt  x4, x1, blt_pass
    addi a0, zero, 0
    addi a7, zero, 1
    ecall
blt_pass:
    addi a0, zero, 1
    addi a7, zero, 1
    ecall

    # BGE signed — 5 >= 5, should branch
    bge  x1, x2, bge_pass
    addi a0, zero, 0
    addi a7, zero, 1
    ecall
bge_pass:
    addi a0, zero, 1
    addi a7, zero, 1
    ecall

    # BLTU unsigned — 5 < 0xFFFFFFFD? no. should NOT branch
    bltu x1, x4, bltu_fail
    addi a0, zero, 1
    addi a7, zero, 1
    ecall
    j    bltu_done
bltu_fail:
    addi a0, zero, 0
    addi a7, zero, 1
    ecall
bltu_done:

    # BGEU unsigned — 0xFFFFFFFD >= 5, should branch
    bgeu x4, x1, bgeu_pass
    addi a0, zero, 0
    addi a7, zero, 1
    ecall
bgeu_pass:
    addi a0, zero, 1
    addi a7, zero, 1
    ecall

    addi a7, zero, 10
    ecall