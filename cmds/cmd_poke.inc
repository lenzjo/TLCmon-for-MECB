;*******************************************************************************
;                 Poke Cmd: poke [h]|addr,byte : Write byte to location addr
;*******************************************************************************

cmd_poke
				lda ARGCNT					; How many args?
				cmp #$02					; Expect two
				beq cpo_arg1				;   Yes, go get 'em
cpo_bexit
				jsr pr_err_arwr				;   No, Show err msg
				rts							;   and exit
cpo_arg1
				ldx ARG1					; Get ptr to Arg1
				jsr get_SRC_addr			; Attempt to get the addr
				bcc cpo_bexit2				;   No, go err exit

				lda SRC+1					;   Yes, so test for ROM addr ($C100-$FFFF)
				cmp /entry
				bcc cpo_arg2				;   In RAM, so go poke it ;)

				jsr pr_err_rom
				rts
cpo_bexit2
				jsr pr_err_inadr			;   No, show err msg
				rts							;   and exit
cpo_arg2
				ldx ARG2					; Get ptr to Arg2
				jsr get_BYTE				; Attempt to get a byte
				bcs cpo_poke				;   Yes, go do poke

				jsr pr_err_lgn				;   No, print err msg
				rts							;   and exit
cpo_poke
				lda BYTE					; Finally, do the poke!
				sta (SRC)
				rts