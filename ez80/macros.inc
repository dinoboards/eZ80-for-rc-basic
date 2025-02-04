	.ASSUME	ADL = 1

.macro		EXTRN  addr
	XDEF	\addr
.endm

SINGLE_QUOTE	EQU 	27H
BACK_SLASH	EQU	5Ch

.macro	ASEG
.endm


.macro			log name
	PUSH	IY
	PUSH	IX
	PUSH	HL
	PUSH	DE
	PUSH	BC
	PUSH	AF
	EXX
	PUSH	HL
	PUSH	DE
	PUSH	BC
	EX	AF, AF'
	PUSH	AF
	LD	HL, .XX\@
	push	hl
	CALL	_log_info
	pop	hl
	POP	AF
	EX	AF, AF'
	POP	BC
	POP	DE
	POP	HL
	EXX
	POP	AF
	POP	BC
	POP	DE
	POP	HL
	POP	IX
	POP	IY

	section	.data, "aw", @progbits
.XX\@:
	DB	"\name", 0

	section	.text, "ax", @progbits

.endm

	.ASSUME	ADL = 1
	section	.text, "ax", @progbits

