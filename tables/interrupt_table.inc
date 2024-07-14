;=============================================================================
;  Vector table data for default ROM handlers
;=============================================================================

VEC_ETABLE
				.DW 	NMI_vector			; NMI Location in ROM
				.DW		NMI_exit			; NMI 1 return handler in ROM

				.DW 	ACIA_isr			; UART isr Location in ROM
				.DW 	IRQ_exit			; IRQ 1 return handler in ROM
				.DW 	IRQ_exit			; IRQ 2 return handler in ROM
				.DW 	IRQ_exit			; IRQ 3 return handler in ROM
				.DW 	IRQ_exit			; IRQ 4 return handler in ROM
				.DW 	IRQ_exit			; IRQ 5 return handler in ROM