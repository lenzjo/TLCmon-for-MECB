;=============================================================================
;	Dump Cmd: dump [h]|addr[,dest]
;		Show hex dump from addr for default 128 bytes or to dest.
;
;	### Note: Uses CMD_IBUF as temp storage
;=============================================================================

cmd_dump
				lda ARGCNT					; How many args?
cdu_getargs
				ldx ARG1					; Get ptr to Arg1
				jsr get_SRC_addr			; Attempt to get the addr
				bcs cdu_getcnt				;   Yes, go get count

				jsr pr_err_inadr			;   No, show err msg
				rts							;   and exit
cdu_getcnt
				lda ARGCNT					; How many args?
				cmp #$01
				beq cdu_dfcnt				;   Only one, so do default count
											;   Else get end addr from
				ldx ARG2					;   the 2nd Arg
				jsr get_UNTIL_addr			; Attempt to get the UNTIL addr
				bcs dump_blk				;   Yes, go do cmd

				jsr pr_err_inuadr			;   No, show err msg
				rts							;   and exit
cdu_dfcnt
				clc							; UNTIL = SRC + $0100
				lda SRC						; This is the default
				adc #$00
				sta UNTIL
				lda SRC+1
				adc #$01
				sta UNTIL+1


;=============================================================================
; Alternate Entry Point: If SRC and UNTIL contain valid address's
;=============================================================================

dump_blk
				>CPYMSPTR dump_hdr			; Print dump header
				jsr put_str
				jsr pr_CRLF
				>CPYMSPTR dump_hdr2			; Print dump header
				jsr put_str
				jsr pr_CRLF
				
				lda #$10					; 80 col width = 16 bytes to show
				sta BYTECNT
cdu_do_row
				ldy #$00					; Bytes per line ctr
				jsr pr_SPC
				jsr pr_SRC_addr				; Print the addr '$xxxx'

				jsr pr_SPC					; Put a SPC either side of the colon
				jsr pr_COLON
				jsr pr_SPC
				bra cdu_pr_byte
cdu_row_cont
				jsr pr_COLON
cdu_pr_byte
				lda (SRC)					; Get byte from memory
				jsr pr_HEX_byte				; and print it
				jsr pr_SPC					; followed by a space

				lda (SRC)					; Now save the ASCII
				cmp #SPC					; If it's >= than a SPC
				bcs cdu_ch_test				;   Yes, then don't change it

				lda #'.'					;   No, substitute a '.' for unprintable chars
				bra cdu_ASC_save
cdu_ch_test
				cmp #$7F
				bcc cdu_ASC_save			; it's printable

				lda #'.'					; Substitute a '.' for unprintable chars
cdu_ASC_save
				sta ASCIIDUMP,y				; Finally save it in buffer
				>INCW SRC					; Inc the mem-ptr
				>CMPAD SRC,UNTIL			; Have we finished yet?
				bne cdu_EOL					;   No, continue

				bra cdu_finish				;   Yes, so go exit
cdu_EOL
				iny							; Inc line byte ctr
				cpy BYTECNT					; End of row yet?
				bne cdu_pr_byte				;   No, go print another byte

				jsr pr_ASC_dump				;   Yes, Print the ASCIIDUMP
				jmp cdu_do_row				;   and go do the next row
cdu_finish
				cpy BYTECNT					; Finish of the last row
				beq cdu_last_ASC_dump		; At the end of line? Go print ASC dump
cdu_fill_gap
				iny
				cpy BYTECNT
				beq cdu_last_ASC_dump

				ldx #$03
				jsr pr_XSPC					; Print 3 SPCs for each 'missing' byte on the row
				lda #SPC					; put a SPC in the ASCIIDUMP string
				sta ASCIIDUMP,y
				bra cdu_fill_gap
cdu_last_ASC_dump
				jsr pr_ASC_dump
				rts


;                 Print ASCII version of the last 16 bytes

pr_ASC_dump
				lda #NULL					; NULL terminate the string
				sta ASCIIDUMP,y
				jsr pr_COLON				; Put a separator after the bytes
pad_cont
				jsr pr_SPC
				>CPYMSPTR ASCIIDUMP			; Now print it
				jsr put_str
				jsr pr_CRLF
				rts

dump_hdr
				.AZ '  ADDR | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F | 0123456789ABCDEF'
dump_hdr2
				.AZ '---------------------------------------------------------------------------'