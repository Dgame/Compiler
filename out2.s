.text
.globl _prog
_prog:

pushl	%ebp
movl	%esp, %ebp

movl	$4, %eax
pushl	%eax
movl	$2, %eax
pushl	%eax

cmpl	%eax, 0(%esp)
movl	$0, %eax
sete 	%al
addl	$4, %esp
testl	%eax, %eax
jz		L0


popl	%ebp
ret


.data

L0:


ret
