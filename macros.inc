;=====================================================================
; My Macros to make coding more readable
;=====================================================================

;=-=-=-=  Move an address into regs A & Y  =-=-=-=

CPYMSPTR		.MA	ADDR
				lda #]1
				ldy /]1
				.EM


;=-=-=-=  Multiply Acc by 8  =-=-=-=

MULTAx8			.MA
				clc
				asl a
				asl a
				asl a
				.EM


;=-=-=-=  Copy a Word  =-=-=-=

CPYW			.MA WORDA, WORDB
				lda ]1
				sta ]2
				lda ]1+1
				sta ]2+1
				.EM


;=-=-=-=  Compare two numbers  =-=-=-=

CMPAD			.MA MEMA, MEMB
				lda ]1+1
				cmp ]2+1
				bne :CA_EXIT
				lda ]1
				cmp ]2
:CA_EXIT
				.EM


;=-=-=-=  Multiply a word by 2  =-=-=-=

MULTW2			.MA WORD
				asl ]1
				rol	]1+1
				.EM


;=-=-=-=  Increment a Word  =-=-=-=

INCW			.MA VAL
				inc ]1
				bne :IW_EXIT
				inc ]1+1
:IW_EXIT
				.EM


;=-=-=-=  Decrement a Word  =-=-=-=

DECW			.MA WORD
				lda ]1
				bne :DW
				dec ]1+1
:DW
				dec ]1
				.EM


;=-=-=-=  Get diff of SRC-UNTIL  =-=-=-=

GETLEN			.MA start,end,size
				sec
				lda ]2
				sbc ]1
				sta ]3
				lda ]2+1
				sbc ]1+1
				sta ]3+1
				.EM


;=-=-=-=  ADD 2 addrs  =-=-=-=

ADDADDRS		.MA size,addr
				clc
				lda ]1
				adc ]2
				sta ]1
				lda ]1+1
				adc ]2+1
				sta ]1+1
				.EM






