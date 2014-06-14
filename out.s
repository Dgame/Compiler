.text
.globl _prog

_prog:
push	%ebp
movl	%esp, %ebp

subl	$12, %esp
movl	$4, %eax
push	%eax
movl	$3, %eax
imull	(%esp), %eax
addl	$4, %esp
push	%eax
movl	$2, %eax
movl	%eax, %ebx
pop 	%eax
movl	$0, %edx
idiv	%ebx
push	%eax
call	_print_int
addl	$4, %esp
addl	$12, %esp

pop 	%ebp
ret
