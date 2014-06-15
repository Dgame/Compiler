.globl _prog

_prog:
push	%ebp
movl	%esp, %ebp

subl	$8, %esp # reserve

movl	$4, %eax # 4
movl	%eax, (%esp) # a = 4

movl	(%esp), %eax # param : a(4)
push	%eax
call	_print_int # print param : a(4)
addl	$4, %esp

movl	(%esp), %eax # param : a(4)
push	%eax
movl	$2, %eax # 2
imull	4(%esp), %eax # * 2
addl	$4, %esp
movl	%eax, 4(%esp) # b = a * 2 = 8

movl	4(%esp), %eax # param : b(8)
push	%eax
call	_print_int # print param : b(8)
addl	$4, %esp

addl	$8, %esp # release

pop 	%ebp
ret
