;=============================================================================
;	fill from until byte
;=============================================================================

cmd_fill
				lda ARGCNT					; How many args?
				cmp #$03					; Expect three
				beq cfo_arg1				;   Yes, go get 'em
cfo_bexit
				jsr pr_err_arwr				;   No, Show err msg
				rts							;   and exit
cfo_arg1
				ldx ARG1					; Get ptr to Arg1
				jsr get_SRC_addr			; Attempt to get the addr
				bcs cfo_arg2				;   Yes, go get arg2

				jsr pr_err_inadr			;   No, show err msg
				rts							;   and exit
cfo_arg2
				ldx ARG2					; Get ptr to Arg2
				jsr get_UNTIL_addr			; Attempt to get the addr
				bcs cfo_arg3				;   Yes, go get arg3

				jsr pr_err_inuadr			;   No, show err msg
				rts							;   and exit
cfo_arg3
				ldx ARG3					; Get ptr to Arg3
				jsr get_BYTE				; Attempt to get the byte
				bcs cfo_test				;   Yes, go do it

				jsr pr_err_invbyt			;   No, show err msg
				rts							;   and exit
cfo_test
				lda SRC+1
				cmp /entry					; Is SRC less than start of rom?
				bcc cfo_tstunt				; 	Yes, then go test UNTIL

				jsr pr_err_rom				; 	No, show err and exit
				rts
cfo_tstunt
				lda UNTIL+1
				cmp /entry					; Is UNTIL less than start of rom?
				bcc cfo_start				;	Yes, Then go do it

				jsr pr_err_rom				; 	No, show err and exit
				rts
cfo_start
				sec							; how many bytes to fill?
				lda UNTIL					; len = until-src
				sbc SRC
				sta LEN
				lda UNTIL+1
				sbc SRC+1
				sta LEN+1
				>INCW LEN
cfo_loop
				lda LEN+1					; Filled all bytes yet?
				bne cfo_fillit				;   No, then go fill byte

				lda LEN
				beq cfo_exit				;   Yes, then go exit
cfo_fillit
				lda BYTE					; get byte
				sta (SRC)

				>INCW SRC					; update the ptrs
				>DECW LEN

				bra cfo_loop				; Go copy the next byte
cfo_exit
				rts