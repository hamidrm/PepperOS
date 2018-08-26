#include "pepper_os.h"
#include _DEV_HAL_HEADER
#include _DEV_INIT_HEADER
uint32_t tid;
_PID _pid;


void Task1_Main(_PID pid){
  _pid = pid;
  while(1){
    pos_get_message();
  }
}



void Task1_Proc(pos_process_message_type msg_type,pos_process_message_content msg_cont,_PID src){
  switch(msg_type){
  case POS_TASK_STARTUP:
    led_toggle();
    pos_delay_ms(80);
    led_toggle();
    pos_delay_ms(80);
    led_toggle();
    pos_delay_ms(80);
    led_toggle();
    pos_delay_ms(80);
    pos_console_rx_register(_pid);
    pos_add_timer(1000,&tid,1,TIMER_MODE_PERIODICALLY);
    break;
  case POS_TASK_TIMER:
    //led_toggle();
    print("Hello World From Task 1!",24 );
    break;
  case POS_TASK_SUICIDE:
    pos_start_timer(tid);
    break;   
  case POS_TASK_CONSOLE_RX:
    led_toggle();
    break;  
  }
  
}