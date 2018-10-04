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
 *  @brief LED Blinking and button project (Example projects)
 *      LED_RED -> PA5
 *      LED_GREEN -> PA6
 *      LED_BLUE -> PA7
 *      Button -> PA9
 *
 */

#include "pepper_os.h"
#include "task1.h"
#include "shared.h"

pos_pid_type task1_pid;
uint8_t led_num;
uint32_t tid,btn_debounce_timer;
uint8_t order;

void InitBoard(void);
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
  pos_extint_add(BTN_PORT,BTN_PIN,EXTINT_FALLING,PULL_UP);
  order = RED_TO_BLUE;
}


void Task1_Proc(pos_process_message_type msg_type,pos_process_message_content msg_cont,pos_pid_type src){
  switch(msg_type){
  case POS_TASK_STARTUP:
    {
      led_num = 0;
      InitBoard();
    }
    break;
    
  case POS_TASK_EXT_INT:
    {
      order ^= 1;
      pos_add_timer(500,&btn_debounce_timer,task1_pid,TIMER_MODE_ONE_SHOT);
      pos_start_timer(btn_debounce_timer);
      
    }
    break;
  case POS_TASK_TIMER:
    
    if(btn_debounce_timer == msg_cont){
      extint_release(BTN_PORT,BTN_PIN);
      btn_debounce_timer = 0xFFFFFFFF;
    }else{
      switch(led_num){
      case 0:
        reset_pin(LED_RED_PORT,LED_RED_PIN);
        break;
      case 1:
        reset_pin(LED_GREEN_PORT,LED_GREEN_PIN);
        break;
      case 2:
        reset_pin(LED_BLUE_PORT,LED_BLUE_PIN);
        break;  
      }
      if(order == RED_TO_BLUE){
        led_num++;
        led_num%=3;
      }else{
        if(led_num>0)
          led_num--;
        else
          led_num = 2;
      }
    }
    break;
  case BLINK_LED_MESSAGE:
    pos_add_timer(80,&tid,task1_pid,TIMER_MODE_ONE_SHOT);
    pos_start_timer(tid);
    switch(led_num){
    case 0:
      set_pin(LED_RED_PORT,LED_RED_PIN);
      break;
    case 1:
      set_pin(LED_GREEN_PORT,LED_GREEN_PIN);
      break;
    case 2:
      set_pin(LED_BLUE_PORT,LED_BLUE_PIN);
      break;  
    }
    break;
  }
  
}