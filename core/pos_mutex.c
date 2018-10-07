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
 *  @file    pos_mutex.c
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief pepper os mutex management
 *
 */

// Header Files
#include <stdint.h>
#include "pepper_os.h"
#include "pos_memory.h"
#include "pos_scheduler.h"
#include "pos_ipc.h"
#include _DEV_HAL_HEADER
#include "pos_mutex.h"
#include _ARCH_HEADER

#if MAX_MUTEX_NUM > 0
static volatile uint32_t __mutex_locks[( MAX_MUTEX_NUM - 1 ) / 32 + 1];
static uint32_t __mutex_locks_map[( MAX_MUTEX_NUM - 1 ) / 32 + 1];
static pos_pid_type     __mutex_locks_pid[MAX_MUTEX_NUM][MAX_PROCCESS_NUM-1];
static uint16_t __mutex_locked_pid_cnt[MAX_MUTEX_NUM];

void pos_mutex_init(void){
  uint32_t i=0,j;
  while(i<(( MAX_MUTEX_NUM - 1 ) / 32 + 1)){
    __mutex_locks[i] = 0; /* Unlock all mutexes */
    __mutex_locks_map[i] = 0; /* Free all mutexes */
    i++;
  }

  for(i=0;i<MAX_MUTEX_NUM;i++){
    for(j=0;j<MAX_PROCCESS_NUM;j++)
      __mutex_locks_pid[i][j] = POS_KERNEL_PID; /* Prepare pid list*/
    __mutex_locked_pid_cnt[i] = 0; /*  Reset tasks number that locked*/
  }
  
}


void pos_mutex_create(pos_mutex_id_t * mutex){
  uint32_t i,j;
  POS_BEGIN_KCRITICAL;
  for(i=0;i<(( MAX_MUTEX_NUM - 1 ) / 32 + 1);i++)
    for(j=0;j<sizeof(uint32_t) * 8;j++)
      if((__mutex_locks_map[i] & (1<<j)) == 0){
        __mutex_locks_map[i] |= (1<<j);
        __mutex_locks[i] &= (0xFFFFFFFF ^ (1<<j));
        POS_END_KCRITICAL;
        *mutex = (i * sizeof(uint32_t) * 8 + j);
        __mutex_locked_pid_cnt[*mutex] = 0;
        return;
      }
  
  POS_END_KCRITICAL;
  *mutex = POS_MUTEX_INVALID;
}

void pos_mutex_free(pos_mutex_id_t * mutex){
  uint32_t x = *mutex / 32;
  uint32_t y = *mutex % 32;
  POS_BEGIN_KCRITICAL;
  if((__mutex_locks_map[x] & (1<<y)) == 0){
    pos_error(POS_MEM_UNALLOCATED);
    POS_END_KCRITICAL;
    return;
  }
  __mutex_locks_map[x] &= (0xFFFFFFFF ^ (1<<y));
  POS_END_KCRITICAL;
}

void pos_mutex_acquire(pos_mutex_id_t * mutex){
  uint32_t x = *mutex / 32;
  uint32_t y = *mutex % 32;
  POS_BEGIN_KCRITICAL;
  if((__mutex_locks_map[x] & (1<<y)) == 0){
    pos_error(POS_MEM_UNALLOCATED);
    POS_END_KCRITICAL;
    return;
  }
  if(__mutex_locks[x] & (1<<y)){
    __mutex_locks_pid[*mutex][__mutex_locked_pid_cnt[*mutex]] = pos_get_current_task()->pid;
    pos_get_current_task()->status = POS_TASK_STATUS_SEM_WAITING;
    __mutex_locked_pid_cnt[*mutex]++;
    pos_yield();
    POS_END_KCRITICAL;
    pos_yield_delay();
  }else{
    __mutex_locks[x] |= (1<<y);
    POS_END_KCRITICAL;
  }

  
}


void pos_mutex_release(pos_mutex_id_t * mutex){
  uint32_t x = *mutex / 32;
  uint32_t y = *mutex % 32;
  POS_BEGIN_KCRITICAL;
  if((__mutex_locks_map[x] & (1<<y)) == 0){
    pos_error(POS_MEM_UNALLOCATED);
    POS_END_KCRITICAL;
    return;
  }
  __mutex_locks[x] &= (0xFFFFFFFF ^ (1<<y));
  if(__mutex_locked_pid_cnt[*mutex] > 0 ){
    __mutex_locked_pid_cnt[*mutex]--;
    pos_get_task_by_pid(__mutex_locks_pid[*mutex][__mutex_locked_pid_cnt[*mutex]])->status = POS_TASK_STATUS_IDLE;
  }
  POS_END_KCRITICAL;
}

#endif