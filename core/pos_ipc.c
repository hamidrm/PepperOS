/*
* @brief POS Memory Management
* @file pos_ipc.c
* @author Hamid Reza Mehrabian
* @code
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

PosStatusType pos_ipc_enq(_PID pid,pos_process_message_t * m);
PosQueueSTatus pos_ipc_deq(_PID pid,pos_process_message_t * res);
static pos_process_message_queue_t * __mq[MAX_PROCCESS_NUM]={NULL};
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


PosStatusType pos_send_message(_PID target,	pos_process_message_type message_type,pos_process_message_content message_content){
	pos_process_message_t e;
#if     MAX_IPC_MESSAGES_NUM > 0
        uint32_t messages_cnt=0;
        pos_process_message_queue_t *q = __mq[target-1];
#endif
        POS_ASSERT(pos_get_task_by_pid(target) == NULL_PTR);
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
          return POS_ERROR;
        }
#endif
	pos_ipc_enq(target,&e);
        return pos_semaphore_signal(&__mqs[target-1],NULL_PTR);
}

PosStatusType pos_get_message(void){
        pos_process_message_t pmsg;
        _PID pid = _pos_get_current_pid();
        POS_ASSERT(pos_get_current_task()->proc_fun == NULL_PTR);
        POS_ASSERT(pos_semaphore_wait(&__mqs[pid-1]) != POS_OK);
	POS_ASSERT_SEND_ERROR(pos_ipc_deq(pid,&pmsg) != POS_QUEUE_NOT_EMPTY,POS_OS_QUEUE_EMPTY);
        pos_get_current_task()->proc_fun(pmsg.message_type,pmsg.message_content,pmsg.src);
        return POS_OK;
}

PosStatusType pos_ipc_enq(_PID pid,pos_process_message_t * m){
	pos_process_message_queue_t *q = __mq[pid-1];
	pos_process_message_queue_t *temp_element;
	POS_BEGIN_KCRITICAL;	
	if(q==NULL){
		__mq[pid-1] = ( pos_process_message_queue_t *)pmalloc(sizeof(pos_process_message_queue_t));
                POS_ASSERT(__mq[pid-1] == NULL_PTR);
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
        POS_ASSERT(temp_element->next == NULL_PTR);
	temp_element = (pos_process_message_queue_t *)(temp_element->next);
	temp_element->element.src = m->src;
	temp_element->element.message_type = m->message_type;
        temp_element->element.message_content = m->message_content;
	temp_element->next = NULL;
	POS_END_KCRITICAL;
        return POS_OK;
}


PosQueueSTatus pos_ipc_deq(_PID pid,pos_process_message_t * res){
	
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
