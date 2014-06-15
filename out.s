.text
.globl _prog

_prog:
push	%ebp
movl	%esp, %ebp

subl	$8, %esp
movl	$1, %eax
push	%eax
movl	$2, %eax
push	%eax
movl	$3, %eax
imull	(%esp), %eax
addl	$4, %esp
addl	(%esp), %eax
addl	$4, %esp
push	%eax
call	_print_int
addl	$4, %esp
movl	$3, %eax
push	%eax
movl	$4, %eax
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
movl	$1, %eax
push	%eax
movl	$2, %eax
addl	(%esp), %eax
addl	$4, %esp
push	%eax
movl	$3, %eax
imull	(%esp), %eax
addl	$4, %esp
push	%eax
call	_print_int
addl	$4, %esp
movl	$3, %eax
push	%eax
movl	$4, %eax
push	%eax
movl	$2, %eax
movl	%eax, %ebx
pop 	%eax
movl	$0, %edx
idiv	%ebx
imull	(%esp), %eax
addl	$4, %esp
push	%eax
call	_print_int
addl	$4, %esp
movl	$4, %eax
movl	%eax, (%esp)
movl	(%esp), %eax
push	%eax
call	_print_int
addl	$4, %esp
movl	(%esp), %eax
push	%eax
movl	$2, %eax
imull	4(%esp), %eax
addl	$4, %esp
movl	%eax, 4(%esp)
movl	4(%esp), %eax
push	%eax
call	_print_int
addl	$4, %esp
addl	$8, %esp

pop 	%ebp
ret
