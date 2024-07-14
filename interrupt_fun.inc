;==============================================================================
;	Iniz the Interrupt vector table
;==============================================================================

iniz_intvec
				ldy	#$00
inin_loop
				lda	VEC_ETABLE,y			; Get Vector table data
				sta	VEC_RTABLE,y			; Store in Soft Vector jmp location
				iny							; Increment index
				cpy #16
				bne	inin_loop				; Loop back till done
				rts							; Return to caller


;==============================================================================
;	IRQ Entry point and exit function
;==============================================================================

;=-=-=-=  This is the ROM start for the IRQ handler  =-=-=-=

IRQ_vector
				pha							; Save A Reg (3)
				phx							; Save X Reg (3)
				phy							; Save Y Reg (3)
				jmp	(IRQ_VEC0)				; Jump to Soft vectored IRQ Handler (5)


;=-=-=-=  This is the standard return for the IRQ handler  =-=-=-=

IRQ_exit
				ply							; Restore Y Reg (4)
				plx							; Restore X Reg (4)
				pla							; Restore A Reg (4)
				rti							; Return from IRQ routine (6)


;==============================================================================
;	NMI Entry point and exit function
;==============================================================================

NMI_vector
				pha							; Save A Reg (3)
				phx							; Save X Reg (3)
				phy							; Save Y Reg (3)
				jmp	(NMI_VEC0)				; Jump to Soft vectored NMI Handler (5)


NMI_exit
				ply							; Restore Y Reg (4)
				plx							; Restore X Reg (4)
				pla							; Restore A Reg (4)
				rti							; Return from NMI routine (6)