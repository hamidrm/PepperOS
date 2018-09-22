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
 *  @file    pos_cm0.h
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief low-level Cortex-M0 header
 *
 */

#ifndef _POS_CS_H_
#define _POS_CS_H_


extern int32_t __primask_value_cnt;

#define _SCV(num) asm volatile ("svc %[svc_num]" : : [svc_num]"I"(num));


#define POS_BEGIN_KCRITICAL do{\
                              if(__primask_value_cnt==0)\
                                asm volatile (" CPSID i\t\n");\
                              asm volatile (" ADDS   %0,#1\t\n" : "+r" (__primask_value_cnt) : );\
                              if(__primask_value_cnt < 0)\
                                pos_error(POS_OS_UNEXPECTED_ERROR);\
                             }while(0)

#define POS_END_KCRITICAL do{\
                              asm volatile (" SUBS   %0,#1\t\n" : "+r" (__primask_value_cnt) : );\
                              if(__primask_value_cnt==0)\
                                asm volatile (" CPSIE i\t\n");\
                              if(__primask_value_cnt < 0)\
                                pos_error(POS_OS_UNEXPECTED_ERROR);\
                             }while(0)

#define pos_os_call_prototype(num) void pos_os_call_##num(uint32_t arg0,uint32_t arg1,uint32_t arg2,uint32_t arg3);

#define pos_os_call_def(num) void pos_os_call_##num (uint32_t arg0,uint32_t arg1,uint32_t arg2,uint32_t arg3){\
_SCV(num);\
  }
#define pos_os_call(num,arg0,arg1,arg2,arg3) pos_os_call_##num(arg0,arg1,arg2,arg3)

typedef enum {
  CM0_STACKING_OFFSET_R8,
  CM0_STACKING_OFFSET_R9,
  CM0_STACKING_OFFSET_R10,
  CM0_STACKING_OFFSET_R11,
  CM0_STACKING_OFFSET_R4,
  CM0_STACKING_OFFSET_R5,
  CM0_STACKING_OFFSET_R6,
  CM0_STACKING_OFFSET_R7,  
  CM0_STACKING_OFFSET_R0,
  CM0_STACKING_OFFSET_R1,
  CM0_STACKING_OFFSET_R2,
  CM0_STACKING_OFFSET_R3,
  CM0_STACKING_OFFSET_R12,
  CM0_STACKING_OFFSET_LR,
  CM0_STACKING_OFFSET_PC,
  CM0_STACKING_OFFSET_XPSR,
} Cm0StackingOffset;
  
void SVC_Handler(void);
  
void SysTick_Handler(void);
void pos_arch_init(void);
void pos_init_stack(pos_pid_type pid,uint8_t * stack,size_t stack_size,task_start_handler_t start_handler);
uint32_t pos_get_stacking_size(void);

void pos_goto_unprivilleged_mode(uint32_t * stack);
extern inline void pos_reset_task_timer(void);
extern inline void pos_yield_delay(void);

/* OS System Call prototypes */

pos_os_call_prototype(POS_SYS_CALL_NOP);
pos_os_call_prototype(POS_SYS_CALL_SLEEP_CPU);






void _POS_BEGIN_KCRITICAL();
void _POS_END_KCRITICAL();
#endif