;=============================================================================
;	Is this ch a space, comma or colon
;		On Exit: clc=No, sec=Yes
;=============================================================================

is_seperator
				cmp #SPC
				beq isse_gexit

				cmp #COMMA
				beq isse_gexit

				cmp #COLON
				beq isse_gexit
isse_bexit
				clc
				rts
isse_gexit
				sec
				rts


;=============================================================================
;	Is this ch a number or a letter?
;		On Exit: clc=No, sec=Yes
;=============================================================================

is_alphanumeric
				jsr is_numeric
				bcc isalet

				rts							; Exit if NOT numeric
isalet
				jsr is_alpha
				rts


;=============================================================================
;	Is this ch a letter?
;		On Exit: clc=No, sec=Yes
;=============================================================================

is_alpha
				jsr is_lowercase
				bcc isalp

				rts							; Exit if NOT lowercase

isalp
				jsr is_uppercase
				rts


;=============================================================================*
;	Is this ch lowercase?
;		On Exit: clc=No, sec=Yes
;=============================================================================

is_lowercase
				cmp #'a'
				bcc islcbad					; < 'a'

				cmp #'z'+1
				bcs islcbad					; >= 'z'

				sec
				rts
islcbad
				clc
				rts


;=============================================================================
;	Is this ch uppercase?
;		On Exit: clc=No, sec=Ye;
;=============================================================================

is_uppercase
				cmp #'A'
				bcc isucbad					; < 'A'

				cmp #'Z'+1
				bcs isucbad					; >= 'Z'

				sec
				rts
isucbad
				clc
				rts


;=============================================================================
;	Is this ch a number?
;		On Exit: clc=No, sec=Yes
;=============================================================================

is_numeric
				cmp #'0'
				bcc isnu_bexit				; < '0'

				cmp #'9'+1
				bcc isnu_gexit				; <= '9'
isnu_bexit
				clc
				rts
isnu_gexit
				sec
				rts


;=============================================================================
;	Is this ch a number?
;		On Exit: clc=No, sec=Yes
;=============================================================================

is_binary
				cmp #'0'
				beq isbi_gexit				; = '0'

				cmp #'1'
				beq isbi_gexit				; = '1'
isbi_bexit
				clc
				rts
isbi_gexit
				sec
				rts
