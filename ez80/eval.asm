	include "macros.inc"

	GLOBAL	EXPR_24BIT_INT

EXPR_24BIT_INT:
	CALL	EXPRI		; RESULT IN HLH'L' C SHOULD BE ZERO
	; U(HL) = l
	; H = H'
	; L = L'

	EXX
	LD	(conversion_store), HL
	EXX
	LD	A, L
	LD	(conversion_store + 2), A
	LD	HL, (conversion_store)
	RET

	include "../src/EVAL.Z80"
