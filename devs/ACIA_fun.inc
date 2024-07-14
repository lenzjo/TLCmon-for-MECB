;=============================================================================
;	Serial Input - 6850 Iniz
;=============================================================================

rs232_int_iniz
				lda #ACIA_RESET				; Reset ACIA
				sta ACIA_ControlRam			; Always save a copy of the register in RAM
				sta ACIA_Control			; Set RAM copy first because hardware generates IRQs
				lda #ACIA_INIT				; Set N-8-1, Div 16 (overwrite old control byte)
				sta ACIA_ControlRam
				sta ACIA_Control
				lda ACIA_ControlRam			; Always load the control byte image from RAM
				ora #ACIA_RXI_EN			; Also enable Rx interrupt (no Tx IRQ at this time)
				sta ACIA_ControlRam
				sta ACIA_Control
				rts


;=============================================================================
;	Serial Input - IRQ driven & 128 byte buffer
;=============================================================================

;=-=-=-=  Did the ACIA cause the int?  =-=-=-=

ACIA_isr
				lda	ACIA_STATUS				; Get status reg -> irq bit to n flag
				bpl	ai_next_irq				; No int, then go exit

				sta ACIA_StatusRam			; Store in MOS variable in RAM

				lda #ACIA_RDRF
				bit ACIA_StatusRam			; Did a ch come in?
				bne ai_rd_chr				; 	Yes, so go get received ch
ai_next_irq
				jmp	(IRQ_VEC1) 				; Go handle next irq


;=-=-=-=  Send a chr from the ACIA to the KYBD buffer  =-=-=-=

ai_rd_chr
				lda ACIA_Rx					; Get chr from 6850
				ldy KBUFF_CNT				; and the buff ctr 
				bmi	ai_next_irq				; then exit if full

				ldy KBUFF_WR_PTR 			; Get next free pos
				sta KYBD_BUFFER,y 			; and save chr in buff
				iny 						; increment tail ptr 
				bpl	ai_nowrap				; No wrap around so skip over 
			
				ldy #$00 					; else, reset ptr
ai_nowrap
				sty KBUFF_WR_PTR 			; update buff tail ptr
				inc KBUFF_CNT 				; Inc chr cnt
				bra ai_next_irq


;=-=-=-=  Get a ch from the KYBD buffer  =-=-=-=

rs232_getc
				lda	KBUFF_CNT				; Wait until
				beq	rs232_getc				; there's a chr in the buff

				phy
				php
				ldy	KBUFF_RD_PTR			; Go to front of queue
				lda	KYBD_BUFFER,y			; and get the chr
				iny
				bpl	tgc_nowrap				; Chk for wrap, skip over if none
			
				ldy	#$00					; Reset the buff ptr
tgc_nowrap
				sty	KBUFF_RD_PTR			; Save buff ptr
				dec	KBUFF_CNT				; reduce cnt
				plp
				ply
				rts


;=-=-=-=  Send a ch to the ACIA  =-=-=-=

rs232_putc
				sta ACIA_Tx					; send char
				pha
apc_loop
				lda	ACIA_Status
				and	#ACIA_TDRE				; is buffer empty?
				beq	apc_loop				; no, then test again!
				pla
				rts

