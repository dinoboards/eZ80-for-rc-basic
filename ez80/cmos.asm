	include "macros.inc"
	include "../src/CMOS.Z80"

	; GLOBAL INSPECT_ALL

	.ASSUME	ADL = 1
	section	.text, "ax", @progbits

INSPECT_ALL:
	PUSH	IY
	CALL	_inspect_all
	POP	IY
	RET
