          .ORG $E000

;WRTDATA: Fill fifteen memory locations with unique, incremental
;values, where each address uses a unique address line.
WRTDATA   LDX #$0F
          STX $4000     ;$4000 = #$0F  (for test of A14)
          DEX
          STX $2000     ;$2000 = #$0E  (for test of A13)
          DEX
          STX $1000     ;$1000 = #$0D  (for test of A12)
          DEX
          STX $0800     ;$0800 = #$0C  (for test of A11)
          DEX
          STX $0400     ;$0400 = #$0B  (for test of A10)
          DEX
          STX $0200     ;$0200 = #$0A  (for test of A9)
          DEX
          STX $0100     ;$0100 = #$09  (for test of A8)
          DEX
          STX $0080     ;$0080 = #$08  (for test of A7)
          DEX
          STX $0040     ;$0040 = #$07  (for test of A6)
          DEX
          STX $0020     ;$0020 = #$06  (for test of A5)
          DEX
          STX $0010     ;$0010 = #$05  (for test of A4)
          DEX
          STX $0008     ;$0008 = #$04  (for test of A3)
          DEX
          STX $0004     ;$0004 = #$03  (for test of A2)
          DEX
          STX $0002     ;$0002 = #$02  (for test of A1)
          DEX
          STX $0001     ;$0001 = #$01  (for test of A0)

;RAMTEST1  Check address lines A0-A7.
;Routine is broken into RAMTEST1 and RAMTEST2 due to the limited range
;of the relative branch instruction BNE.
RAMTEST1  CPX $0001
          BNE FOUNDBAD  ;A0 bad (not = #$01)?
          INX
          CPX $0002
          BNE FOUNDBAD  ;A1 bad (not = #$02)?
          INX
          CPX $0004
          BNE FOUNDBAD  ;A2 bad (not = #$03)?
          INX
          CPX $0008
          BNE FOUNDBAD  ;A3 bad (not = #$04)?
          INX
          CPX $0010
          BNE FOUNDBAD  ;A4 bad (not = #$05)?
          INX
          CPX $0020
          BNE FOUNDBAD  ;A5 bad (not = #$06)?
          INX
          CPX $0040
          BNE FOUNDBAD  ;A6 bad (not = #$07)?
          INX
          CPX $0080     ;A7 bad (not = #$08)?
          BNE FOUNDBAD
          JMP RAMTEST2

;FOUNDBAD  Called by either RAMTEST1 or RAMTEST2 if
;an error was detected.  This is the jump to the error handler.
FOUNDBAD  JMP FAILURE

;RAMTEST2  Tests address lines A8-A14
RAMTEST2  INX
          CPX $0100
          BNE FOUNDBAD  ;A8 bad (not = #$09)?
          INX
          CPX $0200
          BNE FOUNDBAD  ;A9 bad (not = #$0A)?
          INX
          CPX $0400
          BNE FOUNDBAD  ;A10 bad (not = #$0B)?
          INX
          CPX $0800
          BNE FOUNDBAD  ;A11 bad (not = #$0C)?
          INX
          CPX $1000
          BNE FOUNDBAD  ;A12 bad (not = #$0D)?
          INX
          CPX $2000
          BNE FOUNDBAD  ;A13 bad (not = #$0E)?
          INX
          CPX $4000
          BNE FOUNDBAD  ;A14 bad (not = #$0E)?

;SUCCESS  Program completed with no errors detected.
;
SUCCESS   LDX #$00      ;Optional: Clean up the test bytes by filling
          STX $01       ;with #$00.
          STX $02
          STX $04
          STX $08
          STX $10
          STX $20
          STX $40
          STX $80
          STX $0100
          STX $0200
          STX $0400
          STX $0800
          STX $1000
          STX $2000
          STX $4000
          RTS

FAILURE   DEX           ;An error was detected.
;	   .
;	   .            The address line under test (Axx) at the
;	   .            time of failure is in the X register.