.text
.globl _prog
_prog:

pushl	%ebp
movl	%esp, %ebp

subl	$12, %esp

movl	$4, 0(%esp)
movl	$8, 4(%esp)
movl	$16, 8(%esp)
pushl	0(%esp)
call	_println_int
addl	$4, %esp
pushl	4(%esp)
call	_println_int
addl	$4, %esp
pushl	8(%esp)
call	_println_int
addl	$4, %esp

addl	$12, %esp

popl	%ebp
ret

.data
