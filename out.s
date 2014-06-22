.text
.globl _prog

_prog:
push	%ebp
movl	%esp, %ebp

subl	$12, %esp

movl	$2, 0(%esp)
movl	$4, 4(%esp)
movl	$8, 8(%esp)

pushl	0(%esp)
call	_println_int
addl	$4, %esp

pushl	4(%esp)
call	_println_int
addl	$4, %esp

pushl	8(%esp)
call	_println_int
addl	$4, %esp

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

addl	$12, %esp

pop 	%ebp
ret

.data
