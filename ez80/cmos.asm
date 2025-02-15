	include "macros.inc"
	include "../src/CMOS.Z80"

	GLOBAL safe_inspect_all

	.ASSUME	ADL = 1
	section	.text, "ax", @progbits

INSPECT_ALL:
	PUSH	IY
	CALL	_inspect_all
	POP	IY
	RET

safe_inspect_all:
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

	call	_inspect_all

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
	RET
