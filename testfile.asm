; This is a test file
; Containing some random Assembly code.
halt
NOP
Nop
nOp
nop

push	hl
pop		BC
popa
pushA

Label:
	add A
	SUB B
	and a, b, a

	.define 0x12
	.db 0x10, 10, 010, 0b10
	.include "myfile.asm"
