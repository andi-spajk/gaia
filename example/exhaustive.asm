ADDRESS		=	$ABCD
CONST8		=	$7F	; masswerk assembler only looks at 8 chars in labels
CONST16		=	$1111

; in masswerk, REMOVE THE PERIODS! gaia allows them
DIR1		.EQU	$4000
DIR2		.EQU	$8000
;.DEFINE		DIR2	$8000	; masswerk does not accept this directive

		;.ORG	$1000
		;*=$200		; masswerk does not pad the binary file with zero
				; bytes to reach the stated pc, but gaia does
LABEL1
		INX
		ROL	A
		ORA	$AA
		ASL	CONST8
		LDA	$1234
		CPY	LABEL1
		SBC	$BB,X
		ROR	CONST8,X
		INC	$1000,X
		CMP	CONST16,X
		STX	$CC,Y
		LDX	CONST8,Y
		STA	$DEAD,Y
		AND	ADDRESS,Y
		JMP	($FACE)
		JMP	(ADDRESS)
		EOR	#$DD
		LDY	#CONST8
; indirect addressing is always zero page
		ADC	($EE,X)
		LDA	(CONST8,X)
		STA	($FF),Y
		SBC	(CONST8),Y

		BMI	LABEL1		; branch
		BVS	L1		; branch forward ref
		JMP	LABEL1		; jump
		JSR	L3		; jump forward ref
LABEL2		BCC	LABEL1		; label branch
FORREF1		BVC	L19		; label branch forward ref
LABEL3		JSR	LABEL2		; label jump
FORREF2		JMP	L21		; label jump forward ref
		JMP	(WHERE)		; indirect forward ref
FORREF3		JMP	(WHERE)		; label indirect forward ref

L1		DEY
L2		ASL	A
L3		STY	$AA
L4		ROL	CONST8
L5		LSR	$1234
L6		ROR	ADDRESS
L7		LDY	$BB,X
L8		DEC	CONST8,X
L9		INC	$1000,X
L10		CMP	CONST16,X
L11		STX	$CC,Y
L12		LDX	CONST8,Y
L13		STA	$B00B,Y
L14		AND	ADDRESS,Y
L15		JMP	($CAFE)
L16		JMP	(CONST16)
L17		CPX	#$DD
L18		LDY	#CONST8
L19		ADC	($EE,X)
L20		LDA	(CONST8,X)
L21		STA	($FF),Y
L22		SBC	(CONST8),Y
WHERE
		.END