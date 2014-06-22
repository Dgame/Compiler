.text
.globl _prog

_prog:
push	%ebp
movl	%esp, %ebp

subl	$8, %esp

movl	$42, 0(%esp)
movl	$23, 4(%esp)

movl	0(%esp), %eax
movl	%eax, 4(%esp)

addl	$8, %esp

pop 	%ebp
ret

.data
