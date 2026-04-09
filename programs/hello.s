.data
hello: .asciz "Hello world!"

.text
main:
	la a0, hello
	addi a7, zero, 4
	ecall

	addi a7, zero, 10
	ecall