	extern	printint
	section	.data
	global	a
a:	dd	0
	section	.data
	global	b
b:	dq	0
	dq	0
	dq	0
	dq	0
	dq	0
	dq	0
	dq	0
	dq	0
	dq	0
	dq	0
	dq	0
	dq	0
	dq	0
	dq	0
	dq	0
	dq	0
	dq	0
	dq	0
	dq	0
	dq	0
	dq	0
	dq	0
	dq	0
	dq	0
	dq	0
	section	.text
	global	main
main:
	push	rbp
	mov	rbp, rsp
	mov	r8, 12
	mov	r9, b
	mov	r10, 3
	sal	r10, 3
	add	r10, r9
	mov	[r10], r8
	mov	r8, b
	mov	r9, 3
	sal	r9, 3
	add	r9, r8
	movzx	r9, word [r9]
	mov	[a], r9d
	xor	r8, r8
	mov	r8d, dword [a]
	mov	rdi, r8
	call	printint
	mov	r9, rax
	mov	r8, 0
	mov	eax, r8d
	jmp	L1
L1:
	mov	eax, 0
	pop	rbp
	ret
