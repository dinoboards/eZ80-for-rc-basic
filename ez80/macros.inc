	.ASSUME	ADL = 1

.macro		EXTRN  addr
	XDEF	\addr
.endm

CR:		EQU     0DH
LF:		EQU     0AH
ESC:		EQU     1BH
SINGLE_QUOTE	EQU 	27H
BACK_SLASH	EQU	5Ch

; STRING VALUES REFERENCES ARE STORED IN 5 BYTES
; | OFFSET | SIZE | DESCRIPTION |
; |   0    |   1  | Current stored string length        |
; |   1    |   1  | The current allocated storage size  |
; |  2-5   |   3  | The pointer to the actual string    |
STRVAL_SIZE	EQU	5

.macro	ASEG
.endm

.macro PTR	addr
	d24	\addr
.endm

.macro VDU	c
.endm

.macro 	MOS_VDU	val
	LD	A, \val
	CALL	OSWRCH
.endm

.macro log_inspect
	CALL	safe_inspect_all
.endm

.macro			log name
	PUSH	IY

	LD	IY, 3
	ADD	IY, SP

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

	PUSH	IY

	LD	HL, .XX\@
	push	hl

	CALL	_log_info
	pop	hl
	POP	IY	; SP
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

