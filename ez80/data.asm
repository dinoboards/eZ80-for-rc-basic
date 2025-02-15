
	global	_LOMEM
	global	_HIMEM
	global	_ACCS
	global	_PAGE
	global	_DYNVAR
	global _FNPTR
	global _PROPTR
	global _DATPTR
	global _FREE
	GLOBAL _LOCCHK

_LOMEM	=	LOMEM
_HIMEM = HIMEM
_ACCS = ACCS
_PAGE = PAGE
_DYNVAR = DYNVAR
_FNPTR = FNPTR
_PROPTR = PROPTR
_DATPTR = DATPTR
_FREE = FREE

	include "macros.inc"

	section	.bss_on_chip, "aw", @progbits

	BALIGN 		256

	include "../src/DATA.Z80"

	global	conversion_store

conversion_store:	ds 		4		; Scratch pad to convert integer representations
