#ifndef _PEPPER_OS_H_
#define _PEPPER_OS_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "pos_utility.h"
#include "pos_devices.h"
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
#include _ARCH_HEADER
#include "pos_console.h"

void pos_error(uint32_t error_no);
uint32_t pos_get_last_error(void);
void delay(volatile uint32_t time);
PosStatusType pos_create_task(task_start_handler_t main_fn,task_msg_proc_t process_fn,uint8_t * stack_addr,size_t stack_size,PosTaskPriority priority);
void pos_init(void);
void pos_delay_ms(uint32_t time);
void pos_os_sys_call(PosSysCallType num,uint32_t * args);


void pos_uart_sent(void);
void pos_print(char * text);

#define POS_HEAP_SIZE                   MEM_HEAP_SIZE
#define POS_HEAP_REGION                 __section_end(CSTACK)

#endif