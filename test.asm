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

	common	d 4:4
	common	f 4:4
	common	e 8:8
	common	a 4:4
	common	b 4:4
	common	c 4:4
	section	.text
	global	main
main:
	push	rbp
	mov	rbp, rsp
	mov	r8, 3
	mov	[b], r8d
	mov	r8, 5
	mov	[c], r8d
	xor	r8, r8
	mov	r8d, dword [b]
	xor	r9, r9
	mov	r9d, dword [c]
	mov	r10, 10
	imul	r10, r9
	add	r10, r8
	mov	[a], r10d
	xor	r8, r8
	mov	r8d, dword [a]
	mov	rdi, r8
	call	printint
	mov	r8, 12
	mov	[d], r8d
	xor	r8, r8
	mov	r8d, dword [d]
	mov	rdi, r8
	call	printint
	mov	r8, d
	mov	[e], r8
	mov	r10, [e]
	movzx	r8, word [r8]
	mov	[f], r8d
	xor	r8, r8
	mov	r8d, dword [f]
	mov	rdi, r8
	call	printint
	mov	r8, 0
	movzx	eax, r8d
	jmp	L1
L1:
	mov	eax, 0
	pop	rbp
	ret
	mov	eax, 0
	pop	rbp
	ret
