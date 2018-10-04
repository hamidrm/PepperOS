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
 *  @file    pos_delay.c
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief pepper os delay management
 *
 */


// Header Files
#include <stdint.h>
#include "pepper_os.h"
#include "pos_memory.h"
#include "pos_ipc.h"
#include "pos_scheduler.h"
#include "pos_semaphore.h"
#include "pos_delay.h"
#include _DEV_HAL_HEADER
#include _ARCH_HEADER



static _pos_delay_t * pos_delay_head;
extern uint8_t __is_sleep_mode;


PosStatusType pos_delay_init(void){
  pos_delay_head = 0;
  return POS_OK;
}


PosStatusType pos_delay_add(pos_pid_type pid,uint32_t time_ms,PosTaskStatus status){
  _pos_delay_t * curr_element = pos_delay_head;
  _pos_delay_t * prev_element = NULL;
  _pos_delay_t * new_delay = (_pos_delay_t *)pmalloc(sizeof(_pos_delay_t));
  
  POS_ASSERT(new_delay != NULL_PTR);
  POS_ASSERT(pos_get_task_by_pid(pid) != NULL_PTR);
  
  new_delay->elapsed_time = time_ms;
  new_delay->pid = pid;
  
  while((curr_element!= NULL) && (curr_element->elapsed_time < time_ms)){
    prev_element = curr_element;
    curr_element = curr_element->next;
  }
  if(prev_element)
    prev_element->next = new_delay;
  else
    pos_delay_head = new_delay;
  pos_get_task_by_pid(pid)->status = status;
  new_delay->next = curr_element;
  return POS_OK;
}


PosStatusType pos_delay_remove(pos_pid_type pid){
  _pos_delay_t * curr_element = pos_delay_head;
  _pos_delay_t * prev_element = NULL;

  POS_ASSERT(pos_get_task_by_pid(pid) != NULL_PTR);
  
  while(curr_element!= NULL){
    if(curr_element->pid == pid){
      if(prev_element)
        prev_element->next = curr_element->next;
      else
        pos_delay_head = curr_element->next;
      pfree(curr_element);
      return POS_OK;
    }
    prev_element = curr_element;
    curr_element = curr_element->next;
  }
  pos_error(POS_OS_RESOURCE_NOT_FOUND);
  return POS_ERROR;
}

/*
  Note : In this version , just one task with High Priority can Yield on exact delay time.
*/
void pos_delay_tick(void){
  _pos_delay_t * next_element = pos_delay_head;
  while(next_element){
    next_element->elapsed_time--;
    next_element = next_element->next;
  }
  
   if((pos_delay_head != NULL) && (pos_delay_head->elapsed_time <= 0)){
    /* Check task status */
    if(pos_get_task_by_pid(pos_delay_head->pid)->status == POS_TASK_STATUS_DELAY){
      if(pos_get_task_by_pid(pos_delay_head->pid)->priority == POS_TASK_HIGH_PRIORITY && __is_sleep_mode == 0)
        pos_force_cs_by_pid(pos_delay_head->pid);
      else
        pos_get_task_by_pid(pos_delay_head->pid)->status = (pos_delay_head->pid == _pos_get_current_pid()) ? POS_TASK_STATUS_ACTIVE : POS_TASK_STATUS_IDLE;
    }else if(pos_get_task_by_pid(pos_delay_head->pid)->status == POS_TASK_STATUS_SEM_WAITING_UNTIL){
      pos_get_task_by_pid(pos_delay_head->pid)->status = POS_TASK_STATUS_DELAY_TIMEOUT;
      if(pos_get_task_by_pid(pos_delay_head->pid)->priority == POS_TASK_HIGH_PRIORITY && __is_sleep_mode == 0)
        pos_force_cs_by_pid(pos_delay_head->pid);
    }else {
      /* WTF Here?! */
      pos_error(POS_OS_UNEXPECTED_ERROR);
    }
    /* Release elapsed delay element */
    next_element = pos_delay_head->next;
    pfree(pos_delay_head);
    pos_delay_head = next_element;
  }
}