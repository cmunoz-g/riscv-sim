.data
title:      .asciz "First 20 Fibonacci numbers:\n"
space:      .asciz " "
newline:    .asciz "\n"

.text
.globl _start
.globl main

_start:
    .option push
    .option norelax
    la      gp, __global_pointer$
    .option pop

    j       main

main:
    la      a0, title
    li      a7, 4
    ecall

    li      s0, 0
    li      s1, 1
    li      s2, 20

loop:
    mv      a0, s0
    li      a7, 1
    ecall

    la      a0, space
    li      a7, 4
    ecall

    add     t0, s0, s1
    mv      s0, s1
    mv      s1, t0

    addi    s2, s2, -1
    bnez    s2, loop

    la      a0, newline
    li      a7, 4
    ecall

    li      a7, 10
    ecall
    