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
 *  @file    pos_ipc.c
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief pepper os pepper os inter-process communication
 *
 */

// Header Files
#include <stdint.h>
#include "pepper_os.h"
#include "pos_memory.h"
#include "pos_ipc.h"
#include "pos_scheduler.h"
#include "pos_semaphore.h"
#include _DEV_HAL_HEADER
#include _ARCH_HEADER
PosStatusType pos_ipc_enq(pos_pid_type pid,pos_process_message_t * m);
PosQueueSTatus pos_ipc_deq(pos_pid_type pid,pos_process_message_t * res);
static pos_process_message_queue_t * volatile __mq[MAX_PROCCESS_NUM]={NULL};
static pos_semaphore_t               __mqs[MAX_PROCCESS_NUM];


PosStatusType pos_ipc_init(void){
  uint16_t i=0;
  while(i<MAX_PROCCESS_NUM){
    __mq[i] = NULL_PTR;
    pos_semaphore_init(&__mqs[i],0);
    i++;
  }
  return POS_OK;
}


PosStatusType pos_send_message(pos_pid_type target,	pos_process_message_type message_type,pos_process_message_content message_content){
	pos_process_message_t e;
        PosStatusType status;
        //if(pos_get_current_task()->proc_fun == NULL)
        //  return POS_OK;
        POS_BEGIN_KCRITICAL;
#if     MAX_IPC_MESSAGES_NUM > 0
        uint32_t messages_cnt=0;
        pos_process_message_queue_t *q = __mq[target-1];
#endif
        POS_ASSERT(pos_get_task_by_pid(target) != NULL_PTR);
	e.src = _pos_get_current_pid();
	e.message_type = message_type;
	e.message_content = message_content;
        /* Restric Message Queue */
#if     MAX_IPC_MESSAGES_NUM > 0
        while(q){
          q=(pos_process_message_queue_t *)(q->next);
          messages_cnt++;
        }
        if(MAX_IPC_MESSAGES_NUM <= messages_cnt){
          pos_error(POS_MEM_LOW_SPACE);
          POS_END_KCRITICAL;
          return POS_ERROR;
        }
#endif
	pos_ipc_enq(target,&e);
        POS_END_KCRITICAL;
        status = pos_semaphore_signal(&__mqs[target-1],NULL_PTR);
        
        return status;
}

extern uint32_t *   ts1;
extern uint32_t *  ts2;

PosStatusType pos_get_message(void){
        pos_process_message_t pmsg;
 
        pos_pid_type pid = _pos_get_current_pid();
        POS_ASSERT(pos_get_current_task()->proc_fun != NULL_PTR);
        POS_ASSERT(pos_semaphore_wait(&__mqs[pid-1]) == POS_OK);
	POS_ASSERT_SEND_ERROR(pos_ipc_deq(pid,&pmsg) == POS_QUEUE_NOT_EMPTY,POS_OS_QUEUE_EMPTY);
        if(pos_get_current_task()->proc_fun != NULL)
          pos_get_current_task()->proc_fun(pmsg.message_type,pmsg.message_content,pmsg.src);
        return POS_OK;
}

PosStatusType pos_ipc_enq(pos_pid_type pid,pos_process_message_t * m){
	pos_process_message_queue_t *q = __mq[pid-1];
	pos_process_message_queue_t *temp_element;
	POS_BEGIN_KCRITICAL;	
	if(q==NULL){
          __mq[pid-1] = ( pos_process_message_queue_t *)pmalloc(sizeof(pos_process_message_queue_t));
          POS_ASSERT(__mq[pid-1] != NULL_PTR);
          __mq[pid-1]->element.src = m->src;
          __mq[pid-1]->element.message_type = m->message_type;
          __mq[pid-1]->element.message_content = m->message_content;
          __mq[pid-1]->next = NULL;
          POS_END_KCRITICAL;
          return POS_OK;
	}
	while(q){
          temp_element = (pos_process_message_queue_t *)(q);
          q=(pos_process_message_queue_t *)(temp_element->next);
	}
	temp_element->next = (pos_process_message_queue_t *)pmalloc(sizeof(pos_process_message_queue_t));
        POS_ASSERT(temp_element->next != NULL_PTR);
	temp_element = (pos_process_message_queue_t *)(temp_element->next);
	temp_element->element.src = m->src;
	temp_element->element.message_type = m->message_type;
        temp_element->element.message_content = m->message_content;
	temp_element->next = NULL;
	POS_END_KCRITICAL;
        return POS_OK;
}


PosQueueSTatus pos_ipc_deq(pos_pid_type pid,pos_process_message_t * res){
	
	pos_process_message_queue_t * q = __mq[pid-1];
	pos_process_message_queue_t *temp_element;
	POS_BEGIN_KCRITICAL;	
	if(q==NULL){
		POS_END_KCRITICAL;
		return POS_QUEUE_EMPTY;
	}
	res->src = q->element.src;
	res->message_type = q->element.message_type;
        res->message_content = q->element.message_content;
	temp_element = (pos_process_message_queue_t *)(q->next);
	pfree(q);
	__mq[pid-1] = temp_element;
	POS_END_KCRITICAL;	
	return POS_QUEUE_NOT_EMPTY;
}
