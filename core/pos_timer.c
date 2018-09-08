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
 *  @file    pos_timer.c
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief pepper os timer management
 *
 */

#include "pepper_os.h"
#include "pos_timer.h"
#include _DEV_HAL_HEADER
#include _DEV_INIT_HEADER
#include _ARCH_HEADER


static uint8_t TIMER_STATE=0;
static _pos_timer_t * timer_ll_head = 0;


static PosStatusType pos_timer_mem_alloc(void ** new_timer);
static void pos_timer_mem_free(void * _timer);
static void pos_update_timers(int elapsed);
static void pos_set_expire_timer(uint32_t time_ms);
static PosStatusType pos__add_timer(uint32_t org_time,uint32_t time,timer_id_t id,_PID pid,uint8_t mode,uint8_t status);



static  PosStatusType pos_timer_mem_alloc(void ** new_timer){
  *new_timer = pmalloc(sizeof(_pos_timer_t));
  POS_ASSERT(*new_timer != NULL_PTR);
  return POS_OK;
}

static void pos_timer_mem_free(void * _timer){
  pfree(_timer);
}


uint8_t pos_kill_timer(uint32_t id){
    _pos_timer_t * ct = timer_ll_head;
    _pos_timer_t * before =0;
    POS_BEGIN_KCRITICAL;
    while (ct) {
      if(ct->timerId==id){
        if(before==0){
          timer_ll_head = ct->next;
          pos_timer_mem_free(ct);
        }else{
          before->next = ct->next;
          pos_timer_mem_free(ct);
        }
        pos_update_timers(timer_a_get_reload_val());
        if(TIMER_STATE && timer_ll_head==0){
          TIMER_STATE=0;
          timer_a_disable();
        }
        POS_END_KCRITICAL;
        return 1;
      }
      before = ct;
      ct = ct->next;
    }
    POS_END_KCRITICAL;
    return 0;
}


void pos_kill_all_timers(void){
    _pos_timer_t * ct = timer_ll_head;
    _pos_timer_t * before =0;
    POS_BEGIN_KCRITICAL;
    while (ct) {
      before = ct->next;
      pos_timer_mem_free(ct);
      ct  = before;
    }
    timer_ll_head=0;
    TIMER_STATE=0;
    timer_a_disable();
    POS_END_KCRITICAL;
}

uint8_t pos_start_timer(uint32_t id){
    _pos_timer_t * ct = timer_ll_head;
    POS_BEGIN_KCRITICAL;
    while (ct) {
      if(ct->timerId==id){
        ct->complete_time = ct->otime;
        ct->time = (ct->otime>TIMER_MAX_INTERVAL)?TIMER_MAX_INTERVAL:ct->otime;
        ct->status = TIMER_STATUS_RUNNING;
        if(!TIMER_STATE){
          TIMER_STATE=1;
          timer_a_enable();
        }
        POS_END_KCRITICAL;
        return 1;
      }
      ct = ct->next;
    }
    POS_END_KCRITICAL;
    return 0;
}


uint8_t pos_resume_timer(uint32_t id){
    _pos_timer_t * ct = timer_ll_head;
    POS_BEGIN_KCRITICAL;
    while (ct) {
      if(ct->timerId==id){
        ct->status = TIMER_STATUS_RUNNING;
        POS_END_KCRITICAL;
        return 1;
      }
      ct = ct->next;
    }
    POS_END_KCRITICAL;
    return 0;
}


uint8_t pos_stop_timer(uint32_t id){
    _pos_timer_t * ct = timer_ll_head;
    POS_BEGIN_KCRITICAL;
    while (ct) {
      if(ct->timerId==id){
        ct->status = TIMER_STATUS_STOP;
        pos_update_timers(timer_a_get_reload_val());
        POS_END_KCRITICAL;
        return 1;
      }
      ct = ct->next;
    }
    POS_END_KCRITICAL;
    return 0;
}

PosStatusType pos_add_timer(uint32_t time,timer_id_t *id,_PID pid,uint8_t mode) {
  uint32_t cid=0xFFFFFFFF;
  PosStatusType res;
  _pos_timer_t * ct = timer_ll_head;
  POS_BEGIN_KCRITICAL;
  while (ct) {
    if(ct->timerId==cid+1){
       cid = ct->timerId;
       ct = ct->next;
    }else break;
  }
  cid++;
  *id = cid;
 res = pos__add_timer(time,time,cid,pid,mode,TIMER_STATUS_STOP);
 POS_END_KCRITICAL;
 return res;
}


static PosStatusType pos__add_timer(uint32_t org_time,uint32_t time,timer_id_t id,_PID pid,uint8_t mode,uint8_t status) {
	_pos_timer_t * nt;
	_pos_timer_t * ct = timer_ll_head;
	_pos_timer_t * befor = 0;
        POS_ASSERT(pos_timer_mem_alloc((void **)&nt) != NULL_PTR);
        while (ct) {
          if (ct->time > time) {
            if(!befor){
              nt->next = ct;
              nt->time = (time>TIMER_MAX_INTERVAL)?TIMER_MAX_INTERVAL:time;
              nt->pid = pid;
              nt->timerId = id;
              nt->otime = org_time;
              nt->complete_time = time;
              nt->mode = mode;
              nt->status = status;
              timer_ll_head = nt;
              goto eoff;  
            }else{
              nt->pid = pid;
              befor->next = nt;
              nt->next = ct;
              nt->time = (time>TIMER_MAX_INTERVAL)?TIMER_MAX_INTERVAL:time;
              nt->timerId = id;
              nt->otime = org_time;
              nt->complete_time = time;
              nt->mode = mode;
              nt->status = status;
              goto eoff;
            }
          }
          befor = ct;
          ct = ct->next;
        }
	if (timer_ll_head==0) {
          nt->next = 0;
          nt->pid = pid;
          nt->time = (time>TIMER_MAX_INTERVAL)?TIMER_MAX_INTERVAL:time;
          nt->timerId = id;
          nt->otime = org_time;
          nt->complete_time = time;
          nt->mode = mode;
          nt->status = status;
          timer_ll_head = nt;
	}else{
          nt->next = 0;
          nt->pid = pid;
          nt->time = (time>TIMER_MAX_INTERVAL)?TIMER_MAX_INTERVAL:time;
          nt->timerId = id;
          nt->otime = org_time;
          nt->complete_time = time;
          nt->mode = mode;
          nt->status = status;
          befor->next = nt;          
        }
eoff:
        pos_set_expire_timer(timer_ll_head->time);
        return POS_OK;
}

static void pos_update_timers(int elapsed) {
	_pos_timer_t * ct = timer_ll_head;
	while (ct) {
             if(ct->time<elapsed)
                ct->time = 0;
             else{
               ct->complete_time -= elapsed;
		ct->time -= elapsed;
             }
              ct = ct->next;
	}
}





void POS_TIMER_A_EVENT(void){
    _pos_timer_t * before ;
    uint32_t time_org,time_total,time_id;
    _PID pid;
    uint8_t mode,status;
    POS_BEGIN_KCRITICAL;
    if(timer_ll_head==0){
      if(TIMER_STATE){
        TIMER_STATE=0;
        timer_a_disable();
      }
      timer_a_reset_elapsed_status();
      POS_END_KCRITICAL;
      return;
    }
    if (timer_a_get_elapsed_status())
    {
      timer_a_reset_elapsed_status();
        if(timer_ll_head != 0){
          pos_update_timers(timer_ll_head->time);
        before = timer_ll_head;
        while(timer_ll_head && timer_ll_head->time==0){
          before = timer_ll_head->next;
          if(timer_ll_head->complete_time != 0)
          {
            time_total = timer_ll_head->complete_time;//(timer_ll_head->complete_time>TIMER_MAX_INTERVAL)?TIMER_MAX_INTERVAL:timer_ll_head->complete_time;
            time_org = timer_ll_head->otime;
            time_id = timer_ll_head->timerId;
            pid = timer_ll_head->pid;
            mode = timer_ll_head->mode;
            status = timer_ll_head->status;
            pos_timer_mem_free(timer_ll_head);
            timer_ll_head = before;
            pos__add_timer(time_org,time_total,time_id,pid,mode,status);
            if(timer_ll_head)
              pos_set_expire_timer(timer_ll_head->time);  
          }else{
            if(timer_ll_head->status == TIMER_STATUS_RUNNING)
              pos_send_message(timer_ll_head->pid,POS_TASK_TIMER,timer_ll_head->timerId);
            if(timer_ll_head->mode == TIMER_MODE_PERIODICALLY){
                time_org = timer_ll_head->otime;
                time_id = timer_ll_head->timerId;
                pid = timer_ll_head->pid;
                mode = timer_ll_head->mode;
                status = timer_ll_head->status;
                pos_timer_mem_free(timer_ll_head);
                timer_ll_head = before;
                pos__add_timer(time_org,time_org,time_id,pid,mode,status);
                if(timer_ll_head)
                  pos_set_expire_timer(timer_ll_head->time);
              }else if(timer_ll_head->mode == TIMER_MODE_ONE_SHOT){
                pos_timer_mem_free(timer_ll_head);
                timer_ll_head = before;
                if(timer_ll_head)
                  pos_set_expire_timer(timer_ll_head->time);
              }
            }
          //before = timer_ll_head->next;
        }
      }
      
      
    }
    if(TIMER_STATE && timer_ll_head==0){
      TIMER_STATE=0;
      timer_a_disable();
    }
    
    POS_END_KCRITICAL;
}



static void pos_set_expire_timer(uint32_t time_ms){
  timer_a_set_reload_val(time_ms);
}


void pos_init_timers(void){
  timer_a_init();
  TIMER_STATE = 0; 
}

/**
@endcode
*/