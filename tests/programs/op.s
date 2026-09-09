.text
.globl _start
_start:

    # x11=1 x12=2 x13=-1


    add x1, x12, x11
    sub x2, x12, x11
    sll x3, x12, x11
    slt x4, x11, x12
    sltu x5, x13, x12 
    srl x6, x12, x11
    sra x7, x13, x11
    xor x8, x12, x11
    or x9, x12, x11
    and x10, x12, x11
    ecall