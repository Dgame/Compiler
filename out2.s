.text
.globl _prog
_prog:

pushl	%ebp
movl	%esp, %ebp

subl	$8, %esp

movl	$4, %eax
pushl	%eax
call	_println_int
addl	$4, %esp

movl	$8, %eax
pushl	%eax
call	_println_int
addl	$4, %esp

movl	$-42, %eax
pushl	%eax
call	_println_int
addl	$4, %esp

movl	$-42, %eax
pushl	%eax
call	_println_int
addl	$4, %esp

movl	$-42, %eax
pushl	%eax
call	_println_int
addl	$4, %esp

movl	$-42, %eax
pushl	%eax
call	_println_int
addl	$4, %esp

movl	$-42, %eax
pushl	%eax
call	_println_int
addl	$4, %esp

movl	$-42, %eax
pushl	%eax
call	_println_int
addl	$4, %esp

movl	$-42, %eax
pushl	%eax
call	_println_int
addl	$4, %esp

movl	$-42, %eax
pushl	%eax
call	_println_int
addl	$4, %esp

movl	$-42, %eax
pushl	%eax
call	_println_int
addl	$4, %esp

movl	$-42, %eax
pushl	%eax
call	_println_int
addl	$4, %esp

movl	$4, 0(%esp)

movl	$-42, %eax
pushl	%eax
call	_println_int
addl	$4, %esp

movl	$8, 4(%esp)

addl	$8, %esp

popl	%ebp
ret


.data
