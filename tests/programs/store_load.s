.text
.globl _start
_start:
    sb x2, 0(x1)
    sh x2, 4(x1)
    sw x2, 20(x1)
    lb x3, 0(x1)
    lbu x4, 0(x1)
    lh x5, 4(x1)
    lhu x6, 4(x1)
    lw x7, 20(x1)
    ecall
