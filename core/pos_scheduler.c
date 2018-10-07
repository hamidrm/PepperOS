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
 *  @file    pos_scheduler.c
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief pepper os scheduler. with RR method. There are three priority level for each task in PepperOS :
 *      1. Low priority : has been assigned 75% of MEAN_QUANTUM_LENGTH as quantum time to this priority
 *      2. Normal priority : has been assigned 100% of MEAN_QUANTUM_LENGTH as quantum time to this priority
 *      3. High priority : has been assigned 125% of MEAN_QUANTUM_LENGTH as quantum time to this priority. In Addition , vice versa of others , 
 *        when this tasks type are on blocking mode , on signaling semaphores or releasing mutexes or elapsing delay events from other tasks , 
 *        scheduler will immediatly switch to this task. 
 *
 */

// Header Files
#include <stdint.h>
#include "pepper_os.h"
#include "pos_scheduler.h"

#include _ARCH_HEADER
#include _DEV_HAL_HEADER

uint8_t __quantum_cur_len;
uint8_t __mt;
uint8_t __is_sleep_mode;
uint16_t __tasks_cnt;

pos_task_type * __current_task;
pos_task_type * __next_task;
pos_scheduler_t * current_task_element;

pos_elapsed_time_t elapsed_times;
PosOsMode os_mode;
static pos_scheduler_t * tasks_scheduler_head;


extern inline void pos_force_goto_task(pos_pid_type pid);

void pos_scheduler_start(void){
  if(tasks_scheduler_head){
    pos_set_os_mode(POS_KERNEL_ACTIVE);
    __current_task = &tasks_scheduler_head->task;
    __current_task->status = POS_TASK_STATUS_ACTIVE;
    current_task_element = tasks_scheduler_head;
    __next_task = &tasks_scheduler_head->next_task->task;
    
    pos_goto_unprivilleged_mode((uint32_t *)((uint32_t)tasks_scheduler_head->task.sp + 32));
    /* Set quantum length according task priority */
    if(__current_task->priority == POS_TASK_HIGH_PRIORITY)
      /* High priority : use 125% of  MEAN_QUANTUM_LENGTH */
      __quantum_cur_len = (uint8_t)(MEAN_QUANTUM_LENGTH * 1.25);
    else if(__current_task->priority == POS_TASK_NORMAL_PRIORITY)
      /* Normal priority : use 100% of  MEAN_QUANTUM_LENGTH */
      __quantum_cur_len = (uint8_t)(MEAN_QUANTUM_LENGTH * 1.0);
    else
      /* Low priority : use 75% of  MEAN_QUANTUM_LENGTH */
      __quantum_cur_len = (uint8_t)(MEAN_QUANTUM_LENGTH * 0.75);
    pos_reset_task_timer();
    __mt = 1;
    pos_set_os_mode(POS_TASKS_ACTIVE);
    __current_task->start_fun(__current_task->pid);
    while(1); // TODO-Primary Task (PID=1) stoped!
  }
}


void pos_scheduler_init(void){
  tasks_scheduler_head = 0;
  __current_task = 0;
  __next_task = 0;
  current_task_element = 0;
  __mt = 0;
  __tasks_cnt = 0;
  __is_sleep_mode = 0;
  elapsed_times.total_elapsed_time = 0;
  elapsed_times.sleep_elapsed_time = 0;
  elapsed_times.kernel_elapsed_time = 0;
  
  __quantum_cur_len = MEAN_QUANTUM_LENGTH;
}

PosStatusType pos_schedule_init_task(task_start_handler_t start_handler,task_msg_proc_t proc_handler,uint8_t * stack,size_t stack_size,PosTaskPriority priority,pos_pid_type * pid){
  pos_scheduler_t * current_element = tasks_scheduler_head;
  pos_scheduler_t * new_element = (pos_scheduler_t *)pmalloc(sizeof(pos_scheduler_t));
  POS_ASSERT(new_element != NULL_PTR);
  *pid=POS_PID_BASE;
  POS_BEGIN_KCRITICAL;
  /* Find new task PID */
  while (current_element) {
    if(current_element->task.pid == *pid){
       *pid = current_element->task.pid + 1;
       current_element = current_element->next_task;
    }else break;
  }
  current_element = tasks_scheduler_head;
  
  /* Initializing new task */
  new_element->task.pid = *pid;
  new_element->task.sp = (void *)((uint32_t)stack + stack_size - __pos_get_stacking_size());
#if CHECK_STACK_OVER_FLOW == TRUE
  new_element->task.stack_size = stack_size;
  new_element->task.orginal_sp = stack;
#endif
  new_element->task.status = POS_TASK_STATUS_IDLE;
  new_element->task.priority = priority;
  new_element->task.time_elapsed_active = 0;
  new_element->task.start_fun = start_handler;
  new_element->task.proc_fun = proc_handler;
  
  
  if(!current_element){
    tasks_scheduler_head = new_element;
    new_element->next_task = new_element;
  }else{
      while (current_element) {
        if(current_element->next_task == tasks_scheduler_head){
          /* Circular buffer tail found , add new task as new tail */
          current_element->next_task = new_element;
          new_element->next_task = tasks_scheduler_head;
          break;
        }else
          current_element = current_element->next_task;
      }
  }
  
  pos_init_stack(*pid,stack,stack_size,start_handler);
  __tasks_cnt++;
  POS_END_KCRITICAL;
  return POS_OK;
}


void pos_task_finish(void){
  //Kill current task
  POS_BEGIN_KCRITICAL;
  pos_set_os_mode(POS_KERNEL_ACTIVE);
  pos_kill_task(current_task_element->task.pid);
  pos_schedule_tasks();
  pos_set_os_mode(POS_TASKS_ACTIVE);
  POS_END_KCRITICAL;
}

pos_pid_type _pos_get_current_pid(void){
  return current_task_element->task.pid;
}

void pos_get_total_elapsed_time(uint64_t *time){
  *time = elapsed_times.total_elapsed_time;
}

void pos_get_sleep_elapsed_time(uint64_t *time){
  *time = elapsed_times.sleep_elapsed_time;
}




void pos_kill_task(pos_pid_type pid){
  pos_scheduler_t * previuse_element = NULL;
  pos_scheduler_t * current_element = tasks_scheduler_head;

  while (current_element) {
    if(current_element->task.pid == pid){
      /* PID found , remove it */
      if(previuse_element)
        previuse_element->next_task = current_element->next_task;
      else{
        if( current_element->next_task->task.pid != current_element->task.pid )
          tasks_scheduler_head = current_element->next_task;
      }
      //TODO-Check task region (HEAP/STACK)
      //pfree(current_element->task.sp);
      //pfree(current_element);
      __tasks_cnt--;
      break;
    }else{
      previuse_element = current_element;
      current_element = current_element->next_task;
    }
  }
}



pos_task_type * pos_get_task_by_pid(pos_pid_type pid){
  
    pos_scheduler_t * current_element = tasks_scheduler_head;

    while (current_element) {
      if(current_element->task.pid == pid){
        return &current_element->task;
      }
      current_element = current_element->next_task;
    }
    pos_error(POS_OS_PID_NOT_FOUND);
    return NULL;
}


pos_task_type * pos_get_current_task(void){
  return &(current_task_element->task);
}

inline void pos_force_goto_task(pos_pid_type pid){
    pos_scheduler_t * current_element = tasks_scheduler_head;
    pos_set_os_mode(POS_KERNEL_ACTIVE);
    __current_task = &(current_task_element->task);
    while (current_element) {
      if(current_element->task.pid == pid){
        current_task_element = current_element;
        break;
      }
      current_element = current_element->next_task;
    }
   __next_task = &current_task_element->task;
    /* Set quantum length according task priority */
    if(__current_task->priority == POS_TASK_HIGH_PRIORITY)
      /* High priority : use 125% of  MEAN_QUANTUM_LENGTH */
      __quantum_cur_len = (uint8_t)POS_TASK_HIGH_PRIORITY_QUANTUM;
    else if(__current_task->priority == POS_TASK_NORMAL_PRIORITY)
      /* Normal priority : use 100% of  MEAN_QUANTUM_LENGTH */
      __quantum_cur_len = (uint8_t)POS_TASK_NORMAL_PRIORITY_QUANTUM;
    else
      /* Low priority : use 75% of  MEAN_QUANTUM_LENGTH */
      __quantum_cur_len = (uint8_t)POS_TASK_LOW_PRIORITY_QUANTUM;
    __next_task->status = POS_TASK_STATUS_ACTIVE;
    if(__current_task->status == POS_TASK_STATUS_ACTIVE)
      __current_task->status = POS_TASK_STATUS_IDLE;
    if(__next_task->status == POS_TASK_STATUS_IDLE)
      __next_task->status = POS_TASK_STATUS_ACTIVE;
    __pos_do_cs();
}

inline PosSchedulerStatus pos_schedule_tasks(void){
  pos_scheduler_t * executed_task_element;
  if(!__mt)
    return POS_SCHEDULER_OS_NOT_STARTED;
  POS_BEGIN_KCRITICAL;
  pos_set_os_mode(POS_KERNEL_ACTIVE);
  executed_task_element = current_task_element;
  current_task_element = current_task_element->next_task;
  
  
  while(current_task_element->task.status == POS_TASK_STATUS_SEM_WAITING \
     || current_task_element->task.status == POS_TASK_STATUS_SEM_WAITING_UNTIL \
     || current_task_element->task.status == POS_TASK_STATUS_DELAY)
  {
    if(executed_task_element->task.pid == current_task_element->task.pid){
      /* All of the tasks are blocked! Sleep CPU */
      current_task_element = executed_task_element;
      POS_END_KCRITICAL;
      return POS_SCHEDULER_NO_TASK_READY;
    }else
      current_task_element = current_task_element->next_task;
  }
  
  
  __current_task = &(executed_task_element->task);
  __next_task = &(current_task_element->task);
  
  /* Set quantum length according task priority */
  if(__current_task->priority == POS_TASK_HIGH_PRIORITY)
    /* High priority : use 125% of  MEAN_QUANTUM_LENGTH */
    __quantum_cur_len = (uint8_t)POS_TASK_HIGH_PRIORITY_QUANTUM;
  else if(__current_task->priority == POS_TASK_NORMAL_PRIORITY)
    /* Normal priority : use 100% of  MEAN_QUANTUM_LENGTH */
    __quantum_cur_len = (uint8_t)POS_TASK_NORMAL_PRIORITY_QUANTUM;
  else
    /* Low priority : use 75% of  MEAN_QUANTUM_LENGTH */
    __quantum_cur_len = (uint8_t)POS_TASK_LOW_PRIORITY_QUANTUM;
  if(__current_task->status == POS_TASK_STATUS_ACTIVE)
    __current_task->status = POS_TASK_STATUS_IDLE;
  if(__next_task->status == POS_TASK_STATUS_IDLE)
    __next_task->status = POS_TASK_STATUS_ACTIVE;
  
  
  /* Context switch - From __current_task to __next_task */

  __pos_do_cs();
  POS_END_KCRITICAL;
  return POS_SCHEDULER_OK;
}


/*
  Force context switch (according to scheduler order)
*/
void pos_yield(void){
  __is_sleep_mode = 1;
  __pos_clear_systick();
  pos_reset_task_timer();
  pos_set_os_mode(POS_SYSTEM_SLEEP);
  POS_END_KCRITICAL;
  pos_os_call(POS_SYS_CALL_SLEEP_CPU,0,0,0,0);
  pos_yield_delay();
  __is_sleep_mode = 0;
  POS_BEGIN_KCRITICAL;
}


/*
  Force context switch to the pid
*/


void pos_yield_by_pid(pos_pid_type pid){
  pos_set_os_mode(POS_KERNEL_ACTIVE);
  if(__mt){
      pos_reset_task_timer();
      pos_force_goto_task(pid);
  }
}


inline void pos_sleep_cpu(void){

  while(pos_schedule_tasks() == POS_SCHEDULER_NO_TASK_READY){
    pos_set_os_mode(POS_SYSTEM_SLEEP);
    __pos_cpu_sleep_ins();
    pos_set_os_mode(POS_KERNEL_ACTIVE);
    pos_reset_task_timer();
    if(current_task_element->task.status == POS_TASK_STATUS_ACTIVE)
      break;
  }
}


inline void pos_set_current_mode(PosOsMode mode){
  pos_set_os_mode(mode);
}