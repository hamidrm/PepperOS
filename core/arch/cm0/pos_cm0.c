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
 *  @file    pos_cm0.c
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief low-level Cortex-M0 methods
 *
 */

#include "pos_devices.h"

#if defined(CM0)
#include "pepper_os.h"

#include "inc/pos_cm0.h"
#include _DEV_HAL_HEADER_ARCH
#include _DEV_DEFS_HEADER_ARCH
#include _DEV_HAL_HEADER_ARCH

static uint8_t elapsed_ticks=0;

extern pos_elapsed_time_t elapsed_times;
extern PosOsMode os_mode;

extern uint8_t __quantum_cur_len;
extern pos_scheduler_t * current_task_element;
extern uint8_t __mt;
int32_t __primask_value_cnt;


/* POS Call Services 
  {
*/

pos_os_call_def(POS_SYS_CALL_NOP);
pos_os_call_def(POS_SYS_CALL_SLEEP_CPU);
/*
  }
*/


void SysTick_Handler(void)
{
  sys_tick_isr();
  
  if(os_mode == POS_SYSTEM_SLEEP)
    elapsed_times.sleep_elapsed_time++;

  if(os_mode == POS_KERNEL_ACTIVE)
    elapsed_times.kernel_elapsed_time++;
    
  elapsed_times.total_elapsed_time++;
  if(__mt){
    elapsed_ticks++;
    if(current_task_element && os_mode == POS_TASKS_ACTIVE)
      current_task_element->task.time_elapsed_active++;
    
    if(elapsed_ticks == __quantum_cur_len){
      elapsed_ticks=0;
      pos_schedule_tasks();
    }else
      pos_delay_tick();
  }
}


inline void pos_reset_task_timer(void){
  elapsed_ticks = 0;
}


void pos_init_stack(pos_pid_type pid,uint8_t * stack,size_t stack_size,task_start_handler_t start_handler){
  uint32_t * stack_pointer = (uint32_t *)((uint32_t)stack+stack_size-64);
  /* Used frame stack on finishing task */
  stack_pointer[CM0_STACKING_OFFSET_R0] = pid;
  stack_pointer[CM0_STACKING_OFFSET_R1] = 0x00fe1fe1L; /* FelFel */
/* Other registers during calling task
  stack_pointer[CM0_STACKING_OFFSET_R2] = 0L;
  stack_pointer[CM0_STACKING_OFFSET_R3] = 0L;
  stack_pointer[CM0_STACKING_OFFSET_R4] = 0L;
  stack_pointer[CM0_STACKING_OFFSET_R5] = 0L;
  stack_pointer[CM0_STACKING_OFFSET_R6] = 0L;
  stack_pointer[CM0_STACKING_OFFSET_R7] = 0L;
  stack_pointer[CM0_STACKING_OFFSET_R8] = 0L;
  stack_pointer[CM0_STACKING_OFFSET_R9] = 0L;
  stack_pointer[CM0_STACKING_OFFSET_R10] = 0L;
  stack_pointer[CM0_STACKING_OFFSET_R11] = 0L;
  stack_pointer[CM0_STACKING_OFFSET_R12] = 0L;
*/
  stack_pointer[CM0_STACKING_OFFSET_LR] = (uint32_t)&pos_task_finish;
  stack_pointer[CM0_STACKING_OFFSET_PC] = (uint32_t)start_handler;
  stack_pointer[CM0_STACKING_OFFSET_XPSR] = 0x01000000; /* Set T flag (thumb state) */
}

inline void pos_yield_delay(void){
   __asm("PUSH {R7}\t\n"
          "MOVS    R7,#10\t\n"
          "b1:    SUBS    R7,R7,#1\t\n"
          "BEQ    b1\t\n"
          "POP     {R7}\t\n");
}

void pos_arch_init(void){
    __primask_value_cnt = 0;
}

void pos_goto_unprivilleged_mode(uint32_t * stack){
  __set_PSP((uint32_t)stack); /* Set PSP */
  __set_CONTROL(0x03); /* Switch to PSP, unprivilleged mode */
  __ISB(); /* Wait for flusheing pipelines */

}






#endif