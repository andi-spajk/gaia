#ifndef EXHAUSTIVE_LINES_H
#define EXHAUSTIVE_LINES_H

/*
;----------------------------------------------
; EXHAUSTIVE EXAMPLES OF TOKEN SEQUENCES:
ADDRESS         =       $ABCD
CONSTANT8       =       $7F
CONSTANT16      =       $1111

DIRECTIVE1      .EQU    $4000
.DEFINE DIRECTIVE2      $8000

                .ORG    $1000
                *=$800
LABEL1:
                INX
                ROL     A
                ORA     $AA
                ASL     CONSTANT8
                LDA     $1234
                CPY     LABEL1
                SBC     $BB,X
                ROR     CONSTANT8,X
                INC     $1000,X
                CMP     CONSTANT16,X
                STX     $CC,Y
                LDX     CONSTANT8,Y
                STA     $DEAD,Y
                AND     ADDRESS,Y
                JMP     ($FACE)
                JMP     (ADDRESS)
                EOR     #$DD
                LDY     #CONSTANT8
; indirect addressing is always zero page
                ADC     ($EE,X)
                LDA     (CONSTANT8,X)
                STA     ($FF),Y
                SBC     (CONSTANT8),Y

                BMI     LABEL1          ; branch
                BVS     L1              ; branch forward ref
                JMP     LABEL1          ; jump
                JSR     L3              ; jump forward ref
LABEL2          BCC     LABEL1          ; label branch
FORREF1         BVC     L19             ; label branch forward ref
LABEL3          JSR     LABEL2          ; label jump
FORREF2         JMP     L21             ; label jump forward ref
                JMP     (WHERE)         ; indirect forward ref
FORREF3         JMP     (WHERE)         ; label indirect forward ref

L1              DEY
L2              ASL     A
L3:             STY     $AA
L4              ROL     CONSTANT8
L5:             LSR     $1234
L6              ROR     ADDRESS
L7:             LDY     $BB,X
L8              DEC     CONSTANT8,X
L9:             INC     $1000,X
L10             CMP     CONSTANT16,X
L11:            STX     $CC,Y
L12             LDX     CONSTANT8,Y
L13:            STA     $B00B,Y
L14             AND     ADDRESS,Y
L15:            JMP     ($CAFE)
L16             JMP     (CONSTANT16)
L17:            CPX     #$DD
L18             LDY     #CONSTANT8
L19:            ADC     ($EE,X)
L20             LDA     (CONSTANT8,X)
L21:            STA     ($FF),Y
L22             SBC     (CONSTANT8),Y
                .END
*/

// no accumulator instructions because they are lexically identical to implied
// addressing modes.
// and only parse_addr_mode() cares about the distinction

const char *constant_addr =     "ADDRESS\t\t=\t$ABCD\n";
const char *constant_8bit =     "CONSTANT8\t=\t$7F\n";
const char *constant_16bit =    "CONSTANT16\t=\t$1111\n";
const char *equ_directive =     "DIRECTIVE1\t.EQU\t$4000\n";
const char *define_directive =  ".DEFINE\tDIRECTIVE2\t$8000\n";
const char *org_directive =            "\t\t.ORG\t$1000\n";
const char *base =                     "\t\t*=$800\n";
const char *lone_label =        "LABEL1:\n";
const char *imp =                      "\t\tINX\n";
const char *acc =                      "\t\tROL\tA\n";
const char *zp =                       "\t\tORA\t$AA\n";
const char *zp_label =                 "\t\tASL\tCONSTANT8\n";
const char *absolute =                 "\t\tLDA\t$1234\n";
const char *abs_label =                "\t\tCPY\tADDRESS\n";
const char *zpx =                      "\t\tSBC\t$BB,X\n";
const char *zpx_label =                "\t\tROR\tCONSTANT8,X\n";
const char *absx =                     "\t\tINC\t$1000,X\n";
const char *absx_label =               "\t\tCMP\tCONSTANT16,X\n";
const char *zpy =                      "\t\tSTX\t$CC,Y\n";
const char *zpy_label =                "\t\tLDX\tCONSTANT8,Y\n";
const char *absy =                     "\t\tSTA\t$DEAD,Y\n";
const char *absy_label =               "\t\tAND\tADDRESS,Y\n";
const char *ind =                      "\t\tJMP\t($FACE)\n";
const char *ind_label =                "\t\tJMP\t(ADDRESS)\n";
const char *imm =                      "\t\tEOR\t#$DD\n";
const char *imm_label =                "\t\tLDY\t#CONSTANT8\n";
const char *indx =                     "\t\tADC\t($EE,X)\n";
const char *indx_label =               "\t\tLDA\t(CONSTANT8,X)\n";
const char *indy =                     "\t\tSTA\t($FF),Y\n";
const char *indy_label =               "\t\tSBC\t(CONSTANT8),Y\n";
const char *branch =                   "\t\tBMI\tLABEL1\n";
const char *branch_forref =            "\t\tBVS\tL1\n";
const char *jump =                     "\t\tJMP\tLABEL1\n";
const char *jump_forref =              "\t\tJSR\tL3\n";
const char *label_branch =       "LABEL2\t\tBCC\tLABEL1\n";
const char *label_branch_forref="FORREF1\t\tBVC\tL19\n";
const char *label_jump =         "LABEL3\t\tJSR\tLABEL2\n";
const char *label_jump_forref = "FORREF2\t\tJMP\tL21\n";
const char *ind_forref =               "\t\tJMP\t(WHERE)\n";
const char *label_ind_forref =  "FORREF3\t\tJMP\t(THERE)\n";
const char *label_imp =              "L1\t\tDEY\n";
const char *label_acc =              "L2\t\tASL\tA\n";
const char *label_zp =              "L3:\t\tSTY\t$AA\n";
const char *label_zp_label =         "L4\t\tROL\tCONSTANT8\n";
const char *label_abs =             "L5:\t\tLSR\t$1234\n";
const char *label_abs_label =        "L6\t\tROR\tADDRESS\n";
const char *label_zpx =             "L7:\t\tLDY\t$BB,X\n";
const char *label_zpx_label =        "L8\t\tDEC\tCONSTANT8,X\n";
const char *label_absx =            "L9:\t\tINC\t$1000,X\n";
const char *label_absx_label =      "L10\t\tCMP\tCONSTANT16,X\n";
const char *label_zpy =            "L11:\t\tSTX\t$CC,Y\n";
const char *label_zpy_label =       "L12\t\tLDX\tCONSTANT8,Y\n";
const char *label_absy =           "L13:\t\tSTA\t$B00B,Y\n";
const char *label_absy_label =      "L14\t\tAND\tADDRESS,Y\n";
const char *label_ind =            "L15:\t\tJMP\t($CAFE)\n";
const char *label_ind_label =       "L16\t\tJMP\t(CONSTANT16)\n";
const char *label_imm =            "L17:\t\tCPX\t#$DD\n";
const char *label_imm_label =       "L18\t\tLDY\t#CONSTANT8\n";
const char *label_indx =           "L19:\t\tADC\t($EE,X)\n";
const char *label_indx_label =      "L20\t\tLDA\t(CONSTANT8,X)\n";
const char *label_indy =           "L21:\t\tSTA\t($FF),Y\n";
const char *label_indy_label =      "L22\t\tSBC\t(CONSTANT8),Y\n";
const char *end_directive =            "\t\t.END\n";

#endif