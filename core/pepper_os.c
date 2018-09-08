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
 *  @file    pepper_os.c
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief pepper os main file
 *
 */

#include "pepper_os.h"
#include _DEV_HAL_HEADER
#include _DEV_INIT_HEADER

static uint32_t last_error_no;

void pos_init(void)
{
  DEV_INIT();
  pos_arch_init();
  init_pos_memory();
  pos_init_timers();
  led_init();
  pos_ipc_init();
#if MAX_MUTEX_NUM > 0
  pos_mutex_init();
#endif
  pos_scheduler_init();
  pos_delay_init();
#if USE_CONSOLE == TRUE
  pos_console_init();
#endif
}

void pos_delay_ms(uint32_t time){
  POS_BEGIN_KCRITICAL;
  pos_delay_add(pos_get_current_task()->pid,time,POS_TASK_STATUS_DELAY);
  pos_force_cs();
  POS_END_KCRITICAL;
  pos_yield_delay();
}


PosStatusType pos_create_task(task_start_handler_t main_fn,task_msg_proc_t process_fn,uint8_t * stack_addr,size_t stack_size,PosTaskPriority priority){
  uint8_t * _stack_addr;
  _PID pid;
  if(stack_addr != 0)
    _stack_addr = stack_addr;
  else
    _stack_addr = (uint8_t *)pmalloc(stack_size);
  
  if(_stack_addr==NULL)
    return POS_ERROR;
  pos_schedule_init_task(main_fn,process_fn,_stack_addr,stack_size,priority,&pid);
  pos_send_message(pid,POS_TASK_STARTUP,pid); // Send Startup Message
  return POS_OK;
}

void pos_error(uint32_t error_no){
  last_error_no = error_no;
}

uint32_t pos_get_last_error(void){
  return last_error_no;
}


void pos_os_sys_call(PosSysCallType num,uint32_t * args){
  pos_set_current_mode(POS_KERNEL_ACTIVE);
  switch(num){
  case POS_SYS_CALL_SLEEP_CPU:
    pos_sleep_cpu();
    break;

  }
  pos_set_current_mode(POS_TASKS_ACTIVE);
}