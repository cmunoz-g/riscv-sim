.data
array:
    .word 42, 7, 19, -3, 88, 15, 0, 31, 4, 12

before:
    .asciz "Before sorting:\n"

after:
    .asciz "After sorting:\n"

space:
    .asciz " "

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

    jal     ra, main

    li      a7, 10
    ecall

main:
    addi    sp, sp, -16
    sw      ra, 12(sp)
    sw      s0, 8(sp)
    sw      s1, 4(sp)

    la      a0, before
    li      a7, 4
    ecall

    la      a0, array
    li      a1, 10
    jal     ra, print_array

    la      s0, array
    li      s1, 10

outer_loop:
    li      t0, 0
    li      t1, 0

    addi    t2, s1, -1

inner_loop:
    slli    t3, t0, 2
    add     t4, s0, t3

    lw      t5, 0(t4)
    lw      t6, 4(t4)

    ble     t5, t6, no_swap

    sw      t6, 0(t4)
    sw      t5, 4(t4)

    li      t1, 1

no_swap:
    addi    t0, t0, 1
    blt     t0, t2, inner_loop

    beqz    t1, sorting_done

    addi    s1, s1, -1

    li      t0, 1
    bgt     s1, t0, outer_loop

sorting_done:
    la      a0, after
    li      a7, 4
    ecall

    la      a0, array
    li      a1, 10
    jal     ra, print_array

    lw      s1, 4(sp)
    lw      s0, 8(sp)
    lw      ra, 12(sp)
    addi    sp, sp, 16

    ret

print_array:
    addi    sp, sp, -16

    sw      ra, 12(sp)
    sw      s0, 8(sp)
    sw      s1, 4(sp)
    sw      s2, 0(sp)

    mv      s0, a0
    mv      s1, a1
    li      s2, 0

print_loop:
    beq     s2, s1, print_done

    lw      a0, 0(s0)
    li      a7, 1
    ecall

    la      a0, space
    li      a7, 4
    ecall

    addi    s0, s0, 4
    addi    s2, s2, 1

    j       print_loop

print_done:
    la      a0, newline
    li      a7, 4
    ecall

    lw      s2, 0(sp)
    lw      s1, 4(sp)
    lw      s0, 8(sp)
    lw      ra, 12(sp)

    addi    sp, sp, 16
    ret
    