.data
title:
    .asciz "Tower of Hanoi - 4 disks\n\n"

move_text:
    .asciz "Move disk "

from_text:
    .asciz " from "

to_text:
    .asciz " to "

newline:
    .asciz "\n"

.balign 16
stack_bottom:
    .space 4096
stack_top:

.text
.globl _start
.globl main

_start:
    .option push
    .option norelax
    la      gp, __global_pointer$
    .option pop

    la      sp, stack_top

    j       main

main:
    la      a0, title
    li      a7, 4
    ecall

    li      a0, 4
    li      a1, 'A'
    li      a2, 'C'
    li      a3, 'B'

    call    hanoi

    li      a7, 10
    ecall

hanoi:
    beqz    a0, hanoi_return

    addi    sp, sp, -32

    sw      ra, 28(sp)
    sw      a0, 24(sp)
    sw      a1, 20(sp)
    sw      a2, 16(sp)
    sw      a3, 12(sp)

    addi    a0, a0, -1

    mv      t0, a2
    mv      a2, a3
    mv      a3, t0

    call    hanoi

    lw      a0, 24(sp)
    lw      a1, 20(sp)
    lw      a2, 16(sp)
    lw      a3, 12(sp)

    la      a0, move_text
    li      a7, 4
    ecall

    lw      a0, 24(sp)
    li      a7, 1
    ecall

    la      a0, from_text
    li      a7, 4
    ecall

    lw      a0, 20(sp)
    li      a7, 11
    ecall

    la      a0, to_text
    li      a7, 4
    ecall

    lw      a0, 16(sp)
    li      a7, 11
    ecall

    la      a0, newline
    li      a7, 4
    ecall

    lw      a0, 24(sp)
    lw      a1, 20(sp)
    lw      a2, 16(sp)
    lw      a3, 12(sp)

    addi    a0, a0, -1

    mv      t0, a1
    mv      a1, a3
    mv      a3, t0

    call    hanoi

    lw      ra, 28(sp)
    addi    sp, sp, 32

hanoi_return:
    ret
