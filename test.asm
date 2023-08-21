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
	section	.text
	global	main
main:
	push	rbp
	mov	rbp, rsp
	mov	r9, 10
	mov	rdi, r9
	call	printint
	mov	r8, 15
	mov	rdi, r8
	call	fred
	mov	r9, rax
	mov	[result], r9d
	xor	r8, r8
	mov	r8d, dword [result]
	mov	rdi, r8
	call	printint
	mov	r8, 15
	mov	rdi, r8
	call	fred
	mov	r9, rax
	mov	r8, 10
	add	r8, r9
	mov	rdi, r8
	call	printint
L2:
	mov	eax, 0
	pop	rbp
	ret
