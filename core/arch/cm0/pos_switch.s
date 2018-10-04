/*******************************************************************************
MIT License

Copyright (c) 2018 Hamid Reza Mehrabian

This file is part of PepperOS. 


Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
********************************************************************************/


/**
 *  @file    pos_switch.s
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief Low level context switch Routine (on PendSV ISR)
 *
 */



#include "pos_devices.h"


#if defined(CM0)
        public PendSV_Handler
        extern __current_task
        extern __next_task
        
        
        extern pos_kill_task
        extern pos_error
        extern os_mode;
        section .text:CODE(2)
        thumb

PendSV_Handler:


        ; Disable interrupts
        cpsid   i
        
        ; Stack frame layput
        ; Stacking and Unstacking for current and next tasks registers will be done in 2 Step

        /*
        1. will be done by hardware :
        --------
        |<prev>| <- PSP befor PendSV
        --------
        | xPSR |  +28
        --------
        |  PC  |  +24
        --------
        |  LR  |  +20
        --------
        |  R12 |  +16
        --------
        |  R3  |  +12
        --------
        |  R2  |  +8
        --------
        |  R1  |  +4
        --------
        |  R0  |  <- Current PSP
        --------

        2. will be done by software :
         --------
        |  R7   |  -4
        --------
        |  R6   |  -8
        --------
        |  R5   |  -12
        --------
        |  R4   |  -16
        --------
        |  R8   |  -20
        --------
        |  R9   |  -24
        --------
        |  R10   |  -28
        --------
        |  R11   |  <- After complete storing
        --------
        
        */
        
        mrs	r0, psp
#if CHECK_STACK_OVER_FLOW == 1
        ldr     r3,=__current_task
        ldr     r2,[r3]
        adds    r2,r2,#8        ;Original SP offset
        ldr     r1,[r2]
        mov     r2,r0
        subs    r2,r2,#32
        cmp     r2,r1
        bge     valid_stack
        ;StackOverflow occured!
        ;But if one of the tasks stack that is in the first allocated block , Hardault will be occur and code never comes here.
        ldr     r3,[r3]     
        ldrh    r0,[r3] ;Current task PID
        ldr     r3,=pos_kill_task
        blx      r3
        movs    r0,#6   ;Stackoverflow Error
        movs    r1,#1
        lsls    r1,r1,#31
        orrs    r0,r0,r1
        ldr     r3,=pos_error
        blx      r3
        /* EXC_RETURN - Thread mode with PSP */
	ldr r0, =0xFFFFFFFD

	;Enable interrupts
	cpsie	i

        ; Continue next task
	bx	r0
        
valid_stack:
#endif
        subs    r0,r0,#28
        mov     r1,r10
        mov     r2,r9
        mov     r3,r8
        stmia   r0!,{r1-r7}
        subs    r0,r0,#32
        mov     r1,r11
        str     r1,[r0]

        ; Store PSP for current task
        ldr     r1,=__current_task
        ldr     r2,[r1]
        adds    r2,r2,#4 ;SP offset
        str     r0,[r2]

        ; Load PSP for next task
        ldr     r1,=__next_task
        ldr     r2,[r1]
        adds    r2,r2,#4 ;SP offset
        ldr     r0,[r2]
        

        ; Load all registers for next task
        ldmia	r0!,{r4-r7}
	mov	r8, r4
	mov	r9, r5
	mov	r10, r6
	mov	r11, r7
	ldmia	r0!,{r4-r7}
	msr	psp, r0


        ldr     r1,=os_mode
        movs    r0,#1
        strb     r0,[r1]
	
        /* EXC_RETURN - Thread mode with PSP */
	ldr r0, =0xFFFFFFFD

	;Enable interrupts
	cpsie	i

        ; Continue next task
	bx	r0

  
  
 END 
#else
  SECTION .text:CODE:NOROOT
  END
#endif