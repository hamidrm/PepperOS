/*
* @brief POS Semaphore Management
* @file pos_semaphore.c
* @author Hamid Reza Mehrabian
* @code
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

static PosStatusType pos_semaphore_enq(pos_semaphore_t *s,_PID pid);
static PosStatusType pos_semaphore_deq(pos_semaphore_t *s,_PID * pid);

PosStatusType pos_semaphore_init(pos_semaphore_t *s,int16_t iv){
	s->cnt = iv;
	s->q=NULL;
        s->released_task = 0;
        return POS_OK;
}

PosStatusType pos_semaphore_wait(pos_semaphore_t *s){
        _PID pid = _pos_get_current_pid();
        POS_ASSERT(s == NULL_PTR);
	POS_BEGIN_KCRITICAL;
        s->cnt-=1;	
	if(s->cnt>=0){
            POS_END_KCRITICAL;
            return POS_OK;
	}
	POS_ASSERT(pos_semaphore_enq(s,_pos_get_current_pid()) != POS_OK);
        pos_get_current_task()->status = POS_TASK_STATUS_SEM_WAITING;
	
        pos_force_cs();
        POS_END_KCRITICAL;
        pos_yield_delay();
        return POS_OK;
}

PosStatusType pos_semaphore_wait_until(pos_semaphore_t *s,uint32_t time_ms){
        _PID pid = _pos_get_current_pid();
        POS_ASSERT_SEND_ERROR(s == NULL_PTR,POS_MEM_UNALLOCATED);
        
	POS_BEGIN_KCRITICAL;
        s->cnt-=1;	
	if(s->cnt>=0){
            POS_END_KCRITICAL;
            return POS_OK;
	}
	POS_ASSERT(pos_semaphore_enq(s,_pos_get_current_pid()) != POS_OK);

	pos_delay_add(pos_get_current_task()->pid,time_ms,POS_TASK_STATUS_SEM_WAITING_UNTIL);
        /* 
        while(pos_semaphore_get_value(s)!=pid); //Blocking in single threading , Goto next thread otherwise.
        */

        pos_force_cs();
        POS_END_KCRITICAL;
        pos_yield_delay();
        
        if(pos_get_current_task()->status == POS_TASK_STATUS_DELAY_TIMEOUT){
          /* Increase semaphore */
          s->cnt+=1;
          POS_ASSERT(pos_semaphore_deq(s,NULL_PTR) != POS_OK);
          pos_get_current_task()->status = POS_TASK_STATUS_ACTIVE;
          return POS_TIMEOUT;
        }else{
          pos_delay_remove(pid);
          return POS_OK;
        }
}

PosStatusType pos_semaphore_signal(pos_semaphore_t *s,_PID * pid){
        _PID _pid;
        POS_ASSERT_SEND_ERROR(s == NULL_PTR,POS_MEM_UNALLOCATED);
        
        POS_BEGIN_KCRITICAL;
	s->cnt+=1;
	POS_ASSERT(pos_semaphore_deq(s,&_pid) != POS_OK);
        if(_pid != POS_KERNEL_PID &&  pos_get_task_by_pid(_pid)->priority == POS_TASK_HIGH_PRIORITY)
          pos_force_cs_by_pid(_pid);
        POS_END_KCRITICAL;
        pos_yield_delay();
        if(pid != NULL_PTR)
          *pid = _pid;
        return POS_OK;
}

static PosStatusType pos_semaphore_enq(pos_semaphore_t *s,_PID pid){
	pos_semaphore_queue_t * q = s->q;
	pos_semaphore_queue_t *temp_element;
        POS_ASSERT_SEND_ERROR(s == NULL_PTR,POS_MEM_UNALLOCATED);
        POS_ASSERT(pos_get_task_by_pid(pid) == NULL_PTR);
	if(q==NULL){
		s->q = (pos_semaphore_queue_t *)pmalloc(sizeof(pos_semaphore_queue_t));
                POS_ASSERT(s->q == NULL_PTR);
		s->q->pid = pid;
		s->q->next = NULL;
		return POS_OK;
	}
	while(q){
		temp_element = (pos_semaphore_queue_t *)(q);
		q=(pos_semaphore_queue_t *)(temp_element->next);
	}
	temp_element->next = (pos_semaphore_queue_t *)pmalloc(sizeof(pos_semaphore_queue_t));
        POS_ASSERT(temp_element->next == NULL_PTR);
	temp_element = (pos_semaphore_queue_t *)(temp_element->next);
	temp_element->next = NULL;
	temp_element->pid = pid;
        return POS_OK;
}


static PosStatusType pos_semaphore_deq(pos_semaphore_t *s,_PID * pid){
	_PID res;
	pos_semaphore_queue_t * q = s->q;
	pos_semaphore_queue_t *temp_element;
        
        POS_ASSERT_SEND_ERROR(s == NULL_PTR,POS_MEM_UNALLOCATED);
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