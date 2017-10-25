;/******************** (C) COPYRIGHT 2016 Hitrendtech. **************************
;* File Name          : startup_ht501x.s
;* Author             : Hitrendtech_SocTeam
;* Version            : V1.5.0
;* Date               : 04/07/2016
;* Description        : This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == __iar_program_start,
;*                      - Set the vector table entries with the exceptions ISR 
;*                        address.
;********************************************************************************
;* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
;* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
;* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
;* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
;* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
;* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
;*******************************************************************************/
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

        SECTION .password:CODE
        DCD     0xFFFFFFF2

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler             ; Reset Handler
        DCD     NMI_Handler               ; NMI Handler
        DCD     HardFault_Handler         ; Hard Fault Handler
        DCD     0                         ; MPU Fault Handler
        DCD     0                         ; Bus Fault Handler
        DCD     0                         ; Usage Fault Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     SVC_Handler               ; SVCall Handler
        DCD     0                         ; Debug Monitor Handler
        DCD     0                         ; Reserved
        DCD     PendSV_Handler            ; PendSV Handler
        DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     PMU_IRQHandler            ; 16+ 0: PMU
                DCD     AES_IRQHandler            ; 16+ 1: AES
                DCD     EXTI0_IRQHandler          ; 16+ 2: EXTI0
                DCD     EXTI1_IRQHandler          ; 16+ 3: EXTI1
                DCD     EXTI2_IRQHandler          ; 16+ 4: EXTI2
                DCD     EXTI3_IRQHandler          ; 16+ 5: EXTI3
                DCD     EXTI4_IRQHandler          ; 16+ 6: EXTI4
                DCD     EXTI5_IRQHandler          ; 16+ 7: EXTI5
                DCD     EXTI6_IRQHandler          ; 16+ 8: EXTI6
                DCD     UART0_IRQHandler          ; 16+ 9: UART0
                DCD     UART1_IRQHandler          ; 16+10: UART1
                DCD     UART2_IRQHandler          ; 16+11: UART2
                DCD     UART3_IRQHandler          ; 16+12: UART3
                DCD     UART4_IRQHandler          ; 16+13: UART4
                DCD     UART5_IRQHandler          ; 16+14: UART5
                DCD     TIMER_0_IRQHandler        ; 16+15: Timer0
                DCD     TIMER_1_IRQHandler        ; 16+16: Timer1
                DCD     TIMER_2_IRQHandler        ; 16+17: Timer2
                DCD     TIMER_3_IRQHandler        ; 16+18: Timer3
                DCD     TBS_IRQHandler            ; 16+19: TBS
                DCD     RTC_IRQHandler            ; 16+20: RTC
                DCD     I2C_IRQHandler            ; 16+21: I2C
                DCD     SPI_IRQHandler            ; 16+22: SPI
                DCD     Reserved_IRQHandler       ; 16+23: Reserved
                DCD     SelfTestFreq_IRQHandler   ; 16+24: SelfTestFreq
                DCD     EMU_IRQHandler            ; 16+25: EMU
                DCD     DMA_IRQHandler            ; 16+26: DMA
                DCD     KEY_IRQHandler            ; 16+27: KEY
                DCD     Reserved_IRQHandler       ; 16+28: Reserved
                DCD     Reserved_IRQHandler       ; 16+29: Reserved
                DCD     Reserved_IRQHandler       ; 16+30: Reserved
                DCD     Reserved_IRQHandler       ; 16+31: Reserved


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B SysTick_Handler
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        PUBWEAK PMU_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PMU_IRQHandler
        B PMU_IRQHandler

        PUBWEAK AES_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
AES_IRQHandler
        B AES_IRQHandler

        PUBWEAK EXTI0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI0_IRQHandler
        B EXTI0_IRQHandler

        PUBWEAK EXTI1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI1_IRQHandler
        B EXTI1_IRQHandler

        PUBWEAK EXTI2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI2_IRQHandler
        B EXTI2_IRQHandler

        PUBWEAK EXTI3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI3_IRQHandler
        B EXTI3_IRQHandler

        PUBWEAK EXTI4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI4_IRQHandler
        B EXTI4_IRQHandler

        PUBWEAK EXTI5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI5_IRQHandler
        B EXTI5_IRQHandler

        PUBWEAK EXTI6_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI6_IRQHandler
        B EXTI6_IRQHandler

        PUBWEAK UART0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART0_IRQHandler
        B UART0_IRQHandler

        PUBWEAK UART1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART1_IRQHandler
        B UART1_IRQHandler

        PUBWEAK UART2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART2_IRQHandler
        B UART2_IRQHandler

        PUBWEAK UART3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART3_IRQHandler
        B UART3_IRQHandler

        PUBWEAK UART4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART4_IRQHandler
        B UART4_IRQHandler

        PUBWEAK UART5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART5_IRQHandler
        B UART5_IRQHandler

        PUBWEAK TIMER_0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER_0_IRQHandler
        B TIMER_0_IRQHandler

        PUBWEAK TIMER_1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER_1_IRQHandler
        B TIMER_1_IRQHandler

        PUBWEAK TIMER_2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER_2_IRQHandler
        B TIMER_2_IRQHandler

        PUBWEAK TIMER_3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER_3_IRQHandler
        B TIMER_3_IRQHandler

        PUBWEAK TBS_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TBS_IRQHandler
        B TBS_IRQHandler

        PUBWEAK RTC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RTC_IRQHandler
        B RTC_IRQHandler

        PUBWEAK I2C_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C_IRQHandler
        B I2C_IRQHandler

        PUBWEAK SPI_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI_IRQHandler
        B SPI_IRQHandler

        PUBWEAK Reserved_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
Reserved_IRQHandler
        B Reserved_IRQHandler

        PUBWEAK SelfTestFreq_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SelfTestFreq_IRQHandler
        B SelfTestFreq_IRQHandler

        PUBWEAK EMU_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EMU_IRQHandler
        B EMU_IRQHandler

        PUBWEAK DMA_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA_IRQHandler
        B DMA_IRQHandler

        PUBWEAK KEY_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
KEY_IRQHandler
        B KEY_IRQHandler

        END
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
