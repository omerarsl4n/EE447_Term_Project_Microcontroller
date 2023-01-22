; EQU Directives
; These directives do not allocate memory
;***************************************************************
RCGCGPIO 				EQU 0x400FE608
RCGCSSI 				EQU 0x400FE61C
SSI_CR0 				EQU 0x40008000
SSI_CR1 				EQU 0x40008004
SSI_CPSR 				EQU 0x40008010
SSI_ICC 				EQU 0x40008FC8
GPIO_PORTA_BASE 		EQU 0x40004000
GPIO_PORTA_DIR 			EQU 0x40004400
GPIO_PORTA_AFSEL 		EQU 0x40004420
GPIO_PORTA_DEN 			EQU 0x4000451C
GPIO_PORTA_PCTL 		EQU 0x4000452C
RESET_COUNTER 			EQU 0x82354
SSI_DR 					EQU 0x40008008
SSI_SR 					EQU 0x4000800C
;***************************************************************
; Directives - This Data Section is part of the code
; It is in the read only section so values cannot be changed.
;***************************************************************
***************************************************************
; Program section
;***************************************************************
;LABEL DIRECTIVE VALUE COMMENT
		AREA main, READONLY, CODE
		THUMB
		EXPORT InitSPI ; Make available
; TM4C123GH6PM page 965 is followed
InitSPI PUSH{R0-R1}
		LDR R1, =RCGCSSI
		LDR R0, [R1]
		ORR R0, R0, #0x01
		STR R0, [R1]
		NOP
		NOP
		NOP
		;START PORT A
		LDR R1, =RCGCGPIO
		LDR R0, [R1]
		ORR R0, R0, #0x01
		STR R0, [R1]
		;STABILIZE CLOCK
		NOP
		NOP
		NOP
		;DIGITAL ENABLE
		LDR R1, =GPIO_PORTA_DEN
		LDR R0, [R1]
		ORR R0, #0xFC
		STR R0, [R1]
		;SET DIRECTION(input-output)
		LDR R1, =GPIO_PORTA_DIR
		MOV R0, #0xEC
		STR R0, [R1]
		;alternate function for the required pins
		LDR R1, =GPIO_PORTA_AFSEL
		MOV R0, #0x3C
		STR R0, [R1]
		;port control
		LDR R1, =GPIO_PORTA_PCTL
		LDR R0, =0x00202200
		STR R0, [R1]
		;ssi control1
		LDR R1, =SSI_CR1
		MOV R0, #0x0
		STR R0, [R1]
		;configure clock
		LDR R1, =SSI_ICC
		MOV R0, #0x05
		STR R0, [R1]
		;prescaler
		LDR R1, =SSI_CPSR
		MOV R0, #0x02
		STR R0, [R1]
		LDR R1, =SSI_CR0
		MOV R0, #0x707
		STR R0, [R1]
		LDR R1, =SSI_CR1
		MOV R0, #0x010
		STR R0, [R1]
		LDR R1, =SSI_CR1
		LDR R0, [R1]
		ORR R0, R0, #0x02
		STR R0, [R1]
		LDR R1, =GPIO_PORTA_BASE
		MOV R0,#0x40
		STR R0,[R1,#0x100]
		POP{R0,R1}
		BX LR
		ALIGN
		END
