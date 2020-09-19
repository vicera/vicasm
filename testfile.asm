; This is a test file
; Containing some random Assembly code.
mov sp, 0xffef
jp main		; More comments.

HelloWorld:
	MOV		 a, B
	mov A,	     C      

	add    (hl)
	   sub  (HL)
	mov  (0xff0f),    a
	ret

main:
	call HelloWorld
	jp main
