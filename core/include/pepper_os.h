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
 *  @file    pepper_os.h
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief main pepper os header
 *
 */

#ifndef _PEPPER_OS_H_
#define _PEPPER_OS_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "pos_utility.h"
#include "pos_devices.h"
#include _DEV_HAL_HEADER_2
#include "pos_types.h"
#include "pos_config.h"
#include "pos_memory.h"
#include "pos_timer.h"
#include "pos_scheduler.h"
#include "pos_semaphore.h"
#include "pos_mutex.h"
#include "pos_ipc.h"
#include "pos_delay.h"
#include "queue.h"
#include "pos_console.h"
#include _ARCH_HEADER

#include "pos_extint.h"

void pos_error(uint32_t error_no);
uint32_t pos_get_last_error(void);
PosStatusType pos_create_task(task_start_handler_t main_fn,task_msg_proc_t process_fn,uint8_t * stack_addr,size_t stack_size,PosTaskPriority priority);
void pos_init(void);
void pos_delay_ms(uint32_t time);
void pos_os_sys_call(PosSysCallType num,uint32_t * args);
uint32_t pos_task_running_time(pos_pid_type pid);
uint32_t pos_sleep_time(void);
uint32_t pos_total_time(void);

void pos_uart_sent(void);
void pos_print(char * text);

#define POS_HEAP_REGION                 __section_end(CSTACK)

#endif