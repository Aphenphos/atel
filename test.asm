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
	mov	rdi, (null)
	call	printint
	mov	r8, rax
