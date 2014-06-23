.text
.globl _prog

_prog:
push	%ebp
movl	%esp, %ebp

subl	$40, %esp

# -- 1 + 2 * 3 --

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

# -- 12 - 6

movl	$12, %eax
pushl	%eax
movl	$6, %eax
subl	%eax, 0(%esp)
addl	$4, %esp

pushl	%eax
call	_println_int
addl	$4, %esp

# -- 6 - 12



# -- (1 + 2) * 3

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

# -- a = 4 ; b = 8

movl	$4, 0(%esp)
movl	$8, 4(%esp)

# -- a * b --

movl	0(%esp), %eax # a
imull	4(%esp), %eax
movl	%eax, 8(%esp) # c

# -- b * a --

movl	4(%esp), %eax # b
imul	0(%esp), %eax
movl	%eax, 12(%esp) # d

# -- a + b --

movl	0(%esp), %eax # a
addl	4(%esp), %eax
movl	%eax, 16(%esp) # e

# -- a - b -- 

movl	0(%esp), %eax # a
subl	4(%esp), %eax
movl	%eax, 20(%esp) # f

# -- b - a --

movl	4(%esp), %eax # b
subl	0(%esp), %eax
movl	%eax, 24(%esp) # f

# -- a * 2 + b

movl	$2, %eax
imull	0(%esp), %eax # a * 2
addl	4(%esp), %eax
movl	%eax, 28(%esp)

# -- b / a --

movl	4(%esp), %eax # b
movl	0(%esp), %ebx # a
movl	$0, %edx
idiv	%ebx
movl	%eax, 32(%esp)

# -- 19 % 5 --

movl	$19, %eax
movl	$5, %ebx
movl	$0, %edx
idiv	%ebx
movl	%edx, 36(%esp)

# ====

push	0(%esp)
call	_println_int
addl	$4, %esp

push	4(%esp)
call	_println_int
addl	$4, %esp

push	8(%esp)
call	_println_int
addl	$4, %esp

push	12(%esp)
call	_println_int
addl	$4, %esp

push	16(%esp)
call	_println_int
addl	$4, %esp

push	20(%esp)
call	_println_int
addl	$4, %esp

push	24(%esp)
call	_println_int
addl	$4, %esp

push	28(%esp)
call	_println_int
addl	$4, %esp

push	32(%esp)
call	_println_int
addl	$4, %esp

push	36(%esp)
call	_println_int
addl	$4, %esp

addl	$40, %esp

popl 	%ebp
ret

.data
