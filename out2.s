.text
.globl _prog
_prog:

pushl	%ebp
movl	%esp, %ebp

subl	$12, %esp

movl	$4, %eax
pushl	%eax
call	_println_int
addl	$4, %esp

movl	$8, %eax
pushl	%eax
call	_println_int
addl	$4, %esp

movl	$42, %eax
negl	%eax
pushl	%eax
call	_println_int
addl	$4, %esp

pushl	$4
movl	$2, %eax
movl	%eax, %ebx
popl	%eax
movl	$0, %edx
idiv	%ebx
pushl	%eax
call	_println_int
addl	$4, %esp

pushl	$3
pushl	$4
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

pushl	$12
movl	$2, %eax
movl	%eax, %ebx
popl	%eax
movl	$0, %edx
idiv	%ebx
pushl	%eax
call	_println_int
addl	$4, %esp

pushl	$1
pushl	$2
movl	$3, %eax
imull	0(%esp), %eax
addl	$4, %esp
addl	0(%esp), %eax
addl	$4, %esp
pushl	%eax
call	_println_int
addl	$4, %esp

pushl	$12
movl	$6, %eax
subl	%eax, 0(%esp)
popl	%eax
pushl	%eax
call	_println_int
addl	$4, %esp

pushl	$6
movl	$12, %eax
subl	%eax, 0(%esp)
popl	%eax
pushl	%eax
call	_println_int
addl	$4, %esp

pushl	$1
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

pushl	$4
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

pushl	$8
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

movl	$4, 0(%esp)

movl	0(%esp), %eax
movl	$11, %eax
imull	0(%esp), %eax
pushl	%eax
call	_println_int
addl	$4, %esp

movl	$8, 4(%esp)

movl	$16, 4(%esp)

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

addl	$12, %esp

popl	%ebp
ret


.data
