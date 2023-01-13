STREQU:		LDY #$00	;Compare strings, case-sensitive
		LDA ($FA),Y	;Naturally, the zero flag is used to return if the strings are equal
		CMP ($FC),Y
		BEQ STREQU1
		RTS
STREQU1:	TAY
STREQULP:	LDA ($FC),Y
		AND #$7F
		STA $FF
		LDA ($FA),Y
		AND #$7F
		CMP $FF
		BNE STREQUEX
		DEY
		BNE STREQULP
STREQUEX:	RTS

;Comparing Strings (Case-Insensitive):

STREQI:		LDY #$00	;Compare strings, case-insensitive
		LDA ($FA),Y	;Naturally, the zero flag is used to return if the strings are equal
		CMP ($FC),Y
		BEQ STREQI1
		RTS
STREQI1:	TAY
STREQILP:	LDA ($FC),Y
		AND #$7F
		CMP #$7B
		BCS STREQI2
		CMP #$61
		BCC STREQI2
		SBC #$20
STREQI2:	STA $FF
		LDA ($FA),Y
		AND #$7F
		CMP #$7B
		BCS STREQI3
		CMP #$61
		BCC STREQI3
		SBC #$20
STREQI3:	CMP $FF
		BNE STREQIEX
		DEY
		BNE STREQILP
STREQIEX:	RTS
