;====================================================================
;	EDMEM SRC : edit memory starting at SRC - hex input only
;====================================================================

cmd_edmem
				lda ARGCNT					; How many args?
				cmp #$01					; Expect one
				beq cme_arg1				;   Yes, go get 'em
cme_bexit
				jsr pr_err_arwr				;   No, Show err msg
				rts							;   and exit
cme_arg1
				ldx ARG1					; Get ptr to Arg1
				jsr get_SRC_addr			; Attempt to get the addr
				bcs mem_test				;   Yes, go do it
			
				jsr pr_err_inadr			;   No, show err msg
				rts							;   and exit
mem_test
				lda SRC+1					; Test for ram/rom
				cmp #$80					; rom starts at $8000 
				bcc edit_mem				; We're in ram so continue
			
				jsr pr_err_rom				; In rom, show err
				rts	

edit_mem
				jsr pr_CRLF
				jsr pr_SRC_addr				; print $addr: 
				jsr pr_COLON
				jsr pr_SPC

				jsr get_c					; get the first chr
				cmp #ESC					; Is it the esc chr?
				bne em_isCR1				;	No, then go check if it is
em_Exit										;		a CR
				jsr pr_CRLF					;	Yes, then exit
				rts
em_isCR1
				cmp #CR						; is it a CR?
				bne em_isNum1				;	No, then go chk if it is a number
				bra em_Exit
em_isNum1
				jsr put_c
				jsr get_ASC_nybl			; Is it a Hex number?
				bcs em_donyb1				;	Yes, then process it
em_notch
				jsr pr_SPC
				lda #'<'
				jsr put_c
				lda #'-'
				jsr put_c
				jsr pr_err_lgn				;	No then show err msg 
				bra edit_mem				; and try again
em_donyb1
				asl a						; Move it to upper nybble
				asl a
				asl a
				asl a
				sta NUMBASE					; Temporary store

; Get 2nd nybble
				jsr get_c					; get the first chr
				cmp #ESC					; Is it the esc chr?
				bne em_isCR2				;	No, then go check if it is
											;		a CR
				bra em_Exit					;	Yes, then go to Exit
em_isCR2
				cmp #CR						; is it a CR?
				bne em_isNum2				;	No, then go chk if it is a number

				bra em_Exit					;	Yes, then exit
em_isNum2
				jsr put_c
				jsr get_ASC_nybl			; Is it a Hex number?
				bcs em_donyb2				;	Yes, then process it

				bra em_notch				;	No then show err msg and 
											;		try again
em_donyb2
				ora NUMBASE					; add to upper nybble
				sta (SRC)					; and poke into memory

				inc SRC						; inc address
				bne edit_mem
				inc SRC+1
				bra edit_mem