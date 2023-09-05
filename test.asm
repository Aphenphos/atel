	extern	printint
	common	c 4:4
	common	d 4:4
	common	e 8:8
	common	f 4:4
	section	.text
	global	main
main:
	push	rbp
	mov	rbp, rsp
	mov	r8, 12
	mov	[c], r8d
	mov	r8, 18
	mov	[d], r8d
	xor	r8, r8
	mov	r8d, dword [c]
	mov	rdi, r8
	call	printint
	mov	r8, c
	mov	r9, 1
	sal	r9, 3
	add	r9, r8
	mov	[e], r9
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
