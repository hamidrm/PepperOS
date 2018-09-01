#include "pepper_os.h"



void Task2_Main(_PID pid){
  //pos_process_message_t pmsg;
  pos_delay_ms(5000);
  pos_send_message(1,POS_TASK_SUICIDE,0x00);
  while(1){
    
   pos_delay_ms(200);
   print("Hello World From Task 2!",24);
    //pos_send_message(1,POS_TASK_SUICIDE,0x00);
  }
  
}



void Task2_Proc(pos_process_message_type msg_type,pos_process_message_content msg_cont,_PID src){
  switch(msg_type){
  case POS_TASK_STARTUP:
 
    break;
  case POS_TASK_TIMER:

    break;
    
    
  }
  
}