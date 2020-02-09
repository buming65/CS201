	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 14
	.globl	_gcd                    ## -- Begin function gcd
	.p2align	4, 0x90
_gcd:                                   ## @gcd
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	cmpl	$0, -4(%rbp)
	jne	LBB0_2
## %bb.1:
	movl	-8(%rbp), %eax
	movl	%eax, -12(%rbp)
	jmp	LBB0_9
LBB0_2:
	cmpl	$0, -8(%rbp)
	jne	LBB0_4
## %bb.3:
	movl	-4(%rbp), %eax
	movl	%eax, -12(%rbp)
	jmp	LBB0_9
LBB0_4:
	movl	-4(%rbp), %eax
	cmpl	-8(%rbp), %eax
	jne	LBB0_6
## %bb.5:
	movl	-4(%rbp), %eax
	movl	%eax, -12(%rbp)
	jmp	LBB0_9
LBB0_6:
	movl	-4(%rbp), %eax
	cmpl	-8(%rbp), %eax
	jle	LBB0_8
## %bb.7:
	movl	-4(%rbp), %edi
	subl	-8(%rbp), %edi
	movl	-8(%rbp), %esi
	callq	_gcd
	movl	%eax, -12(%rbp)
	jmp	LBB0_9
LBB0_8:
	movl	-4(%rbp), %edi
	movl	-8(%rbp), %esi
	subl	-4(%rbp), %esi
	callq	_gcd
	movl	%eax, -12(%rbp)
LBB0_9:
	movl	-12(%rbp), %eax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r14
	pushq	%rbx
	subq	$16, %rsp
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	movl	$0, -28(%rbp)
	movl	$98, -24(%rbp)
	movl	$56, -20(%rbp)
	movl	-24(%rbp), %ebx
	movl	-20(%rbp), %r14d
	movl	-24(%rbp), %edi
	movl	-20(%rbp), %esi
	callq	_gcd
	leaq	L_.str(%rip), %rdi
	movl	%ebx, %esi
	movl	%r14d, %edx
	movl	%eax, %ecx
	movb	$0, %al
	callq	_printf
	xorl	%eax, %eax
	addq	$16, %rsp
	popq	%rbx
	popq	%r14
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"GCD of %d and %d is %d "


.subsections_via_symbols
