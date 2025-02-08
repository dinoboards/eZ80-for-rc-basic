
	global	_LOMEM
	global	_HIMEM
	global	_ACCS

_LOMEM	=	LOMEM
_HIMEM = HIMEM
_ACCS = ACCS


	include "macros.inc"

	section	.bss_on_chip, "aw", @progbits

	BALIGN 		256

	include "../src/DATA.Z80"

	global	conversion_store

conversion_store:	ds 		4		; Scratch pad to convert integer representations
