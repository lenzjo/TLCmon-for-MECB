;=============================================================================
;	Table of cmds and their format
;=============================================================================

;====  Each cmd can be no more than 8 chr INCLUDING the NULL terminator  ====
;====
;====  $		: prefix denotes a hex number
;====  %		; prefix denotes a binary number
;====  n		: a single digit 0-9
;====  h		: a single digit 0-9 or letter A-F
;====  b		: a single digit 0-1
;====  n..		: one or more digits
;====  x		: a single letter
;====  a		: a single letter or number
;====  ext		: xxx
;====  count,
;====  addr		: $h[h..] | n[n..] | %b[b..] in range 0-65535
;====  from,
;====  src,
;====  until,
;====  to,
;====  dest		: addr
;====  dbyte	: $h[h..] | n[n..] | %b[b..] in range 0-255


; COMMANDS FORMAT
VALID_CMDS
				;==== Returns the DB at location addr
				.DB "PEEK", NULL, NULL, NULL, NULL				; peek addr

				;==== Writes DB at location addr
				.DB "POKE", NULL, NULL, NULL, NULL				; poke addr,dbyte

				;====  Display a hex dump from addr for either count or 100 dbytes
				.DB "DUMP", NULL, NULL, NULL, NULL				; dump from[,to]


				;==== Edit memory starting at addr
				.DB "EDMEM", NULL, NULL, NULL					; edmem addr

				;====  Display help page
				.DB "HELP", NULL, NULL, NULL, NULL				; help

				;====  Copy memory block src-until to dest
				.DB "COPY", NULL, NULL, NULL, NULL				; copy src,until,dest


				;====  Fill memory range src-until with DB
				.DB "FILL", NULL, NULL, NULL, NULL				; fill src,until,dbyte

				;====  Convert a number from any base to hex
				.DB "HEX", NULL, NULL, NULL, NULL, NULL			; hex %b | n

				;====  Execute code at location addr
				.DB "GO", NULL, NULL, NULL, NULL, NULL, NULL	; exec addr


				;====  Dis-assemble memory from src to either until or 100 dbytes
				.DB "DISM", NULL, NULL, NULL, NULL				; dism src[,until]

				;==== Uptime show time elapsed since turn-on
				.DB "UPTIME", NULL, NULL						; uptime

				;====  Convert a number from any base to decimal
				.DB "DEC", NULL, NULL, NULL, NULL, NULL			; dec $h | %b


				;====  Convert a number from any base to binary
				.DB "BIN", NULL, NULL, NULL, NULL, NULL			; bin $h | n

				
				.DB NULL					; Cmd-Table end-marker is a NULL
