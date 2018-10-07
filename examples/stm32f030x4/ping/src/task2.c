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
 *  @file    task2.c
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief Ping (Example projects)
 *
 */

#include "pepper_os.h"
#include "task2.h"
#include "shared.h"

extern pos_pid_type task1_pid;
pos_pid_type task2_pid;
static uint32_t tid;


static void InitBoard(void);

void Task2_Main(pos_pid_type pid){
  task2_pid = pid;
  while(1){
    pos_get_message();
  }
}

static void InitBoard(void){
  config_pin(LED_PORT,LED_PIN,OUTPUT_PUSH_PULL,PULL_UP);
}

void Task2_Proc(pos_process_message_type msg_type,pos_process_message_content msg_cont,pos_pid_type src){
  switch(msg_type){
  case POS_TASK_STARTUP:
    pos_add_timer(100,&tid,task2_pid,TIMER_MODE_PERIODICALLY);
    pos_start_timer(tid);
    InitBoard();
    break;
  case POS_TASK_TIMER:
    toggle_pin(LED_PORT,LED_PIN);
    break; 
  case MSG_PING:
    pos_send_message(task1_pid,MSG_PONG,0);
    break;
  }
}