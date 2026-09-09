.text
.globl _start
# x1=0, x2=0, x3=-1, x4=1
_start:
    beq x1, x2, beq_pass
    ecall
beq_pass:
    bne x1, x3, bne_pass
    ecall
bne_pass:
    blt x3, x4, blt_pass
    ecall
blt_pass:
    bge x1, x2, bge_pass
    ecall
bge_pass:
    bltu x4, x3, bltu_pass
    ecall
bltu_pass:
    bgeu x3, x4, bgeu_pass
    ecall
bgeu_pass:
    beq x1, x3, fail
    bne x1, x2, fail
    blt x4, x3, fail
    bge x1, x4, fail
    bltu x3, x4, fail
    bgeu x4, x3, fail 
    lui x5, 0x1
    ecall
fail:
    ecall
