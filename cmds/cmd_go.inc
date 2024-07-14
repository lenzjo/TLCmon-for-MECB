;=============================================================================
;	go addr
;=============================================================================

cmd_go
				lda ARGCNT					; How many args?
				cmp #$01					; Only one?
				beq cex_arg1				;   Yes, go do cmd
cex_bexit
				jsr pr_err_arwr				;   No, Show err msg
				rts							;   and exit
cex_arg1
				ldx ARG1					; Get ptr to Arg1
				jsr get_SRC_addr			; Attempt to get the addr
				bcs exec_code				;   Yes, go do it

				jsr pr_err_inadr			;   No, show err msg
				rts							;   and exit


;=============================================================================
;	Alternate Entry Point: If SRC, UNTIL and BYTE parm's are set up already
;=============================================================================

exec_code
				jmp (src)


copy_code
				ldx #0
				ldy #5
coco_lp
				lda dummy,x
				sta $500,x
				inx
				dey
				bne coco_lp

				rts


dummy
				.DB $A9, $AA, $85, $00, $60