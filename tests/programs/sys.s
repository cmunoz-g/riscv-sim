.data
str: .asciz "test"

.text
.globl _start
_start:
    li a0, 'a'
    li a7, 11
    ecall
    li a0, 1
    li a7, 1
    ecall
    la a0, str
    li a7, 4
    ecall
    li a7, 10
    ecall

