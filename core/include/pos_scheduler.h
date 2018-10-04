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
 *  @file    pos_scheduler.h
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief pepper os scheduler header
 *
 */
#ifndef _POS_SCHEDULER_H_
#define _POS_SCHEDULER_H_


void pos_scheduler_init(void);
PosStatusType pos_schedule_init_task(task_start_handler_t start_handler,task_msg_proc_t proc_handler,uint8_t * stack,size_t stack_size,PosTaskPriority priority,pos_pid_type * pid);
void pos_task_finish(void);
extern inline PosSchedulerStatus pos_schedule_tasks(void);
void pos_scheduler_start(void);
pos_pid_type _pos_get_current_pid(void);
pos_task_type * pos_get_current_task(void);
void pos_kill_task(pos_pid_type pid);
pos_task_type * pos_get_task_by_pid(pos_pid_type pid);
void pos_get_total_elapsed_time(uint64_t *time);
void pos_get_sleep_elapsed_time(uint64_t *time);
void pos_force_cs_by_pid(pos_pid_type pid);
void pos_force_cs(void);
extern inline void pos_sleep_cpu(void);
extern inline void pos_set_current_mode(PosOsMode mode);

#endif

