;=============================================================================
;	hex n : Convert a decimal or binary number to hexadecimal
;=============================================================================

cmd_hex
				lda ARGCNT					; How many args?
				cmp #$01					; Expect only one
				beq che_arg					;   Yes, go get 'em

				jsr pr_err_arwr				;   No, Show err msg
				rts							;   and exit
che_arg
				ldx ARG1					; Get ptr to Arg
				lda CMD_BUFFER,x			; and the ch there
che_num
				cmp #BINARY					; Is it a binary number?
				bne che_hex					;	No, go check hex number

				jsr get_ASC_binary			;	Yes, then get it
				bcs che_res					; Valid bin, then go print it

				bra che_bexit				; Invalid, then go show err msg
che_hex
				cmp #HEXADECIMAL			; Is it a hex number
				bne che_dec					;	No then go get decimal

				jsr pr_err_invhex			;	Yes, then show err msg
				rts
che_dec
				jsr get_ASC_dec				; Attempt to get the decimal number
				bcs che_res					;   Yes, go do cmd
che_bexit
				jsr pr_err_invnum			;   No, show err msg
				rts							;   and exit
che_res
				jsr pr_SPC
				jsr pr_EQUALS
				jsr pr_SPC
				jsr pr_HEX_number
				rts



