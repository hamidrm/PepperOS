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
 *  @brief Ping (Example projects)
 *      UART_TX -> PA9
 *      UART_RX -> PA10
 *
 */

#include "pepper_os.h"
#include "task1.h"
#include "shared.h"

pos_pid_type task1_pid;
extern pos_pid_type task2_pid;
static uint32_t start_time;
static uint32_t tid;

static const char digits[16] = "0123456789abcdef";
static uint8_t _itoa(uint32_t a,char * out,uint8_t radix);
static uint8_t itoa(uint32_t a,char * out,uint8_t radix);

void Task1_Main(pos_pid_type pid){
  task1_pid = pid;
  while(1){
    pos_get_message();
  }
}

static uint8_t _itoa(uint32_t a,char * out,uint8_t radix){
  if(a==0)
    return 0;
  else
  {
    uint8_t l = _itoa(a/radix,out,radix);
    out[l] = digits[a%radix];
    return l + 1;
  }
}

static uint8_t itoa(uint32_t a,char * out,uint8_t radix){
  uint8_t len = _itoa(a,out,radix);
  out[len] = 0;
  return len+1;
}


void Task1_Proc(pos_process_message_type msg_type,pos_process_message_content msg_cont,pos_pid_type src){
  switch(msg_type){
  case POS_TASK_STARTUP:
    pos_add_timer(1000,&tid,task1_pid,TIMER_MODE_PERIODICALLY);
    pos_start_timer(tid);
    start_time = 0;
    break;
  case POS_TASK_TIMER:
    start_time = pos_total_time();
    pos_send_message(task2_pid,MSG_PING,0);

    break;
  case MSG_PONG:
    {
      char buff[12];
      uint32_t diff_time = pos_total_time() - start_time;
      if(diff_time != 0 ){
        uint8_t len = itoa(diff_time,buff,10);
        print("Ping :",6);
        print(buff,len-1);
        print(" (ms)\n",6);
      }
    }
    break;
  }
}
