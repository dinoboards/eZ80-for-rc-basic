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

; COLOUR colour
; COLOUR L,P
; COLOUR L,R,G,B
;
VDU_BUFFER:		EQU	ACCS		; Storage for VDU commands
	GLOBAL	COLOUR
COLOUR:	CALL	EXPRI			; The colour / mode
	EXX
	LD	A, L
	LD	(VDU_BUFFER+0), A	; Store first parameter
	CALL	NXT			; Are there any more parameters?
	CP	','
	JR	Z, COLOR1		; Yes, so we're doing a palette change next
;
	MOS_VDU	17			; Just set the colour
	MOS_VDU	(VDU_BUFFER+0)
	JP	XEQ
;
COLOR1: CALL	COMMA
	CALL	EXPRI			; Parse R (OR P)
	EXX
	LD	A, L
	LD	(VDU_BUFFER+1), A
	CALL	NXT			; Are there any more parameters?
	CP	','
	JR	Z, COLOR2		; Yes, so we're doing COLOUR L,R,G,B
;
	MOS_VDU	13h			; MOS_VDU:COLOUR
	MOS_VDU	(VDU_BUFFER+0)		; Logical Colour
	MOS_VDU	(VDU_BUFFER+1)		; Palette Colour
	MOS_VDU	0			; RGB set to 0
	MOS_VDU	0
	MOS_VDU	0
	JP	XEQ
;
COLOR2: CALL	COMMA
	CALL	EXPRI			; Parse G
	EXX
	LD	A, L
	LD	(VDU_BUFFER+2), A
	CALL	COMMA
	CALL	EXPRI			; Parse B
	EXX
	LD	A, L
	LD	(VDU_BUFFER+3), A
	MOS_VDU	13h			; MOS_VDU:COLOUR
	MOS_VDU	(VDU_BUFFER+0)		; Logical Colour
	MOS_VDU	0FFh			; Physical Colour (-1 for RGB mode)
	MOS_VDU	(VDU_BUFFER+1)		; R
	MOS_VDU	(VDU_BUFFER+2)		; G
	MOS_VDU	(VDU_BUFFER+3)		; B
	JP	XEQ
