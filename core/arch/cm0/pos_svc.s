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
 *  @file    pos_svc.s
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief SVC Handler
 *
 */

        public SVC_Handler
        extern pos_os_sys_call
        section CORE:CODE(2)
        thumb
        
SVC_Handler:
        MOVS r0, #4
        MOV r1, LR
        TST r0, r1
        BEQ main_stack_mode
        MRS r1, PSP
        ADDS r1,r1,#24
        LDR  r0,[r1]
        MOVS    r1,#3
        MVNS    r1,r1
        ANDS r0,r0,r1
        LDR  r0,[r0] ;1st parameter - SVC number
        MRS r1, PSP ; 2nd parameter - arg pointer
        LDR r2,=pos_os_sys_call
        BX r2
main_stack_mode:
        MRS r1, MSP
        ADDS r1,r1,#24
        LDR  r0,[r1]
        MOVS    r1,#3
        MVNS    r1,r1
        ANDS r0,r0,r1
        LDR  r0,[r0] ;1st parameter - SVC number
        MRS r1, MSP ; 2nd parameter - arg pointer
        LDR r2,=pos_os_sys_call
        BX r2
        END
