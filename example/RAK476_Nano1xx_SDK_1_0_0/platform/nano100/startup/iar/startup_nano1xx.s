;******************** (C) COPYRIGHT 2012 STMicroelectronics ********************
;* File Name          : startup_stm32f0xx.s
;* Author             : MCD Application Team
;* Version            : V1.0.0
;* Date               : 23-March-2012
;* Description        : STM32F0xx Devices vector table for EWARM toolchain.
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == iar_program_start,
;*                      - Set the vector table entries with the exceptions ISR 
;*                        address.
;*                      After Reset the Cortex-M0 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;*******************************************************************************
;  @attention
; 
;  Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
;  You may not use this file except in compliance with the License.
;  You may obtain a copy of the License at:
; 
;         http://www.st.com/software_license_agreement_liberty_v2
; 
;  Unless required by applicable law or agreed to in writing, software 
;  distributed under the License is distributed on an "AS IS" BASIS, 
;  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;  See the License for the specific language governing permissions and
;  limitations under the License.
; 
;*******************************************************************************
;
;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  OS_CPU_PendSVHandler            ; uC/OS-III
        EXTERN  OS_CPU_SysTickHandler           ; uC/OS-III
        
        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler                  ; Reset Handler

        DCD     NMI_Handler                    ; NMI Handler
        DCD     HardFault_Handler              ; Hard Fault Handler
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     SVC_Handler                    ; SVCall Handler
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     OS_CPU_PendSVHandler            ; uC/OS-II, was PendSV_Handler
        DCD     OS_CPU_SysTickHandler           ; uC/OS-II, was SysTick_Handler

	; External Interrupts
	DCD     BOD_IRQHandler            ; Brownout low voltage detected interrupt
	DCD     WDT_IRQHandler            ; Watch Dog Timer interrupt
	DCD     EINT0_IRQHandler          ; External signal interrupt from PB.14 pin
	DCD     EINT1_IRQHandler          ; External signal interrupt from PB.15 pin
	DCD     GPABC_IRQHandler          ; External interrupt from PA[15:0]/PB[15:0]/PC[15:0]
	DCD     GPDEF_IRQHandler          ; External interrupt from PD[15:0]/PE[15:0]/PF[7:0]
	DCD     PWM0_IRQHandler           ; PWM 0 interrupt
	DCD     PWM1_IRQHandler           ; PWM 1 interrupt
	DCD     TMR0_IRQHandler           ; Timer 0 interrupt
	DCD     TMR1_IRQHandler           ; Timer 1 interrupt
	DCD     TMR2_IRQHandler           ; Timer 2 interrupt
	DCD     TMR3_IRQHandler           ; Timer 3 interrupt
	DCD     UART0_IRQHandler          ; UART0 interrupt
	DCD     UART1_IRQHandler          ; UART1 interrupt
	DCD     SPI0_IRQHandler           ; SPI0 interrupt
	DCD     SPI1_IRQHandler           ; SPI1 interrupt
	DCD     SPI2_IRQHandler           ; SPI2 interrupt
	DCD     HIRC_IRQHandler           ; HIRC interrupt
	DCD     I2C0_IRQHandler           ; I2C0 interrupt
	DCD     I2C1_IRQHandler           ; I2C1 interrupt
	DCD     Default_Handler           ; Reserved
	DCD     SC0_IRQHandler            ; SC0 interrupt
	DCD     SC1_IRQHandler            ; SC1 interrupt
	DCD     USBD_IRQHandler           ; USB FS Device interrupt
	DCD     TK_IRQHandler             ; Touch key interrupt
	DCD     LCD_IRQHandler            ; LCD interrupt
	DCD     PDMA_IRQHandler           ; PDMA interrupt
	DCD     I2S_IRQHandler            ; I2S interrupt
	DCD     PDWU_IRQHandler           ; Power Down Wake up interrupt
	DCD     ADC_IRQHandler            ; ADC interrupt
	DCD     DAC_IRQHandler            ; DAC interrupt
	DCD     RTC_IRQHandler            ; Real time clock interrupt
        
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER(2)
Reset_Handler
	LDR     R0, =0x50000100
	; Unlock Register
	LDR     R1, =0x59
	STR     R1, [R0]
	LDR     R1, =0x16
	STR     R1, [R0]
	LDR     R1, =0x88
	STR     R1, [R0]

	; Init POR
	LDR     R2, =0x50000060
	LDR     R1, =0x00005AA5
	STR     R1, [R2]

	; Lock register
	MOVS    R1, #0
	STR     R1, [R0]

        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0
        
        PUBWEAK NMI_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
NMI_Handler
        B NMI_Handler
        
        
        PUBWEAK HardFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
HardFault_Handler
        B HardFault_Handler
       
        
        PUBWEAK SVC_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SVC_Handler
        B SVC_Handler
       
        
        PUBWEAK PendSV_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
PendSV_Handler
        B PendSV_Handler
        
        
        PUBWEAK SysTick_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SysTick_Handler
        B SysTick_Handler
        
        PUBWEAK BOD_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
BOD_IRQHandler
        B BOD_IRQHandler

        PUBWEAK WDT_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
WDT_IRQHandler
        B WDT_IRQHandler

        PUBWEAK EINT0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EINT0_IRQHandler
        B EINT0_IRQHandler

        PUBWEAK EINT1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
EINT1_IRQHandler
        B EINT1_IRQHandler

        PUBWEAK GPABC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
GPABC_IRQHandler
        B GPABC_IRQHandler

        PUBWEAK GPDEF_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
GPDEF_IRQHandler
        B GPDEF_IRQHandler

        PUBWEAK PWM0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PWM0_IRQHandler
        B PWM0_IRQHandler

        PUBWEAK PWM1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PWM1_IRQHandler
        B PWM1_IRQHandler

        PUBWEAK TMR0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TMR0_IRQHandler
        B TMR0_IRQHandler

        PUBWEAK TMR1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TMR1_IRQHandler
        B TMR1_IRQHandler

        PUBWEAK TMR2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TMR2_IRQHandler
        B TMR2_IRQHandler

        PUBWEAK TMR3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TMR3_IRQHandler
        B TMR3_IRQHandler

        PUBWEAK UART0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART0_IRQHandler
        B UART0_IRQHandler

        PUBWEAK UART1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART1_IRQHandler
        B UART1_IRQHandler

        PUBWEAK SPI0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI0_IRQHandler
        B SPI0_IRQHandler

        PUBWEAK SPI1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI1_IRQHandler
        B SPI1_IRQHandler

        PUBWEAK SPI2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI2_IRQHandler
        B SPI2_IRQHandler

        PUBWEAK HIRC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
HIRC_IRQHandler
        B HIRC_IRQHandler

        PUBWEAK I2C0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C0_IRQHandler
        B I2C0_IRQHandler

        PUBWEAK I2C1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C1_IRQHandler
        B I2C1_IRQHandler
	
        PUBWEAK Default_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
Default_Handler
        B Default_Handler

        PUBWEAK SC0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SC0_IRQHandler
        B SC0_IRQHandler

        PUBWEAK SC1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SC1_IRQHandler
        B SC1_IRQHandler

        PUBWEAK USBD_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
USBD_IRQHandler
        B USBD_IRQHandler

        PUBWEAK TK_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TK_IRQHandler
        B TK_IRQHandler

        PUBWEAK LCD_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
LCD_IRQHandler
        B LCD_IRQHandler

        PUBWEAK PDMA_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PDMA_IRQHandler
        B PDMA_IRQHandler

        PUBWEAK I2S_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2S_IRQHandler
        B I2S_IRQHandler

        PUBWEAK PDWU_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PDWU_IRQHandler
        B PDWU_IRQHandler

        PUBWEAK ADC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ADC_IRQHandler
        B ADC_IRQHandler
		
		PUBWEAK DAC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DAC_IRQHandler
        B DAC_IRQHandler
		
		PUBWEAK RTC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RTC_IRQHandler
        B RTC_IRQHandler

        END
;************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE*****
