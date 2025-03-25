	include "macros.inc"

	GLOBAL	MODE
MODE:	CALL    EXPRI
	EXX
	MOS_VDU	22			; Mode change
	MOS_VDU	L
	JP	XEQ

	global _vdu_not_implemented

_vdu_not_implemented:
	XOR	A
	CALL	EXTERR
	DB	"VDU Function not implemented", 0
