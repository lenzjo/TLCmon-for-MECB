;=====================================================================
; I/O starts at $C000.
; ACIA - 68B50
;=====================================================================

ACIABase		.EQ		ACIA1

ACIA_Control	.EQ		ACIABase+$0			; Control reg
ACIA_Status		.EQ		ACIABase+$0			; Status reg
ACIA_Tx			.EQ		ACIABase+$1			; Transmit buffer reg
ACIA_Rx			.EQ		ACIABase+$1			; Receive buffer reg


;=====================================================================
;	ACIA CONSTANTS
;=====================================================================

; Control register bit fields
ACIA_RXI_EN		.EQ		%10000000			; Receive interrupt enable
ACIA_TXI_EN		.EQ		%00100000			; Transmit interrupt enable, /rts low
ACIA_TXI_DS		.EQ		%10011111			; Transmit interrupt disable, /rts low (AND)
ACIA_N_8_1		.EQ		%00010100			; No parity, 8 bit data, 1 stop (see docs)
ACIA_DIV_16		.EQ		%00000001			; Divide tx & rx clock by 16, sample middle
ACIA_RESET		.EQ		%00000011			; Master reset
ACIA_INIT		.EQ		%00010101			; No parity, 8 bit data, 1 stop, Divide by 16

; Status register bit fields
ACIA_RDRF		.EQ		%00000001			; Receive Data Register Full
ACIA_TDRE		.EQ		%00000010			; Transmit Data Register Empty
ACIA_DCD		.EQ		%00000100			; Data Carrier Detect line
ACIA_CTS		.EQ		%00001000			; Clear To Send line
ACIA_ER_F		.EQ		%00010000			; Error- Framing
ACIA_ER_O		.EQ		%00100000			; Error- Overrun
ACIA_ER_P		.EQ		%01000000			; Error- Parity
ACIA_IRQ		.EQ		%10000000			; Interrupt Request
