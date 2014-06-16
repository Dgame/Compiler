.text
.globl _prog
_prog:

pushl	%ebp
movl	%esp, %ebp

movl	$4, %eax
pushl	%eax
movl	$2, %eax
movl	%eax, %ebx
popl	%eax
movl	$0, %edx
idiv	%ebx
pushl	%eax
call	_println_int
addl	$4, %esp
movl	$3, %eax
pushl	%eax
movl	$4, %eax
pushl	%eax
movl	$2, %eax
movl	%eax, %ebx
popl	%eax
movl	$0, %edx
idiv	%ebx
imull	0(%esp), %eax
addl	$4, %esp
pushl	%eax
call	_println_int
addl	$4, %esp
movl	$12, %eax
pushl	%eax
movl	$2, %eax
movl	%eax, %ebx
popl	%eax
movl	$0, %edx
idiv	%ebx
pushl	%eax
call	_println_int
addl	$4, %esp
movl	$1, %eax
pushl	%eax
movl	$2, %eax
pushl	%eax
movl	$3, %eax
imull	0(%esp), %eax
addl	$4, %esp
addl	0(%esp), %eax
addl	$4, %esp
pushl	%eax
call	_println_int
addl	$4, %esp
movl	$12, %eax
pushl	%eax
movl	$6, %eax
subl	%eax, 0(%esp)
popl	%eax
pushl	%eax
call	_println_int
addl	$4, %esp
movl	$6, %eax
pushl	%eax
movl	$12, %eax
subl	%eax, 0(%esp)
popl	%eax
pushl	%eax
call	_println_int
addl	$4, %esp
movl	$1, %eax
pushl	%eax
movl	$2, %eax
addl	0(%esp), %eax
addl	$4, %esp
pushl	%eax
movl	$3, %eax
imull	0(%esp), %eax
addl	$4, %esp
pushl	%eax
call	_println_int
addl	$4, %esp
movl	$4, %eax
pushl	%eax
movl	$2, %eax
imull	0(%esp), %eax
addl	$4, %esp
pushl	%eax
movl	$8, %eax
addl	0(%esp), %eax
addl	$4, %esp
pushl	%eax
call	_println_int
addl	$4, %esp
movl	$8, %eax
pushl	%eax
movl	$6, %eax
addl	0(%esp), %eax
addl	$4, %esp
pushl	%eax
movl	$2, %eax
subl	%eax, 0(%esp)
popl	%eax
pushl	%eax
call	_println_int
addl	$4, %esp

popl	%ebp
ret

.data
