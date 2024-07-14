;=============================================================================
;	System Messages
;=============================================================================

MonHeader
				.DB		ESC,'[2J'
				.AZ		'TLCmon '
MonVersion
				.DB		'v', $30+VerMaj,'.',$30+VerMin,'.',$30+VerMnt, NULL

MonPrompt		.AZ		'> '


;=============================================================================
;	Error Messages
;=============================================================================

ERR_LEADER		.AZ		'Err: '

ERR_GARBAGE		.AZ		'Huh?? Do not understand.. Try again..'
ERR_WAR			.AZ		'Wrong number of Args.'
ERR_INSADR		.AZ		'Invalid Start address.'
ERR_ROM			.AZ		'Cannot change EEPROM.'
ERR_LGN			.AZ		'Byte out of range (0-255).'
ERR_INUADR		.AZ		'Invalid Until address.'
ERR_INDADR		.AZ		'Invalid Destination address.'
ERR_EQADR		.AZ		'Destination same as Source.'
ERR_INVBYT		.AZ		'Missing or invalid byte.'
ERR_INVNUM      .AZ     'Missing or invalid number'
ERR_INVHEX		.AZ		'Enter only a Binary or Decimal number'
ERR_INVDEC		.AZ		'Enter only a Binary or Hex number'
ERR_INVBIN		.AZ		'Enter only a Decimal or Hex number'


;=============================================================================
;	Print System Messages
;=============================================================================
                  
pr_MonHeader								; Print the Startup Banner
				>CPYMSPTR MonHeader
				jsr put_str
				jsr pr_MonVersion
				jsr pr_CRLF
				rts


pr_MonVersion
				>CPYMSPTR MonVersion
				jsr put_str
				rts


pr_MonPrompt								; Print the system prompt
				jsr pr_CRLF
				>CPYMSPTR MonPrompt
				jsr put_str
				rts


;=============================================================================
;	Print Functions
;=============================================================================

put_str
				sta MSG_PTR
				sty MSG_PTR+1
pr_str
				ldy #$00
pes_loop
				lda (MSG_PTR),y				; Get the next char in the string
				beq pes_exit				;   NULL means the end

				jsr put_c					;   Otherwise put the char
				iny							; Increment MSGPTR-lo
				bne pes_loop				;   No roll-over? Loop back for next character

				inc MSG_PTR+1				; MSGPTR-lo rolled over--carry hi byte
				jmp pes_loop				; Now loop back
pes_exit
				rts


pr_err_leader
				>CPYMSPTR ERR_LEADER
				jsr put_str
				rts


pr_err_msg
				pha
				phy
				jsr pr_err_leader			; Print 'Err: '
				ply
				pla
				jsr put_str					; then the actual error msg
				jsr pr_CRLF
				rts


pr_SRC_addr
				jsr pr_DOLLAR				; Format is $xxxx
				lda SRC+1
				jsr pr_HEX_byte
				lda SRC
				jsr pr_HEX_byte
				rts


pr_UNTIL_addr
				jsr pr_DOLLAR				; Format is $xxxx
				lda UNTIL+1
				jsr pr_HEX_byte
				lda UNTIL
				jsr pr_HEX_byte
				rts


pr_DEST_addr
				jsr pr_DOLLAR				; Format is $xxxx
				lda DEST+1
				jsr pr_HEX_byte
				lda DEST
				jsr pr_HEX_byte
				rts


pr_HEX_number
				jsr pr_DOLLAR
				lda HEX_RES+1
				jsr pr_HEX_byte
				lda HEX_RES
				jsr pr_HEX_byte
				rts


pr_BIN_number
				jsr pr_BINARY
				>CPYMSPTR ASCIIDUMP
				jsr put_str
				rts


pr_BINARY_number
				ldx #$FF
pbn_skip0
				inx
				lda ASCIIDUMP,x
				cmp #'0'
				beq pbn_skip0
pbn_prlp
				cmp #NULL
				beq pbn_exit

				jsr put_c
				inx
				lda ASCIIDUMP,x
				bra pbn_prlp
pbn_exit
				rts
			

;=============================================================================
;	Print various characters
;=============================================================================

pr_CRLF
				lda #CR
				jsr put_c
				lda #LF
				jmp put_c


pr_SPC
				lda #SPC
				jmp put_c


;====  Print X Spaces  ====

pr_XSPC
				lda #SPC
				jsr put_c
				dex
				bne pr_XSPC

				rts


pr_BKSPC
				lda #BS
				jmp put_c


pr_DOLLAR
				lda #DOLLAR
				jmp put_c


pr_EQUALS
				lda #EQUALS
				jmp put_c


pr_COLON
				lda #COLON
				jmp put_c


pr_BINARY
				lda #BINARY
				jmp put_c


;=============================================================================
;	Print Error Messages
;=============================================================================

pr_err_nocmd								; Print Unkown Device Name Err msg
				>CPYMSPTR ERR_GARBAGE
				jsr pr_err_msg
				rts


pr_err_arwr									; Print Arg number Err msg
				>CPYMSPTR ERR_WAR
				jsr pr_err_msg
				rts


pr_err_inadr								; Print Invalid Start Addr Err msg
				>CPYMSPTR ERR_INSADR
				jsr pr_err_msg
				rts


pr_err_rom									; Print Byte out of range Err msg
				>CPYMSPTR ERR_ROM
				jsr pr_err_msg
				rts

               
pr_err_lgn									; Print Byte out of range Err msg
				>CPYMSPTR ERR_LGN
				jsr pr_err_msg
				rts


pr_err_inuadr								; Print Invalid Until Addr Err msg
				>CPYMSPTR ERR_INUADR
				jsr pr_err_msg
				rts


pr_err_indadr								; Print Invalid Dest Addr Err msg
				>CPYMSPTR ERR_INDADR
				jsr pr_err_msg
				rts


pr_err_eqadr								; Print src = dest
				>CPYMSPTR ERR_EQADR
				jsr pr_err_msg
				rts


pr_err_invbyt								; Print Invalid or missing byte
				>CPYMSPTR ERR_INVBYT
				jsr pr_err_msg
				rts


pr_err_invnum								; Print invalid or missing number
				>CPYMSPTR ERR_INVNUM
				jsr pr_err_msg
				rts


pr_err_invhex								; Print bin or dec numbers only
				>CPYMSPTR ERR_INVHEX
				jsr pr_err_msg
				rts


pr_err_invdec								; Print hex or bin numbers only
				>CPYMSPTR ERR_INVDEC
				jsr pr_err_msg
				rts


pr_err_invbin								; Print hex or dec numbers only
				>CPYMSPTR ERR_INVBIN
				jsr pr_err_msg
				rts



;=============================================================================
;	Print numbers from CMD_IBUF
;=============================================================================

;=-=-=-=  Print byte in A from ASCBYTE =-=-=-=

pr_HEX_byte
				phy
				jsr byt_2_ASC				; Convert byte to ASCII
				lda ASCBYTE					; get Hi-nyb
				jsr put_c					; and print it
				lda ASCBYTE+1				; Get Lo-nyb
				jsr put_c					; and print it
				ply
				rts


;=-=-=-=  Convert a byte to an ASCII char in ASCBYTE & ASCBYTE+1  =-=-=-=

byt_2_ASC
				pha							; Save byte
				lsr a
				lsr a
				lsr a
				lsr a						; move to lower nybble
				tay
				lda HEXNUM,y
				sta ASCBYTE					; and store it
				pla							; get the byte back
				and #$0F					; get lower nybble
				tay
				lda HEXNUM,y
				sta ASCBYTE+1				; and store it
				rts


HEXNUM			.DB		'0123456789ABCDEF'

