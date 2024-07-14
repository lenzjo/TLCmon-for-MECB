;=============================================================================
;	Help menu functions and messages
;=============================================================================

HLP_HDR			.AZ 'Available Commands:'

HLP_PEEK		.AZ 'peek addr              - Returns byte at addr.'
HLP_POKE		.AZ 'poke addr,byte         - Store byte at addr.'
HLP_DUMP		.AZ 'dump addr,dest         - Show hex-dump from addr, for 128bytes or to dest.'

HLP_EDMEM		.AZ 'edmem addr             - Edit memory starting from addr.'
HLP_HELP		.AZ 'help                   - Show this help page.'
HLP_COPY		.AZ 'copy addr,until dest   - Copy blk from-until to dest.'

HLP_FILL		.AZ 'fill addr,until byte   - Fill blk from-until with byte.'
HLP_HEX			.AZ 'hex %|n                - Convert num from one base to hexadecimal.'
HLP_GO			.AZ 'go addr                - Run code starting at addr.'

HLP_DISM		.AZ 'dism addr,until        - Disassemble mem blk addr-until.'
HLP_UPTIME		.AZ 'uptime                 - Show the elapsed time since power on.'
HLP_DEC			.AZ 'dec $|%                - Convert num from one base to decimal.'

HLP_BIN			.AZ 'bin $|n                - Convert num from one base to binary.'


HLP_CMDH		.AZ 'cmd ?                  - Shows one line help for cmd.'


cmd_help
				>CPYMSPTR HLP_HDR
				jsr put_str
				jsr pr_CRLF
				jsr pr_CRLF

				>CPYMSPTR HLP_PEEK
				jsr put_str
				jsr pr_CRLF
				>CPYMSPTR HLP_POKE
				jsr put_str
				jsr pr_CRLF
				>CPYMSPTR HLP_DUMP
				jsr put_str
				jsr pr_CRLF

				>CPYMSPTR HLP_EDMEM
				jsr put_str
				jsr pr_CRLF
				>CPYMSPTR HLP_HELP
				jsr put_str
				jsr pr_CRLF
				>CPYMSPTR HLP_COPY
				jsr put_str
				jsr pr_CRLF

				>CPYMSPTR HLP_FILL
				jsr put_str
				jsr pr_CRLF

				>CPYMSPTR HLP_HEX
				jsr put_str
				jsr pr_CRLF

				>CPYMSPTR HLP_GO
				jsr put_str
				jsr pr_CRLF
;
;				>CPYMSPTR HLP_DISM
;				jsr put_str
;				jsr pr_CRLF

;				>CPYMSPTR HLP_UPTIME
;				jsr put_str
;				jsr pr_CRLF

				>CPYMSPTR HLP_DEC
				jsr put_str
				jsr pr_CRLF

				>CPYMSPTR HLP_BIN
				jsr put_str
				jsr pr_CRLF

				jsr pr_CRLF
				>CPYMSPTR HLP_CMDH
				jsr put_str
				jsr pr_CRLF
				rts
				

;=============================================================================
;	Print a cmd's help line if needed
;		On Entry: THISCMD holds cmd's index#
;		On Exit: sec = done, clc = not done
;=============================================================================

pr_cmd_help
				ldx ARG1
				lda CMD_BUFFER,x				; Get Arg1's 1st ch
				cmp #HELPME					; Is the help?
				bne prcm_bexit				;   No, then go exit

				lda THISCMD					; Get cmd index
				clc
				asl a						; double
				tax
				lda HELP_MSG_TABLE,x
				sta MSG_PTR
				inx
				lda HELP_MSG_TABLE,x
				sta MSG_PTR+1
				jsr pr_str
				jsr pr_CRLF
				sec
				rts
prcm_bexit
				clc
				rts



;=============================================================================
;	Execute a command from the jumptable
;=============================================================================

exec_cmd
				lda THISCMD					; Get the cmd index
				clc
				asl a						; and double it
				tax
				jmp (CMD_JUMP_TABLE,x)



