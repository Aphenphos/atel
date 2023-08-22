	global	main
	extern	printf
	section	.text
LC0:	db	"%d",10,0
printint:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	[rbp-4], edi
	mov	eax, [rbp-4]
	mov	esi, eax
	lea	rdi, [rel LC0]
	mov	eax, 0
	call	printf
	nop
	leave
	ret

	common	x 4:4
	common	y 4:4
	common	iptr 8:8
	section	.text
	global	main
main:
	push	rbp
	mov	rbp, rsp
	mov	r8, 10
	mov	[x], r8d
	mov	r8, 20
	mov	[y], r8d
	mov	r8, x
	mov	r9, 1
	add	r9, r8
	mov	[iptr], r9
	mov	r11, [iptr]
	movzx	r8, word [r8]
	mov	rdi, r8
	call	printint
	mov	r8, 0
	movzx	eax, r8d
	jmp	L1
L1:
	mov	eax, 0
	pop	rbp
	ret
