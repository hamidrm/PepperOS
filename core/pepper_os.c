#include "pepper_os.h"
#include _DEV_HAL_HEADER
#include _DEV_INIT_HEADER

static uint32_t last_error_no;

void pos_init(void)
{
  DEV_INIT();
  pos_arch_init();
  init_pos_memory();
  pos_init_timers();
  led_init();
  pos_ipc_init();
#if MAX_MUTEX_NUM > 0
  pos_mutex_init();
#endif
  pos_scheduler_init();
  pos_delay_init();
  pos_console_init();
}

void pos_delay_ms(uint32_t time){
  POS_BEGIN_KCRITICAL;
  pos_delay_add(pos_get_current_task()->pid,time,POS_TASK_STATUS_DELAY);
  pos_force_cs();
  POS_END_KCRITICAL;
  pos_yield_delay();
}


PosStatusType pos_create_task(task_start_handler_t main_fn,task_msg_proc_t process_fn,uint8_t * stack_addr,size_t stack_size,PosTaskPriority priority){
  uint8_t * _stack_addr;
  _PID pid;
  if(stack_addr != 0)
    _stack_addr = stack_addr;
  else
    _stack_addr = (uint8_t *)pmalloc(stack_size);
  
  if(_stack_addr==NULL)
    return POS_ERROR;
  pos_schedule_init_task(main_fn,process_fn,_stack_addr,stack_size,priority,&pid);
  pos_send_message(pid,POS_TASK_STARTUP,pid); // Send Startup Message
  return POS_OK;
}

void pos_error(uint32_t error_no){
  last_error_no = error_no;
}

uint32_t pos_get_last_error(void){
  return last_error_no;
}


void delay(volatile uint32_t time)
{
	while (time > 0)
		time--;
}


void pos_os_sys_call(PosSysCallType num,uint32_t * args){
  pos_set_current_mode(POS_KERNEL_ACTIVE);
  switch(num){
  case POS_SYS_CALL_SLEEP_CPU:
    pos_sleep_cpu();
    break;

  }
  pos_set_current_mode(POS_TASKS_ACTIVE);
}