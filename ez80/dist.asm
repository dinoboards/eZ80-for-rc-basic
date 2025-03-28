	include "macros.inc"

CPM:		EQU	cpm_bdos_adl	; CP/M extended memory marshaller

	GLOBAL	_main
	GLOBAL	CLRSCN
	GLOBAL	PUTCSR
	GLOBAL	GETCSR
	GLOBAL	PUTIME
	GLOBAL	GETIME
	GLOBAL	GETKEY
	GLOBAL	BYE
	GLOBAL	BDOS

_main:
	JP	COLD

CLRSCN:	JP	CLS		;CLEAR SCREEN
; PUTCSR:	JP	PCSR		;SET CURSOR POSN.
; GETCSR:	JP	GCSR		;READ CURSOR POSN.
; PUTIME:	JP	PTIME		;SET ELAPSED TIME
; GETIME:	JP	GTIME		;READ ELAPSED TIME
; GETKEY:	JP	INKEY		;READ KEY (TIME LIMIT)
BYE:
	ld	hl, 0
	push	hl
	call	_exit		;RETURN TO CP/M
;
;BDOS	- Save the IX and IY registers and before performing a
;	  CP/M function call.
;
BDOS:	PUSH	IX
	PUSH	IY
	CALL	CPM
	POP	IY
	POP	IX
	RET
;
;GTIME	- Read elapsed-time clock.
;  	  Outputs: DEHL = elapsed time (centiseconds)
; 	  Destroys: A,D,E,H,L,F
;
GTIME:	PUSH	BC
	CALL	TICKS
	LD	BC,(OFFLO)
	ADD	HL,BC
	EX	DE,HL
	LD	BC,(OFFHI)
	ADC	HL,BC
	EX	DE,HL
	POP	BC
	RET
;
;PTIME	- Load elapsed-time clock.
;   	  Inputs: DEHL = time to load (centiseconds)
; 	  Destroys: A,D,E,H,L,F
;
PTIME:	PUSH	BC
	PUSH	DE
	PUSH	HL
	CALL	TICKS
	LD	B,H
	LD	C,L
	POP	HL
	OR	A
	SBC	HL,BC
	LD	(OFFLO),HL
	LD	B,D
	LD	C,E
	POP	HL
	SBC	HL,BC
	LD	(OFFHI),HL
	POP	BC
	RET
;
; Get OS elapsed-time clock
;  Outputs: DEHL = time (centiseconds)
; Destroys: A,B,C,D,E,H,L,F
;
TICKS:	LD	C,248		;RunCPM-specific function call
	CALL	BDOS
	PUSH	DE
	EX	DE,HL
	OR	A
	SBC	HL,HL
	LD	BC,-5
	LD	A,32
DIV0:	ADD	HL,BC
	JR	C,DIV1
	SBC	HL,BC
DIV1:	RL	E
	RL	D
	EX	(SP),HL
	RL	L
	RL	H
	EX	(SP),HL
	ADC	HL,HL
	DEC	A
	JR	NZ,DIV0
	EX	DE,HL
	POP	DE
	RET
;
;INKEY	- Sample keyboard with specified wait.
;   	  Inputs: HL = Time to wait (centiseconds)
;  	  Outputs: Carry reset indicates time-out.
;                  If carry set, A = character typed.
; 	  Destroys: A,D,E,H,L,F
;
INKEY:	PUSH	BC
	PUSH	HL
	CALL	TICKS
	POP	DE
	ADD	HL,DE
WAIT:	PUSH	HL
	LD	C,6
	LD	E,0FFH
	CALL	BDOS
	POP	HL
	OR	A
	SCF
	JR	NZ,INKEY1
	PUSH	HL
	CALL	TICKS
	POP	DE
	SBC	HL,DE
	EX	DE,HL
	JR	C,WAIT
INKEY1:	POP	BC
	RET
;
;CLS	- Clear screen.
;	  (Customise to suit your VDU)
; 	  Destroys: A,D,E,H,L,F
;
CLS:	MOS_VDU	12
	XOR     A
	LD      (COUNT),A
	JP      XEQ
;
;PCSR	- Move cursor to specified position.
;   	  Inputs: DE = horizontal position (LHS=0)
;                 HL = vertical position (TOP=0)
; 	  Destroys: A,D,E,H,L,F
;
PCSR:	RET
;
;GCSR	- Return cursor coordinates.
;   	  Outputs:  DE = X coordinate (POS)
;                   HL = Y coordinate (VPOS)
;  	  Destroys: A,D,E,H,L,F
;
GCSR:	LD	DE,0
	LD	HL,0
	RET
;
;COUT - Output a character to the console
;   Inputs: A = character
; Destroys: A,F
;
COUT:	PUSH	BC
	PUSH	DE
	PUSH	HL
	LD	E,A
	LD	C,2
	CALL	BDOS
	POP	HL
	POP	DE
	POP	BC
	RET
;
.if	(0)
	.err	'INSUFFICIENT SPACE'
.endif
;
	ORG	1F0H
;
OFFLO:	DEFW	0		;TIME OFFSET LO
OFFHI:	DEFW	0		;TIME OFFSET HI
	DEFB	80		;WIDTH
	DEFB	'G' AND 1FH	;CURSOR UP
	DEFB	'O' AND 1FH	;CURSOR DOWN
	DEFB	'F' AND 1FH	;START OF LINE
	DEFB	'N' AND 1FH	;END OF LINE
	DEFB	'X' AND 1FH	;DELETE TO END OF LINE
	DEFB	08H		;BACKSPACE & DELETE
	DEFB	'U' AND 1FH	;DEL TO START OF LINE
	DEFB	'J' AND 1FH	;CURSOR LEFT
	DEFB	'L' AND 1FH	;CURSOR RIGHT
	DEFB	'R' AND 1FH	;DELETE CHARACTER
	DEFB	'Q' AND 1FH	;INS/OVR TOGGLE
;
FIN:	END
