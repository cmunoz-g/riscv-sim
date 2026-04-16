.data
str: .asciz "This is for sure a string\n"

.text
.globl _start
_start:
    # PRINT_INTEGER
    li a0, 10
    li a7, 1
    ecall

    # PRIINT_STRING
    la a0, str
    li a7, 4
    ecall

    # PRINT_CHARACTER
    li a0, '!'
    li a7, 11
    ecall

    # EXIT_WITH_CODE
    li a0, 100
    li a7, 17
    ecall
    
    # EXIT tested in all other binaries