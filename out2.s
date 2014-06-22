.text
.globl _prog
_prog:

pushl	%ebp
movl	%esp, %ebp

subl	$36, %esp

movl	$4, 0(%esp)

movl	$8, 4(%esp)

movl	0(%esp), %eax
imull	4(%esp), %eax
pushl	%eax
call	_println_int
addl	$4, %esp

movl	4(%esp), %eax
imull	0(%esp), %eax
pushl	%eax
call	_println_int
addl	$4, %esp

movl	0(%esp), %eax
addl	4(%esp), %eax
movl	%eax, 8(%esp)

pushl	8(%esp)
call	_println_int
addl	$4, %esp

movl	4(%esp), %eax
addl	0(%esp), %eax
movl	%eax, 12(%esp)

pushl	12(%esp)
call	_println_int
addl	$4, %esp

movl	0(%esp), %eax
imull	4(%esp), %eax
movl	%eax, 16(%esp)

pushl	16(%esp)
call	_println_int
addl	$4, %esp

movl	0(%esp), %eax
subl	4(%esp), %eax
movl	%eax, 20(%esp)

pushl	20(%esp)
call	_println_int
addl	$4, %esp

movl	4(%esp), %eax
subl	0(%esp), %eax
movl	%eax, 24(%esp)

pushl	24(%esp)
call	_println_int
addl	$4, %esp

movl	0(%esp), %eax
movl	$2, %eax
imull	0(%esp), %eax
addl	4(%esp), %eax
movl	%eax, 28(%esp)

pushl	28(%esp)
call	_println_int
addl	$4, %esp

movl	4(%esp), %eax
movl	$0, %edx
movl	0(%esp), %ebx
idiv	%ebx
movl	%eax, 32(%esp)

pushl	32(%esp)
call	_println_int
addl	$4, %esp

movl	$4, 0(%esp)

movl	0(%esp), %eax
movl	$11, %eax
imull	0(%esp), %eax
pushl	%eax
call	_println_int
addl	$4, %esp

movl	$2, 0(%esp)

movl	$4, 4(%esp)

movl	$8, 8(%esp)

movl	4(%esp), %eax
imull	8(%esp), %eax
addl	0(%esp), %eax
pushl	%eax
call	_println_int
addl	$4, %esp

movl	$4, %eax
imull	8(%esp), %eax
addl	0(%esp), %eax
pushl	%eax
call	_println_int
addl	$4, %esp

addl	$36, %esp

popl	%ebp
ret


.data
