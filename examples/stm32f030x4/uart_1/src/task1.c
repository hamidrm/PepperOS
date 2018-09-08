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
 *  @file    task1.c
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief Simple UART project (Example projects)
 *      printing every 1 second.
 */

#include "pepper_os.h"

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
    pos_console_rx_register(_pid);
    pos_add_timer(1000,&tid,1,TIMER_MODE_PERIODICALLY);
    pos_start_timer(tid);
    break;
  case POS_TASK_TIMER:
    print("Hello World From Task 1!\r\n",26 );
    break; 
  case POS_TASK_CONSOLE_RX:
    led_toggle();
    break;  
  }
  
}