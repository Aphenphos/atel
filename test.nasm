	global	_start
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

_start:
	push	rbp
	mov	rbp, rsp
	common	x 8:8
	mov	r8, 7
	mov	r9, 9
	cmp	r8, r9
	setl 	r9b
	and	r9, 255
	mov	[x], r9
	mov	r8, [x]
	mov	rdi, r8
	call	printint
	mov	r8, 7
	mov	r9, 9
	cmp	r8, r9
	setle 	r9b
	and	r9, 255
	mov	[x], r9
	mov	r8, [x]
	mov	rdi, r8
	call	printint
	mov	r8, 7
	mov	r9, 9
	cmp	r8, r9
	setne 	r9b
	and	r9, 255
	mov	[x], r9
	mov	r8, [x]
	mov	rdi, r8
	call	printint
	mov	r8, 7
	mov	r9, 7
	cmp	r8, r9
	sete 	r9b
	and	r9, 255
	mov	[x], r9
	mov	r8, [x]
	mov	rdi, r8
	call	printint
	mov	r8, 7
	mov	r9, 7
	cmp	r8, r9
	setge 	r9b
	and	r9, 255
	mov	[x], r9
	mov	r8, [x]
	mov	rdi, r8
	call	printint
	mov	r8, 7
	mov	r9, 7
	cmp	r8, r9
	setle 	r9b
	and	r9, 255
	mov	[x], r9
	mov	r8, [x]
	mov	rdi, r8
	call	printint
	mov	r8, 9
	mov	r9, 7
	cmp	r8, r9
	setg 	r9b
	and	r9, 255
	mov	[x], r9
	mov	r8, [x]
	mov	rdi, r8
	call	printint
	mov	r8, 9
	mov	r9, 7
	cmp	r8, r9
	setge 	r9b
	and	r9, 255
	mov	[x], r9
	mov	r8, [x]
	mov	rdi, r8
	call	printint
	mov	r8, 9
	mov	r9, 7
	cmp	r8, r9
	setne 	r9b
	and	r9, 255
	mov	[x], r9
	mov	r8, [x]
	mov	rdi, r8
	call	printint
	mov	eax, 0
	pop	rbp
	ret
