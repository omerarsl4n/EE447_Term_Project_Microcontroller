
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
tlocation				EQU 0x20007000
minlocation				EQU 0x20007004
maxlocation				EQU 0x20007008

;***************************************************************
;LABEL DIRECTIVE VALUE COMMENT
	 AREA sdata, DATA, READONLY
	 THUMB
			
TEMP_WORD	DCB 0x01, 0x01, 0x7f, 0x01, 0x01
			DCB 0x38, 0x54, 0x54, 0x54, 0x18
			DCB 0x7c, 0x04, 0x18, 0x04, 0x78
			DCB 0x7c, 0x14, 0x14, 0x14, 0x08
			DCB 0x00, 0x00, 0x00, 0x00, 0x00
			DCB 0x00, 0x36, 0x36, 0x00, 0x00
			DCB 0x42, 0x61, 0x51, 0x49, 0x46
			DCB 0x42, 0x61, 0x51, 0x49, 0x46
			DCB 0x00, 0x60, 0x60, 0x00, 0x00
			DCB 0x42, 0x61, 0x51, 0x49, 0x46

MIN_WORD 	DCB	0x7f, 0x02, 0x0c, 0x02, 0x7f
			DCB	0x00, 0x44, 0x7d, 0x40, 0x00
			DCB	0x7c, 0x08, 0x04, 0x04, 0x78
			DCB 0x00, 0x00, 0x00, 0x00, 0x00
			DCB 0x00, 0x00, 0x00, 0x00, 0x00
			DCB 0x00, 0x36, 0x36, 0x00, 0x00
			DCB 0x42, 0x61, 0x51, 0x49, 0x46
			DCB 0x18, 0x14, 0x12, 0x7f, 0x10
			DCB 0x00, 0x60, 0x60, 0x00, 0x00
			DCB 0x42, 0x61, 0x51, 0x49, 0x46
			
MAX_WORD 	DCB	0x7f, 0x02, 0x0c, 0x02, 0x7f
			DCB	0x20, 0x54, 0x54, 0x54, 0x78
			DCB	0x44, 0x28, 0x10, 0x28, 0x44
			DCB 0x00, 0x00, 0x00, 0x00, 0x00
			DCB 0x00, 0x00, 0x00, 0x00, 0x00
			DCB 0x00, 0x36, 0x36, 0x00, 0x00
			DCB 0x42, 0x61, 0x51, 0x49, 0x46
			DCB 0x21, 0x41, 0x45, 0x4b, 0x31
			DCB 0x00, 0x60, 0x60, 0x00, 0x00
			DCB 0x42, 0x61, 0x51, 0x49, 0x46
			
DOT			DCB 0x00, 0x60, 0x60, 0x00, 0x00

ASCII_num	DCB 0x3e, 0x51, 0x49, 0x45, 0x3e
			DCB 0x00, 0x42, 0x7f, 0x40, 0x00 
			DCB 0x42, 0x61, 0x51, 0x49, 0x46 
			DCB 0x21, 0x41, 0x45, 0x4b, 0x31 
			DCB 0x18, 0x14, 0x12, 0x7f, 0x10 
			DCB 0x27, 0x45, 0x45, 0x45, 0x39 
			DCB 0x3c, 0x4a, 0x49, 0x49, 0x30 
			DCB 0x01, 0x71, 0x09, 0x05, 0x03
			DCB 0x36, 0x49, 0x49, 0x49, 0x36 
			DCB 0x06, 0x49, 0x49, 0x29, 0x1e 
;***************************************************************
;LABEL DIRECTIVE VALUE COMMENT
		AREA main, READONLY, CODE
		THUMB
		EXPORT initializeScreen
		EXPORT drawMeasure
		EXPORT drawMin
		EXPORT drawMax
;
; Command is in R2
;
sendCommand PROC
		PUSH{R3}
		PUSH{LR}
		MOV R3,#0x00
		BL write
		POP {LR}
		POP {R3}
		BX LR
		ENDP
; Data is in R2
;
sendData PROC
		PUSH{R3}
		PUSH{LR}
		MOV R3,#0x80 ;Indicate data
		BL write
		POP {LR}
		POP {R3}
		BX LR
		ENDP
;
; Set temperature, vop and another parameters
; Then, draw Meas: and Thre: strings to screen
initializeScreen PROC
		PUSH{LR}
		PUSH{R0-R6}
		BL resetScreen
		MOV R2,#0x21
		BL sendCommand
		MOV R2,#0xBA
		BL sendCommand
		MOV R2,#0x05
		BL sendCommand
		MOV R2,#0x13
		BL sendCommand
		MOV R2,#0x20
		BL sendCommand
		MOV R2,#0x0C
		BL sendCommand
		MOV R0,#0x0
		MOV R1,#0x0
		BL setCursor
		MOV R0,#0x1F8
		MOV R2,#0x00
clearLoop BL sendData
		SUBS R0,#1
		BNE clearLoop
		MOV R0,#0x0
		MOV R1,#0x0
		BL setCursor
		MOV R4,#0x32
		LDR R3, =TEMP_WORD
		LDR R5, =MIN_WORD
		LDR R6, =MAX_WORD
drawM 	LDRB R2,[R3],#1
		BL sendData
		ADD R1,#1
		BL setCursor
		LDRB R2,[R5],#1
		BL sendData	
		ADD R1,#1
		BL setCursor
		LDRB R2,[R6],#1
		BL sendData
		SUB R1,#2
		ADD R0,#1
		BL setCursor
		SUBS R4,#0x1
		BNE drawM 
		POP{R0-R6}
		POP{LR}
		BX LR
		ENDP
; R5 memory location show each number
; Obtained data from convert subroutine is passed both draw measure and
drawMeasure PROC
		PUSH{LR}
		PUSH{R0-R6}
		LDR R5,=tlocation
		MOV R0,#30
		MOV R1,#0x0
		BL setCursor
		MOV R1,#2
		MOV R2,#5
		LDR R6,=ASCII_num
loop3 	LDRB R0,[R5],#1
		MUL R0,R0,R2
		ADD R4,R6,R0
		PUSH{R2,R3}
		MOV R3,#5
loop4 	LDRB R2,[R4],#1
		BL sendData
		SUBS R3,#1
		BNE loop4
		POP{R2,R3}
		SUBS R1,#1
		BNE loop3
		LDR R4,= DOT
		MOV R3, #5
drloop	LDRB R2,[R4],#1
		BL sendData
		SUBS R3, #1
		BNE drloop
		MOV R2,#5
		LDRB R0,[R5]
		MUL R0,R0,R2
		ADD R4,R6,R0
		MOV R3, #5
lastb	LDRB R2,[R4],#1
		BL sendData
		SUBS R3,#1
		BNE lastb
		POP{R0-R6}
		POP{LR}
		BX LR
		ENDP

drawMin PROC
		PUSH{LR}
		PUSH{R0-R6}
		LDR R5,=minlocation
		MOV R0,#30
		MOV R1,#0x1
		BL setCursor
		MOV R1,#2
		MOV R2,#5
		LDR R6,=ASCII_num
loop3_a LDRB R0,[R5],#1
		MUL R0,R0,R2
		ADD R4,R6,R0
		PUSH{R2,R3}
		MOV R3,#5
loop4_a	LDRB R2,[R4],#1
		BL sendData
		SUBS R3,#1
		BNE loop4_a
		POP{R2,R3}
		SUBS R1,#1
		BNE loop3_a
		LDR R4,= DOT
		MOV R3, #5
drloop_a LDRB R2,[R4],#1
		BL sendData
		SUBS R3, #1
		BNE drloop_a
		MOV R2,#5
		LDRB R0,[R5]
		MUL R0,R0,R2
		ADD R4,R6,R0
		MOV R3, #5
lastbit_a LDRB R2,[R4],#1
		BL sendData
		SUBS R3,#1
		BNE lastbit_a
		POP{R0-R6}
		POP{LR}
		BX LR
		ENDP

drawMax PROC
		PUSH{LR}
		PUSH{R0-R6}
		LDR R5,=maxlocation
		MOV R0,#30
		MOV R1,#0x2
		BL setCursor
		MOV R1,#2
		MOV R2,#5
		LDR R6,=ASCII_num
loop3_b LDRB R0,[R5],#1
		MUL R0,R0,R2
		ADD R4,R6,R0
		PUSH{R2,R3}
		MOV R3,#5
loop4_b LDRB R2,[R4],#1
		BL sendData
		SUBS R3,#1
		BNE loop4_b
		POP{R2,R3}
		SUBS R1,#1
		BNE loop3_b
		LDR R4,= DOT
		MOV R3, #5
drloop_b	LDRB R2,[R4],#1
		BL sendData
		SUBS R3, #1
		BNE drloop_b
		MOV R2,#5
		LDRB R0,[R5]
		MUL R0,R0,R2
		ADD R4,R6,R0
		MOV R3, #5
lastbit_b LDRB R2,[R4],#1
		BL sendData
		SUBS R3,#1
		BNE lastbit_b
		POP{R0-R6}
		POP{LR}
		BX LR
		ENDP
; get apporiate bits to be used in drawing
; R5 memory location show each number

;Sets set cursor command for both in X and Y coordinates given in R0 and R1
setCursor PROC
		PUSH{R0-R2}
		PUSH{LR}
		ORR R2,R0,#0x80
		BL sendCommand
		MOV R2,#0x0
		ORR R2,R1,#0x40
		BL sendCommand
		POP{LR}
		POP{R0-R2}
		NOP
		NOP
		NOP
		BX LR
		ENDP
;
;Data is in R2
;DC Pin Value in R3 -> 0x80 for Data write 0x00 for Command Write
;
write   PROC
		PUSH {R0-R3}
		LDR R1, =GPIO_PORTA_BASE
		STR R3, [R1,#0x200]
		LDR R1, =SSI_SR
waitFIFO_RD LDR R0,[R1] ; Check if transmit FIFO is full or not
		ANDS R0,#0x02
		BEQ waitFIFO_RD
		LDR R1, =SSI_DR
		STR R2,[R1]
		LDR R1, =SSI_SR
waitFinish LDR R0, [R1]
		ANDS R0, #0x10
		BNE waitFinish
		POP {R0-R3}
		BX LR
		ENDP
; RESETS THE SCREEN
resetScreen PROC
		PUSH {R0,R1}
		LDR R1, =GPIO_PORTA_BASE
		MOV R0,#0x00
		STR R0,[R1,#0x100]
		LDR R10, =RESET_COUNTER
countDown SUBS R10, #1
		BNE countDown
		MOV R0,#0x40
		STR R0,[R1,#0x100]
		POP {R0,R1}
		BX LR
		ENDP
		;***************************************************************
		; End of the program section
		;***************************************************************
		;LABEL DIRECTIVE VALUE COMMENT
		ALIGN
		END
