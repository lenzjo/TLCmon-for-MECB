;=====================================================================
; ASCII and Control Character Constants
;=====================================================================

NULL			.EQ		$00
BELL			.EQ		$07
BS				.EQ		$08
TAB				.EQ		$09
LF				.EQ		$0A
CLEAR_SCREEN	.EQ		$0C
CR				.EQ		$0D

ESC				.EQ		$1B					; ESC - reserved for ANSI seqences
CUR_RIGHT		.EQ		$1C
CUR_LEFT		.EQ		$1D
CUR_UP			.EQ		$1E
CUR_DOWN		.EQ		$1F

DEL				.EQ		$7F

SPC				.EQ		$20
BANG			.EQ		$21					; !
QUOTE			.EQ		$22					; "
HASH			.EQ		$23					; #
DOLLAR			.EQ		$24					; $
HEXADECIMAL		.EQ		$24					; $
PERCENT         .EQ     $25                 ; %
BINARY	        .EQ     $25                 ; %
LPAREN			.EQ		$28					; (
RPAREN			.EQ		$29					; )
STAR			.EQ		$2A					; *
COMMA			.EQ		$2C					; ,
MINUS			.EQ		$2D					; -
PERIOD			.EQ		$2E					; .
COLON			.EQ		$3A					; :
EQUALS			.EQ		$3D					; =
REDIR_OUT		.EQ		$3E					; >
HELPME			.EQ		$3F					; ?
LSBRACKET		.EQ		$5B					; [
RSBRACKET		.EQ		$5D					; ]
PIPE			.EQ		$7C					; |
