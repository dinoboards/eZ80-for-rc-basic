	section	.text,"ax",@progbits
	.assume	adl = 1
	section	.text._main,"ax",@progbits
	.global	_main
_main:
	ld	hl, _.str
	push	hl
	call	_printf
	pop	hl
	call	_mainxx
	or	a, a
	sbc	hl, hl
	ret
	section	.text._main,"ax",@progbits

	section	.text._log_info,"ax",@progbits
	.global	_log_info
_log_info:
	ld	hl, -3
	call	__frameset
	ld	iy, (ix + 18)
	ld	bc, (ix + 30)
	ld	de, (ix + 33)
	ld	hl, (ix + 36)
	push	hl
	push	de
	push	iy
	ld	hl, (ix + 15)
	push	hl
	ld	hl, (ix + 12)
	push	hl
	ld	hl, (ix + 9)
	push	hl
	push	bc
	ld	hl, (ix + 27)
	push	hl
	ld	hl, (ix + 24)
	push	hl
	ld	hl, (ix + 21)
	push	hl
	ld	hl, (ix + 6)
	push	hl
	ld	hl, _.str.1
	push	hl
	call	_printf
	ld	hl, 36
	add	hl, sp
	ld	sp, hl
	ld	de, 0
	push	de
	pop	hl
	ld	iy, (ix + 33)
	ld	l, (iy)
	ld	e, (iy + 1)
	ld	(ix - 3), de
	push	de
	push	hl
	ld	hl, _.str.2
	push	hl
	call	_printf
	pop	hl
	pop	hl
	pop	hl
	ld	iy, (ix + 36)
	ld	de, (ix - 3)
	ld	e, (iy)
	or	a, a
	sbc	hl, hl
	ld	l, (iy + 1)
	ld	(ix - 3), hl
	push	hl
	push	de
	ld	hl, _.str.3
	push	hl
	call	_printf
	pop	hl
	pop	hl
	pop	hl
	ld	iy, (ix + 30)
	ld	de, (ix - 3)
	ld	e, (iy)
	or	a, a
	sbc	hl, hl
	ld	l, (iy + 1)
	push	hl
	push	de
	ld	hl, _.str.4
	push	hl
	call	_printf
	ld	sp, ix
	pop	ix
	ret
	section	.text._log_info,"ax",@progbits

	section	.rodata._.str,"a",@progbits
	.local	_.str
_.str:
	db	"hello", 0

	section	.rodata._.str.1,"a",@progbits
	.local	_.str.1
_.str.1:
	db	"log: %s.  AF:%X, BC: %X, DE: %X, HL: %X, AF':%X, BC': %X, DE': %X, HL': %X, ix: %X, iy: %X", 13, 10, 0

	section	.rodata._.str.2,"a",@progbits
	.local	_.str.2
_.str.2:
	db	"*iX: %x %x", 13, 10, 0

	section	.rodata._.str.3,"a",@progbits
	.local	_.str.3
_.str.3:
	db	"*iy: %x %x", 13, 10, 0

	section	.rodata._.str.4,"a",@progbits
	.local	_.str.4
_.str.4:
	db	"*hl: %x %x", 13, 10, 0

	;ident	"clang version 15.0.0 (https://github.com/dinoboards/llvm-project.git 85d408fe6bc12f624f4990e20dea30d1c3dd59e3)"
	.extern	__Unwind_SjLj_Register
	.extern	_printf
	.extern	__frameset
	.extern	__Unwind_SjLj_Unregister
	.extern	_mainxx
