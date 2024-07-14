;=============================================================================
;	TLCmon for MECB
;=============================================================================

				.CR     65c02
				.TF     tlcmon.bin,BIN
				.LF     tlcmon.list

;=============================================================================
;	Construction Progress
;=============================================================================
;
; Serial Port				-	Working	28/05/24
; Interrupt rx chrs			-	Working 28/05/24
;
; cmd input
;	v1 - cmd buff & edit	-	Working 28/05/24
;	v2 - parse cmds			-	Working 28/05/24
;	v3 - parse args			-	Working 28/05/24
;	v4 - parse redirects	-
;
; cmd exec via table		-	Working 28/05/24
; cmd help via table		-	Working 28/05/24
;
; stdout redirection		-
;
; Keyboard history			-
;
; Cmds:
;	peek					-	Working 29/05/24
;	poke					-	Working 29/05/24
;	dump					-	Working 29/05/24
;	copy					-	Working 29/05/24
;	fill					-	Working 29/05/24
;	edmem					-	Working 29/05/24
;	go						-	Working 29/05/24
;	help					-	Working 28/05/24
;	hex						-	Working 12/07/24
;	dec						-	Working 12/07/24
;	bin						-	Working 12/07/24
;	load					-
;	save					-
;	time					-
;	uptime					-
;
;=============================================================================

;=-=-=-= TLCmon Version number =-=-=-=

VerMaj			.EQ		1
VerMin			.EQ		3
VerMnt			.EQ		9


;=============================================================================
;	Program constants
;=============================================================================

RAM_Base		.EQ		$0000
RAM_End			.EQ		RAM_Base + $BFFF

ROM_Base		.EQ 	$8000				; Physical Start of eeprom
entry			.EQ		$C100				; Monitor Entry point

ACIA1			.EQ		$C000				; 6850 location
PTM_BASE		.EQ		$C010				; 6840 location
PIA_BASE		.EQ		$C020				; 6821 location


				.IN		ascii_con.inc

				.IN		devs/ACIA_cons.inc


MAXCMD			.EQ 	$7F					; Maximum cmdline length
NO_CMD			.EQ 	$FF


ISHEX			.EQ 	$00
ISDEC			.EQ 	$01


;=============================================================================
;	Zero Page Program Registers
;=============================================================================

ZPage			.EQ		$00

KBUFF_RD_PTR	.EQ		ZPage				; Input buff head ptr
KBUFF_WR_PTR	.EQ		ZPage+1				; Input buff tail ptr
KBUFF_CNT		.EQ		ZPage+2				; Input buff count

MSG_PTR			.EQ		ZPage+3				; 2 bytes!!

CMD_BUF_PTR		.EQ		ZPage+5


;=-=-=-= CMD_IBUFF regs =-=-=-=

THISCMD			.EQ 	ZPage+6				; cmd index ptr
ARG1			.EQ 	ZPage+7				; ptr to 1st arg on cmd line
ARG2			.EQ 	ZPage+8				; ptr to 2nd arg on cmd line
ARG3			.EQ 	ZPage+9				; ptr to 3rd arg on cmd line
ARG4			.EQ 	ZPage+10			; ptr to 4th arg on cmd line
ARGCNT			.EQ 	ZPage+11			; how many args on cmd line
CMDXPTR			.EQ 	ZPage+12			; Holds the x ptr for the CMD_IBUFF

SRC				.EQ 	ZPage+14			; Start addr
UNTIL			.EQ 	ZPage+16
DEST			.EQ 	ZPage+18
LEN				.EQ 	ZPage+20
ROMAD			.EQ		ZPage+22
BYTE			.EQ 	ZPage+24


ASCBYTE			.EQ 	ZPage+26		; 2 BYTEs
HEX_RES			.EQ 	ZPage+28		; 2 BYTEs for hex results

BYTECNT			.EQ 	ZPage+32
NUMBASE			.EQ 	ZPage+34

DEC_BASE		.EQ		ZPage+35
DEC_RES			.EQ		ZPage+37		; 5 Bytes

ACIA_ControlRam	.EQ		ZPage+45
ACIA_StatusRam	.EQ		ZPage+46

BIN_CTR			.EQ		ZPage+47


;=-=-=-=  Interrupt Vectors  =-=-=-=

iptr			.EQ 	$80

VEC_RTABLE		.EQ 	iptr

NMI_VEC0		.EQ		iptr				; nmi vector 0
NMI_VEC1		.EQ		iptr+2				; nmi vector 1

IRQ_VEC0		.EQ		iptr+4				; irq vector 0
IRQ_VEC1		.EQ		iptr+6				; irq vector 1
IRQ_VEC2		.EQ		iptr+8				; irq vector 2
IRQ_VEC3		.EQ		iptr+10				; irq vector 3
IRQ_VEC4		.EQ		iptr+12				; irq vector 4
IRQ_VEC5		.EQ		iptr+14				; irq vector 5


;=============================================================================
;	RAM buffers and stuff
;=============================================================================

KYBD_BUFFER		.EQ		$0200				; INPUT BUFF  128 BYTES 

CMD_BUFFER		.EQ 	$0280				; cmdline buffer

ASCIIDUMP		.EQ		$0300				; text formatting area for dump cmd


;=============================================================================
;	Macros MUST be first before any code
;=============================================================================

				.IN		macros.inc


;=============================================================================
;	Rom start
;=============================================================================

				.OR		ROM_Base

				nop


;=============================================================================
;	TLCmon start
;=============================================================================

				.NO		entry

RST_vector


Main
				sei							; Pause interrupts
				cld							; clear decimal mode
				ldx #$00
pgz_lp
				stz $00,x					; clr page zero
				dex
				bne pgz_lp					; loop til done

				dex							; $ff - ready for stack ptr
				txs

				jsr iniz_intvec				; Setup interrupt vector table in ZP

				jsr rs232_int_iniz			; Setup 6850

				cli							; Resume interrupts

				jsr pr_MonHeader			; Show Header
cmd_loop
				jsr pr_MonPrompt			; Show Prompt

				jsr get_cmdline				; Read in a cmd line
				beq cmd_loop				; If empty then retry

				jsr parse_cmdline			; Is it a valid cmd?
				bcs cmd_doit				;   Yes, then go do it.
cmd_bad
				jsr pr_err_nocmd			;   Else, show error msg
				bra cmd_loop				;   and go retry.
cmd_doit
				jsr pr_cmd_help				; Was 1st arg = help?
				bcs cmd_loop				;   Yes, then go get next cmd

				jsr exec_cmd				; Execute the cmd line
				bra cmd_loop


;=============================================================================
;	Program functions
;=============================================================================


				.IN		interrupt_fun.inc
				.IN		devs/ACIA_fun.inc
				.IN		print_fun.inc
				.IN		cmdline/cmds_fun.inc
				.IN		ch_tests.inc


put_c			jsr rs232_putc
				rts


get_c			jsr rs232_getc
				rts


;=============================================================================
;	COMMANDS functions
;=============================================================================

				.IN 	cmds/cmd_peek.inc
				.IN 	cmds/cmd_poke.inc
				.IN 	cmds/cmd_dump.inc

				.IN 	cmds/cmd_edmem.inc
				.IN 	cmds/cmd_help.inc
				.IN 	cmds/cmd_copy.inc

				.IN 	cmds/cmd_fill.inc
				.IN 	cmds/cmd_hex.inc
				.IN 	cmds/cmd_go.inc

				.IN 	cmds/cmd_dism.inc
				.IN 	cmds/cmd_uptime.inc
				.IN 	cmds/cmd_dec.inc

				.IN 	cmds/cmd_bin.inc
				

;=============================================================================
;	Program tables
;=============================================================================

				.NO		$FE00
											; 13x2 = 26 bytes ($1A)
				.IN		tables/help_table.inc

				.NO		$FE20
											; 20x8 + 8 =160 bytes ($A0)
				.IN		tables/cmds_table.inc

				.NO 	$FED0
											; 8x2bytes = 16 bytes ($10)
				.IN		tables/interrupt_table.inc

				.NO		$FEE0				
											; 20x2 = 40 bytes ($28)
				.IN		tables/cmds_jump_table.inc



;=-=-=-=  Interrupt Vectors  =-=-=-=

				.NO		$FFFA

                .DW		NMI_vector			; NMI
                .DW		RST_vector			; RESET
                .DW		IRQ_vector			; BRK/IRQ

				.end