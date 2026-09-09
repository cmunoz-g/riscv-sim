.text
.globl _start
_start:
    jal x1, jal_target 
    ecall
jal_target:
    lui x2, 0x1
    ecall