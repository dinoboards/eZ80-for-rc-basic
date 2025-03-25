	include "macros.inc"

CEXPRI:	CALL	COMMA
	JP	EXPRI

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

;TODO: WHAT IS TBY?
TBY	EQU	0FH

;
;MOVE [BY} x,y
;DRAW [BY] x,y
;PLOT [BY] [n,]x,y
;FILL [BY] x,y
;
	GLOBAL	MOVE
MOVE:	LD	C,4
	JR	PLOT1
;
	GLOBAL	DRAW
DRAW:	LD	C,5
	JR	PLOT1
;
	GLOBAL	FILL
FILL:	LD	C,133
	JR	PLOT1
;
	GLOBAL	PLOT
PLOT:	LD	C,69
	CP	TBY
	JR	Z,PLOT1
	CALL	EXPRI
	EXX
	PUSH	HL
	CALL	CEXPRI
	EXX
	LD	A,(IY)
	CP	','
	JR	Z,PLOT3
	POP	DE
	LD	C,69
	JR	PLOT4
;
PLOT1:	CP	TBY
	JR	NZ,PLOT2
	INC	IY
	RES	2,C		;Change absolute to relative
PLOT2:	PUSH	BC
	CALL	EXPRI
	EXX
PLOT3:	PUSH	HL
	CALL	CEXPRI
	EXX
	POP	DE
	POP	BC
PLOT4:	CALL	VDU25
	JP	XEQ

VDU25:	LD	B,25
WRCH6:	LD	A,B
	CALL	OSWRCH
WRCH5:	LD	A,C
	CALL	OSWRCH
WRCH4:	LD	A,E
	CALL	OSWRCH
WRCH3:	LD	A,D
	CALL	OSWRCH
WRCH2:	LD	A,L
	CALL	OSWRCH
	LD	A,H
	JP	OSWRCH
