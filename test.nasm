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
	common	i 8:8
	common	j 8:8
	mov	r8, 6
	mov	[i], r8
	mov	r8, 12
	mov	[j], r8
	mov	r8, [i]
	mov	r9, [j]
	cmp	r8, r9
	jge	L1
	mov	r8, [i]
	mov	rdi, r8
	call	printint
	jmp	L2
L1:
	mov	r8, [j]
	mov	rdi, r8
	call	printint
L2:
	mov	eax, 0
	pop	rbp
	ret
