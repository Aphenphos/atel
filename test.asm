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

	section	.text
	global	fred
fred:
	push	rbp
	mov	rbp, rsp
	mov	r8, 20
	movzx	eax, r8d
	jmp	L1
L1:
	mov	eax, 0
	pop	rbp
	ret
	common	result 4:4
