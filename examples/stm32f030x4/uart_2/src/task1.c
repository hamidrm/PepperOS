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
 *  @brief UART manager task (Example projects)
 *      UART_TX -> PA9
 *      UART_RX -> PA10
 *
 */

#include "pepper_os.h"
#include "task1.h"
#include "shared.h"

void InitBoard(void);

pos_pid_type task1_pid;
static uint32_t tid;

static uint8_t rx_buffer[MAX_RX_BUFFER_LEN];
static uint8_t rx_recv_buffer_idx;
static uint8_t uart_session_expired;

void Task1_Main(pos_pid_type pid){
  task1_pid = pid;
  while(1){
    pos_get_message();
  }
}

void InitBoard(void){
  config_pin(LED_RED_PORT,LED_RED_PIN,OUTPUT_PUSH_PULL,PULL_DOWN);
  config_pin(LED_GREEN_PORT,LED_GREEN_PIN,OUTPUT_PUSH_PULL,PULL_DOWN);
  config_pin(LED_BLUE_PORT,LED_BLUE_PIN,OUTPUT_PUSH_PULL,PULL_DOWN);
}


void Task1_Proc(pos_process_message_type msg_type,pos_process_message_content msg_cont,pos_pid_type src){
  switch(msg_type){
  case POS_TASK_STARTUP:
    pos_console_rx_register(task1_pid);
    rx_recv_buffer_idx = 0;
    uart_session_expired = 1;
    InitBoard();
    break;
  case POS_TASK_TIMER:
    {
      if(msg_cont == tid){
        if(rx_recv_buffer_idx){
          if(strcmp("RED LED ON\n",(char *)rx_buffer)==0)
            set_pin(LED_RED_PORT,LED_RED_PIN);
          else if(strcmp("RED LED OFF\n",(char *)rx_buffer)==0)
            reset_pin(LED_RED_PORT,LED_RED_PIN);
          else if(strcmp("GREEN LED ON\n",(char *)rx_buffer)==0)
            set_pin(LED_GREEN_PORT,LED_GREEN_PIN);
          else if(strcmp("GREEN LED OFF\n",(char *)rx_buffer)==0)
            reset_pin(LED_GREEN_PORT,LED_GREEN_PIN);
          else if(strcmp("BLUE LED ON\n",(char *)rx_buffer)==0)
            set_pin(LED_BLUE_PORT,LED_BLUE_PIN);
          else if(strcmp("BLUE LED OFF\n",(char *)rx_buffer)==0)
            reset_pin(LED_BLUE_PORT,LED_BLUE_PIN);
          print((char *)rx_buffer,rx_recv_buffer_idx );
        }
        rx_recv_buffer_idx = 0;
        uart_session_expired = 1;
        memset(rx_buffer,0,MAX_RX_BUFFER_LEN);
      }
    }
    break; 
  case POS_TASK_CONSOLE_RX:
    if(rx_recv_buffer_idx < MAX_RX_BUFFER_LEN)
      rx_buffer[rx_recv_buffer_idx++] = msg_cont;
    if(uart_session_expired){
      uart_session_expired = 0;
      pos_add_timer(100,&tid,task1_pid,TIMER_MODE_ONE_SHOT);
      pos_start_timer(tid);
    }
    break;  
  }
}