;=============================================================================
;	dec n : Convert a hex or binary number to decimal
;=============================================================================
cmd_dec
				lda ARGCNT					; How many args?
				cmp #$01					; Expect only one
				beq cde_arg					;   Yes, go get 'em

				jsr pr_err_arwr				;   No, Show err msg
				rts							;   and exit
cde_arg
				ldx ARG1					; Get ptr to Arg
				lda CMD_BUFFER,x			; and the ch there
				cmp #HEXADECIMAL			; Is it HEX?
				bne cde_arg2				;   No, err msg and exit

				jsr get_ASC_hex				; Attempt to get the hexadecimal number
				bcs cde_dec					;   Yes, go do cmd
cde_arg2
				cmp #BINARY					; Is it binary?
				bne cde_bexit				;	No, show err msg

				jsr get_ASC_binary			; Attempt to get the binary number
				bcs cde_dec
cde_bexit
				jsr pr_err_invdec			; Show err msg
				rts							;   and exit
cde_dec
				jsr Bin2Dec					; convert it to decimal

				jsr pr_SPC					; Now print the hex number
				jsr pr_EQUALS
				jsr pr_SPC

				ldx #6
cde_skip0
				dex
				lda DEC_BASE,x
				beq cde_skip0				; skip over leading zeros
cde_ploop
				lda DEC_BASE,x				; and then the decimal number
				clc
				adc #$30					; make it printable ascii
				jsr put_c
				dex
				bne cde_ploop

				jsr pr_CRLF
				rts


Bin2Dec
				sed
				stz DEC_RES+0
				stz DEC_RES+1
				stz DEC_RES+2
				stz DEC_RES+3
				stz DEC_RES+4
				ldx #$10
BitLoop
				asl HEX_RES+0
				rol HEX_RES+1
				ldy DEC_RES+0
				lda BinTable,y
				rol a
				sta DEC_RES+0
				ldy DEC_RES+1
				lda BinTable,y
				rol a
				sta DEC_RES+1
				ldy DEC_RES+2
				lda BinTable,y
				rol a
				sta DEC_RES+2
				ldy DEC_RES+3
				lda BinTable,y
				rol a
				sta DEC_RES+3
				rol DEC_RES+4
				dex
				bne BitLoop

				cld

				rts

BinTable
   				.DB $00, $01, $02, $03, $04, $80, $81, $82, $83, $84