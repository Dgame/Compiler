.text
.globl _prog

_prog:
push	%ebp
movl	%esp, %ebp

subl	$8, %esp
movl	$4, %eax
movl	%eax, (%esp)
movl	(%esp), %eax
push	%eax
movl	$2, %eax
imull	4(%esp), %eax
addl	$4, %esp
movl	%eax, 4(%esp)
push	$L0
call	_println_string
addl	$4, %esp
movl	(%esp), %eax
push	%eax
call	_print_int
addl	$4, %esp
push	$L1
call	_print_string
addl	$4, %esp
movl	4(%esp), %eax
push	%eax
call	_print_int
addl	$4, %esp
push	$L2
call	_print_string
addl	$4, %esp
movl	(%esp), %eax
push	%eax
movl	4(%esp), %eax
addl	8(%esp), %eax
addl	$4, %esp
push	%eax
call	_println_int
addl	$4, %esp
addl	$8, %esp

pop 	%ebp
ret

.data
L0:.ascii "Hallo Welt\n\0"
L1:.ascii " + \0"
L2:.ascii " = \0"
