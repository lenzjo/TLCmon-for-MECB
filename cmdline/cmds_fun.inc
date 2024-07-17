;=============================================================================
;	Get a command line input
;=============================================================================

get_cmdline
				stz CMD_BUF_PTR				; CMDBUFF chr ptr
gecm_loop
				jsr get_c					; Read a chr from STDIN
				cmp #CR
				beq gecm_exit				; If it's a CR then exit
gecm_bs
				cmp #BS
				bne gecm_maxbuff			; If it's not a BackSpace go on

				lda CMD_BUF_PTR
				beq gecm_loop

				dec CMD_BUF_PTR
				jsr back_spc				; Do the BSread 0
				jmp gecm_loop				;   and go get next chr

gecm_maxbuff
				cpx #MAXCMD
				bne gecm_svch				; If CMDBUFF not full jump

				lda #BELL					; Else
				jsr put_c					;   sound the bell
				jmp gecm_loop				;   and go get next chr

gecm_svch
				ldx CMD_BUF_PTR
				pha							; save ch
				cmp #$61					; Is it lowercase?
				bcc gecm_uc					; No, so go save it

				sbc #$20					; Yes, then convert to uppercase
gecm_uc
				sta CMD_BUFFER,x				; Save the char
				pla							; restore ch
				jsr put_c					;   print it to STDOUT
				inc CMD_BUF_PTR
				jmp gecm_loop				;   and go get next chr

gecm_exit
				jsr pr_CRLF					; print a CRLF
				lda #NULL
				ldx CMD_BUF_PTR
				sta CMD_BUFFER,x			; and store a NULL in the cmd buffer
				cpx #$00					; Set flags on exit
				rts


;====  Destructive BackSpace  ====
back_spc
				jsr pr_BKSPC				; clear the ch from the scr.
				jsr pr_SPC
				jsr pr_BKSPC

				ldx CMD_BUF_PTR
				lda #NULL
				sta CMD_BUFFER,x			; and put a null there
bs_exit
				rts


;=============================================================================
;	Break cmdline up into CMD and optional ARGs
;=============================================================================

parse_cmdline
				stz ARGCNT
				stz ARG1
				stz ARG2
				stz ARG3
				stz ARG4

				stz CMDXPTR

				jsr get_cmd					; Is there a valid CMD present?
				bcc pacm_bexit				;   No, go to err exit

				jsr get_next_arg_ptr		; Is there an ARG1?
				bcc pacm_gexit				;   No, go to clean exit

				stx ARG1					;   Yes, then store index
				inc ARGCNT					;   and increment arg counter

				jsr get_next_arg_ptr		; Is there an ARG2?
				bcc pacm_gexit				;   No, go to clean exit

				stx ARG2					;   Yes, then store index
				inc ARGCNT					;   and increment arg counter

				jsr get_next_arg_ptr		; Is ther an ARG3?
				bcc pacm_gexit				;   No, go to clean exit

				stx ARG3					;   Yes, then store index
				inc ARGCNT					;   and increment arg counter

				jsr get_next_arg_ptr		; Is there an ARG4?
				bcc pacm_gexit				;   No, go to clean exit

				stx ARG4					;   Yes, then store index
				inc ARGCNT					;   and increment arg counter
pacm_gexit
				lda THISCMD					; Clean exit
				sec							; Set Carry flag
				rts							; and exit

pacm_bexit
				lda THISCMD					; Error exit
				clc							; Clear Carry flag
				rts							; and exit


;=============================================================================
;	Is there a cmd present?
;		Success:	sec = Yes, y = ptr
;		Faliure:	clc = No, y = NO_CMD
;=============================================================================

get_cmd
				stz THISCMD					; Iniz cmd ptr
				ldy #$00					; Iniz Cmd table ptr
				ldx CMDXPTR					; Get start of cmd pos.
gc_loop
				lda CMD_BUFFER,x			; Get cmd line chr
				beq gc_test_table			; found a NULL  - exit cmp loop

				cmp #SPC					; Is it a space?
				beq gc_test_table			;   Yes, go exit cmp loop

				cmp VALID_CMDS,y			; is it in curr cmd word?
				bne gc_next_cmd				;   No, go find next cmd word

				inx							;   inc ptrs
				iny
				bra gc_loop					;   and go test next chr
gc_test_table
				lda VALID_CMDS,y			; Is it end of cmd?
				bne gc_next_cmd				;   No, go find next cmd word

				ldy THISCMD
				sec							; Found exit
				rts

gc_next_cmd
				ldx CMDXPTR					; Reset cmd-ptr
				inc THISCMD					; inc to nxt cmd
				lda THISCMD					; Get cmd ptr
				>MULTAx8					;   mult by 8 - each cmd is max 7 chr long + NULL
				tay							;   transfer to y index
				lda VALID_CMDS,y			; Is it end of VALID_CMDS table?
				bne gc_loop					;   No, so go compare next cmd

gc_bad_exit
				ldy #NO_CMD					; Cmd not Found exit
				sty THISCMD
				clc
				rts


;=============================================================================
;	Get a possible ARG ptr from CMDLINE
;		On Exit:  Success:	sec
;				  Failure:	clc
;=============================================================================

get_next_arg_ptr
				lda CMD_BUFFER,x			; Get cmd line chr
				beq gnap_bexit				; It's a NULL - EOL go exit

				jsr find_next_SPC
				bcc gnap_bexit
gnap_loop
				lda CMD_BUFFER,x			; Get cmd line chr
				jsr is_seperator			; Is it a SPC, COMMA or COLON?
				bcc gnap_isalpha			;   No, then go chk if it is AlphaNumeric?

				inx
				bra gnap_loop				;   Yes, then skip to next ch
gnap_isalpha
				jsr is_alphanumeric			; Is it AlphaNumeric (A-Z, a-z, 0-9)?
				bcs gnap_gexit				;   Yes, found 1st ch of ARG, go good exit

				cmp #HASH					; Is it a # ?
				beq gnap_gexit				;   Yes, go good exit

				cmp #DOLLAR					; Is it a $ ?
				beq gnap_gexit				;   Yes, go good exit

				cmp #BINARY					; Is it a % ?
				beq gnap_gexit

				cmp #HELPME					; Is it a ? ?
				beq gnap_gexit				;   Yes, go good exit

gnap_bexit
				clc							; Anything that get's to here is an illegal ch
				rts

gnap_gexit
				sec							; Good exit, ARG found
				rts


find_next_SPC
				lda CMD_BUFFER,x			; Get cmd line chr
				beq fis_bexit				; It's a NULL - EOL go exit

				jsr is_seperator
				bcs fis_gexit

				inx
				bra find_next_SPC
fis_bexit
				clc
				rts

fis_gexit
				sec
				rts


;=============================================================================
;	Get SRC addr
;		sec=Yes, clc=No
;=============================================================================

get_SRC_addr
				jsr get_ASC_number			; Attempt to get a number
				bcs gsa_mvnum				;   Yes, go move it

				rts							;   No, exit

gsa_mvnum
				>CPYW HEX_RES,SRC			; Copy to SRC
				sec
				rts


;=============================================================================
;	Get UNTIL addr
;		sec=Yes, clc=No
;=============================================================================

get_UNTIL_addr
				jsr get_ASC_number			; Attempt to get a number
				bcs gua_mvnum				;   Yes, go move it

				rts							;   No, exit

gua_mvnum
				>CPYW HEX_RES,UNTIL			; Copy to UNTIL
				sec
				rts


;=============================================================================
;	Get DEST addr
;		sec=Yes, clc=No
;=============================================================================

get_DEST_addr
				jsr get_ASC_number			; Attempt to get a number
				bcs gda_mvnum				;   Yes, go move it

				rts							;   No, exit

gda_mvnum
				>CPYW HEX_RES,DEST			; Copy to DEST
				sec
				rts


;=============================================================================
;	Get an ASCII number from the cmdline
;		On Exit:
;			Result in HEX_RES and HEX_RES+1
;			sec = Success, clc = Failure
;=============================================================================

get_ASC_number
				lda CMD_BUFFER,x			; Get a chr
				cmp #HEXADECIMAL			; Is it HEX?
				beq gan_getHEX				;   Yes, go get a Hex num

				cmp #BINARY					; Is it BINARY?
				beq gan_getBINARY			;	Yes, go get a binary number

				jmp get_ASC_dec				;   No, go get a DEC number
gan_getHEX
				jmp get_ASC_hex
gan_getBINARY
				jmp get_ASC_binary


;=============================================================================
;	Convert an ASCII binary number on the cmd line to binary
;		On Exit:
;			Result in HEX_RES and HEX_RES+1
;			sec = Success, clc = Failure
;=============================================================================

get_ASC_binary
				stz HEX_RES					; Clear result regs
				stz HEX_RES+1
				lda #$FF
				sta BIN_CTR
;				dex
gab_next_digit
				inx
				inc BIN_CTR					; count the input bits from 0

				lda CMD_BUFFER,x			; Get a chr
				cmp #SPC					; End of #?
				beq gab_gexit				;   Yes, then exit

				cmp #COMMA					; End of #?
				beq gab_gexit				;   Yes, then exit

				cmp #NULL					; End of cmd?
				beq gab_gexit				;   Yes, then exit

				jsr is_binary				; Is it a 1 or a 0?
				bcc gab_bexit				;   No, then err exit

				ldy BIN_CTR
				cpy #16						; Counted more than 16 bits?
				beq gab_bexit				;	Yes, then err exit

				lsr a						; get bit 0 into Carry

				rol HEX_RES
				rol HEX_RES+1

				jmp gab_next_digit
gab_gexit
				sec							; sec = success
				rts
gab_bexit
				clc							; clc = failure
				rts


;=============================================================================
;	Convert an ASCII decimal number on the cmd line to binary
;		On Exit:
;			Result in HEX_RES and HEX_RES+1
;			sec = Success, clc = Failure
;=============================================================================

get_ASC_dec
				stz HEX_RES					; Clear result regs
				stz HEX_RES+1
				dex
gad_next_digit
				inx
				lda CMD_BUFFER,x			; Get a chr
				cmp #SPC					; End of #?
				beq gad_gexit				;   Yes, then exit

				cmp #COMMA					; End of #?
				beq gad_gexit				;   Yes, then exit

				cmp #NULL					; End of cmd?
				beq gad_gexit				;   Yes, then exit

				jsr is_numeric				; Is it numeric?
				bcc gad_bexit				;   No, then err exit

				and #$0F					; Convert to binary
				jsr multx10					; mult HEX_RES x 10

				clc
				adc HEX_RES					; and add new number to HEX_RES
				sta HEX_RES
				bcc gad_next_digit			; If no carry go get next digit

				inc HEX_RES+1				; There's a carry so add 1 to hi-byte
				jmp gad_next_digit
gad_gexit
				sec							; sec = success
				rts
gad_bexit
				clc							; clc = failure
				rts


;=============================================================================
;                 Convert an ASCII Hex number on the cmd line to binary
;                 On Exit:
;                   Result in HEX_RES and HEX_RES+1
;                   sec = Success, clc = Failure
;=============================================================================

get_ASC_hex
				stz HEX_RES					; clear the result regs
				stz HEX_RES+1
				lda #ISHEX
				sta NUMBASE
gah_loop
				inx							; inc line ptr
				lda CMD_BUFFER,x				; Get a chr
				cmp #SPC					; End of #?
				beq gah_gexit				;   Yes, then exit

				cmp #COMMA					; End of #?
				beq gah_gexit				;   Yes, then exit

				cmp #NULL					; End of cmd?
				beq gah_gexit				;   Yes, then exit

				jsr get_ASC_nybl			; Is it valid # ?
				bcc gah_bexit				;   No, go to err exit

				>MULTW2 HEX_RES				;   Yes, so shift left the result 4 times
				>MULTW2 HEX_RES
				>MULTW2 HEX_RES
				>MULTW2 HEX_RES

				ora HEX_RES					; Add in the new nybble
				sta HEX_RES
				bra gah_loop
gah_gexit
				sec							; sec = success
				rts
gah_bexit
				clc							; clc = failure
				rts


;=============================================================================
;	Get a number in range 0-255
;=============================================================================

get_BYTE
				jsr get_ASC_number    ; Get a number
				bcc gby_bexit         ; If invalid go exit

				lda HEX_RES+1         ; Is hi-byte == 0
				bne gby_bexit         ;   No, go err exit
gby_gexit
				lda HEX_RES
				sta BYTE
				sec
				rts
gby_bexit
				clc
				rts


;=============================================================================
;	Conv ASC to hex nibble.
;		On Exit:
;			clc = ERR, sec = Valid
;			A = number
;=============================================================================

get_ASC_nybl
				sec
				sbc #$30					; Subtract  ASCII '0'
				cmp #$0A					; Is it a number?
				bcc gan_gexit				;   Yes, then go good exit

				sbc #$07					; Subtract diff '9' - 'A'
				cmp #$10					; Is it 'A' - 'F'?
				bcc gan_gexit				;   Yes, then go good exit

				sbc #$20					; Must be lowercase, subtract diff
				cmp #$10					; Is it less than 16?
				bcc gan_gexit				;   Yes, then go good exit
gan_bexit
				clc							;   No, bad ch err exi
				rts
gan_gexit
				sec
				rts


;=============================================================================
;	Multiply HEX_RES by 10
;=============================================================================

multx10
				php
				pha
				asl HEX_RES					; Mult by 2
				rol HEX_RES+1
				lda HEX_RES+1				; Save a copy of HEX_RES
				pha							; multiplied by 2
				lda HEX_RES
				asl HEX_RES					; Now mult by 8
				rol HEX_RES+1				; since HEX_RES has already
				asl HEX_RES					; been mult by 2
				rol HEX_RES+1				; A simple mult by 4 gives
				clc
				adc HEX_RES					; Add in 2x to 8x
				sta HEX_RES					; to get 10x
				pla
				adc HEX_RES+1
				sta HEX_RES+1
				pla
				plp
				rts

