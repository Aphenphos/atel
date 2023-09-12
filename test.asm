	extern	printint
	extern	printchar
	section	.data
	global	c
c:	db	0
	section	.data
	global	str
str:	dq	0
	section	.data
	global	anInt
anInt:	dq	0
L6:
	db	34
	db	72
	db	101
	db	108
	db	108
	db	111
	db	32
	db	119
	db	111
	db	114
	db	108
	db	100
	db	92
	db	110
	db	34
	db	0
	section	.text
	global	main
main:
	push	rbp
	mov	rbp, rsp
	mov	r8, 377
	mov	[anInt], r8
	mov	r8b, [anInt]
	mov	rdi, r8
	call	printint
	mov	r9, rax
	mov	r8, 10
	mov	[c], r8b
	movzx	r8, byte [c]
	mov	rdi, r8
	call	printint
	mov	r9, rax
	mov	r8, L6
	mov	[str], r8
L2:
	mov	r11, [str]
	movzx	r8, byte [r8]
	mov	r9, 0
	cmp	r8, r9
	je	L3
	mov	r11, [str]
	movzx	r8, byte [r8]
	mov	rdi, r8
	call	printchar
	mov	r9, rax
	mov	r11, [str]
	mov	r9, 1
	sal	r9, 3
	add	r9, r8
	mov	[str], r9
	jmp	L2
L3:
	mov	r8, 0
	mov	eax, r8d
	jmp	L1
L1:
	mov	eax, 0
	pop	rbp
	ret
