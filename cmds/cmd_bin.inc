;=============================================================================
;	bin n : Convert a decimal or hex number to binary
;=============================================================================
cmd_bin
				lda ARGCNT					; How many args?
				cmp #$01					; Expect only one
				beq chb_arg					;   Yes, go get 'em

				jsr pr_err_arwr				;   No, Show err msg
				rts							;   and exit
chb_arg
				ldx ARG1					; Get ptr to Arg
				lda CMD_BUFFER,x			; and the ch there
chb_num
				cmp #BINARY					; Is it a binary number?
				beq chb_bexit				;	No, go check hex number

				cmp #HEXADECIMAL			; Is it a hex number
				bne chb_dec					;	No then go get decimal

				jsr get_ASC_hex				; Attempt to get the hexadecimal number
				bcs chb_res					;   Yes, go pr bin

				jsr pr_err_invhex			;	No, then show err msg
				rts
chb_dec
				jsr get_ASC_dec				; Attempt to get the decimal number
				bcs chb_res					;   Yes, go pr bin
chb_bexit
				jsr pr_err_invbin			;   No, show err msg
				rts							;   and exit
chb_res
				jsr hex2bin

				jsr pr_SPC
				jsr pr_EQUALS
				jsr pr_SPC
				jsr pr_BINARY_number
				rts


hex2bin
				ldx #$FF
h2b_hi
				inx
				cpx #8
				beq h2b_lo

				rol HEX_RES+1
				bcs h2b_h1
				lda #'0'
				sta ASCIIDUMP,x
				bra h2b_hi
h2b_h1
				lda #'1'
				sta ASCIIDUMP,x
				bra h2b_hi

h2b_lo
				rol HEX_RES
				bcs h2b_l1
				lda #'0'
				sta ASCIIDUMP,x
				bra h2b_inc
h2b_l1
				lda #'1'
				sta ASCIIDUMP,x
h2b_inc
				inx
				cpx #16
				bne h2b_lo

				lda #0
				sta ASCIIDUMP,x
				rts






