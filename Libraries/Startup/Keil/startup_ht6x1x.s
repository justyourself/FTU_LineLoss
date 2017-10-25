;/*
;*********************************************************************************************************
;*                                             HT6X1X
;*                                         Library Function
;*
;*                                  Copyright 2013, Hi-Trend Tech, Corp.
;*                                       All Rights Reserved
;* 
;*
;* Project      : HT6xxx
;* File         : startup_ht6x1x.s
;* By           : SocTeam
;* Version      : CurrentVersion1_5_20160407
;* Description  : 
;*********************************************************************************************************
;*/

; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00001000

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     PMU_IRQHandler            ; 16+ 0: PMU
                DCD     TDES_IRQHandler           ; 16+ 1: 3DES
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
                DCD     SPI0_IRQHandler           ; 16+22: SPI0
                DCD     WDT_IRQHandler            ; 16+23: WDT
                DCD     Reserved_IRQHandler       ; 16+24: Reserved
                DCD     Reserved_IRQHandler       ; 16+25: Reserved
                DCD     Reserved_IRQHandler       ; 16+26: Reserved
                DCD     Reserved_IRQHandler       ; 16+27: Reserved
                DCD     Reserved_IRQHandler       ; 16+28: Reserved
                DCD     Reserved_IRQHandler       ; 16+29: Reserved
                DCD     Reserved_IRQHandler       ; 16+30: Reserved
                DCD     Reserved_IRQHandler       ; 16+31: Reserved


                IF      :LNOT::DEF:NO_CRP
                AREA    |.ARM.__at_0x0FC0|, CODE, READONLY
CRP_Key         DCD     0xFFFFFF09
                ENDIF


                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

; now, under COMMON NMI.c and NMI.h, a real NMI handler is created if NMI is enabled 
; for particular peripheral.
NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  NMI_Handler               [WEAK]
                EXPORT  PMU_IRQHandler            [WEAK]
                EXPORT  TDES_IRQHandler           [WEAK]
                EXPORT  EXTI0_IRQHandler          [WEAK]
                EXPORT  EXTI1_IRQHandler          [WEAK]
                EXPORT  EXTI2_IRQHandler          [WEAK]
                EXPORT  EXTI3_IRQHandler          [WEAK]
                EXPORT  EXTI4_IRQHandler          [WEAK]
                EXPORT  EXTI5_IRQHandler          [WEAK]
                EXPORT  EXTI6_IRQHandler          [WEAK]

                EXPORT  UART0_IRQHandler          [WEAK]
                EXPORT  UART1_IRQHandler          [WEAK]
                EXPORT  UART2_IRQHandler          [WEAK]
                EXPORT  UART3_IRQHandler          [WEAK]
                EXPORT  UART4_IRQHandler          [WEAK]
                EXPORT  UART5_IRQHandler          [WEAK]
                EXPORT  TIMER_0_IRQHandler        [WEAK]
                EXPORT  TIMER_1_IRQHandler        [WEAK]
                EXPORT  TIMER_2_IRQHandler        [WEAK]
                EXPORT  TIMER_3_IRQHandler        [WEAK]
                EXPORT  TBS_IRQHandler            [WEAK]
                EXPORT  RTC_IRQHandler            [WEAK]
                EXPORT  I2C_IRQHandler            [WEAK]
                EXPORT  SPI0_IRQHandler           [WEAK]
                EXPORT  WDT_IRQHandler            [WEAK]
                EXPORT  Reserved_IRQHandler       [WEAK]


PMU_IRQHandler
TDES_IRQHandler
EXTI0_IRQHandler
EXTI1_IRQHandler
EXTI2_IRQHandler
EXTI3_IRQHandler
EXTI4_IRQHandler
EXTI5_IRQHandler
EXTI6_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
UART3_IRQHandler
UART4_IRQHandler
UART5_IRQHandler
TIMER_0_IRQHandler
TIMER_1_IRQHandler
TIMER_2_IRQHandler
TIMER_3_IRQHandler
TBS_IRQHandler
RTC_IRQHandler
I2C_IRQHandler
SPI0_IRQHandler
WDT_IRQHandler
Reserved_IRQHandler
                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB
                
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
