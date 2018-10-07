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
 *  @file    pos_semaphore.c
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief pepper os semaphore management
 *
 */


// Header Files
#include <stdint.h>
#include "pepper_os.h"
#include "pos_memory.h"
#include "pos_scheduler.h"
#include "pos_ipc.h"
#include _DEV_HAL_HEADER
#include "pos_semaphore.h"
#include _ARCH_HEADER

static PosStatusType pos_semaphore_enq(pos_semaphore_t *s,pos_pid_type pid);
static PosStatusType pos_semaphore_deq(pos_semaphore_t *s,pos_pid_type * pid);
extern uint8_t __is_sleep_mode;

PosStatusType pos_semaphore_init(pos_semaphore_t *s,int16_t iv){
	s->cnt = iv;
	s->q=NULL;
        s->released_task = 0;
        return POS_OK;
}

PosStatusType pos_semaphore_wait(pos_semaphore_t *s){

        POS_ASSERT(s != NULL_PTR);
	POS_BEGIN_KCRITICAL;
        s->cnt-=1;	
	if(s->cnt>=0){
            POS_END_KCRITICAL;
            return POS_OK;
	}
	POS_ASSERT(pos_semaphore_enq(s,_pos_get_current_pid()) == POS_OK);
        pos_get_current_task()->status = POS_TASK_STATUS_SEM_WAITING;
	
        pos_yield();
        POS_END_KCRITICAL;
        pos_yield_delay();
        return POS_OK;
}

PosStatusType pos_semaphore_wait_until(pos_semaphore_t *s,uint32_t time_ms){
        pos_pid_type pid = _pos_get_current_pid();
        POS_ASSERT_SEND_ERROR(s != NULL_PTR,POS_MEM_UNALLOCATED);
        
	POS_BEGIN_KCRITICAL;
        s->cnt-=1;	
	if(s->cnt>=0){
            POS_END_KCRITICAL;
            return POS_OK;
	}
	POS_ASSERT(pos_semaphore_enq(s,_pos_get_current_pid()) == POS_OK);

	pos_delay_add(pos_get_current_task()->pid,time_ms,POS_TASK_STATUS_SEM_WAITING_UNTIL);
        /* 
        while(pos_semaphore_get_value(s)!=pid); //Blocking in single threading , Goto next thread otherwise.
        */

        pos_yield();
        POS_END_KCRITICAL;
        pos_yield_delay();
        
        if(pos_get_current_task()->status == POS_TASK_STATUS_DELAY_TIMEOUT){
          /* Increase semaphore */
          s->cnt+=1;
          POS_ASSERT(pos_semaphore_deq(s,NULL_PTR) == POS_OK);
          pos_get_current_task()->status = POS_TASK_STATUS_ACTIVE;
          return POS_TIMEOUT;
        }else{
          pos_delay_remove(pid);
          return POS_OK;
        }
}

PosStatusType pos_semaphore_signal(pos_semaphore_t *s,pos_pid_type * pid){
        pos_pid_type _pid;
        POS_ASSERT_SEND_ERROR(s != NULL_PTR,POS_MEM_UNALLOCATED);
        
        POS_BEGIN_KCRITICAL;
	s->cnt+=1;
	POS_ASSERT(pos_semaphore_deq(s,&_pid) == POS_OK);
        
        
        if(_pid != POS_KERNEL_PID &&  pos_get_task_by_pid(_pid)->priority == POS_TASK_HIGH_PRIORITY && __is_sleep_mode == 0)
          pos_yield_by_pid(_pid);
        
        POS_END_KCRITICAL;
        pos_yield_delay();
        if(pid != NULL_PTR)
          *pid = _pid;
        return POS_OK;
}

static PosStatusType pos_semaphore_enq(pos_semaphore_t *s,pos_pid_type pid){
	pos_semaphore_queue_t * q = s->q;
	pos_semaphore_queue_t *temp_element;
        POS_ASSERT_SEND_ERROR(s != NULL_PTR,POS_MEM_UNALLOCATED);
        POS_ASSERT(pos_get_task_by_pid(pid) != NULL_PTR);
	if(q==NULL){
		s->q = (pos_semaphore_queue_t *)pmalloc(sizeof(pos_semaphore_queue_t));
                POS_ASSERT(s->q != NULL_PTR);
		s->q->pid = pid;
		s->q->next = NULL;
		return POS_OK;
	}
	while(q){
		temp_element = (pos_semaphore_queue_t *)(q);
		q=(pos_semaphore_queue_t *)(temp_element->next);
	}
	temp_element->next = (pos_semaphore_queue_t *)pmalloc(sizeof(pos_semaphore_queue_t));
        POS_ASSERT(temp_element->next != NULL_PTR);
	temp_element = (pos_semaphore_queue_t *)(temp_element->next);
	temp_element->next = NULL;
	temp_element->pid = pid;
        return POS_OK;
}


static PosStatusType pos_semaphore_deq(pos_semaphore_t *s,pos_pid_type * pid){
	pos_pid_type res;
	pos_semaphore_queue_t * q = s->q;
	pos_semaphore_queue_t *temp_element;
        
        POS_ASSERT_SEND_ERROR(s != NULL_PTR,POS_MEM_UNALLOCATED);
        if(q == NULL_PTR){
          *pid = NULL_PTR;
          return POS_OK;
        }
        
	res = q->pid;
        if(s->cnt <= 0){
          temp_element = (pos_semaphore_queue_t *)(q->next);
          pfree(q);
          s->q = temp_element;
        }
        pos_get_task_by_pid(res)->status = POS_TASK_STATUS_IDLE;
        if(pid)
          *pid = res;
        return POS_OK;
}