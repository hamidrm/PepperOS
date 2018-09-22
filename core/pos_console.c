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
 *  @file    pos_console.c
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief pepper os console management
 *
 */

#include <stdint.h>
#include "pepper_os.h"

#include _DEV_HAL_HEADER

#if USE_CONSOLE == TRUE
static pos_hw_uart_t pos_uart;
static uint16_t bytes_remained;
static uint16_t bytes_cnt;
static pos_queue_t * console_queue;
static uint8_t is_sending = 0;
static pos_pid_type     recv_pid;
static uint8_t * tx_buffer=NULL_PTR;
static void data_sent(void);


void pos_console_init(void){
  pos_uart.baudrate = CONSOLE_BAUDRATE;
  pos_uart.data_mode = POS_UART_DATA_MODE_1B_START_8B_DATA_1B_STOP;
  pos_uart.int_flags = POS_UART_INT_FLAG_DATA_SENT | POS_UART_INT_FLAG_DATA_READY;
  pos_uart.parity_mode = POS_UART_PARITY_MODE_NONE;
  uart_init(&pos_uart,POS_UART1);
  bytes_remained = 0;
  bytes_cnt = 0;
  tx_buffer = 0;
  pos_create_queue(&console_queue);
  recv_pid = POS_KERNEL_PID;
  print(pos_information,strlen(pos_information));
}


void pos_console_rx_register(pos_pid_type pid){
  recv_pid = pid;
}


PosStatusType print(const char * text,uint8_t len){
  uint8_t * buffer = (uint8_t *)pmalloc(len);
  POS_ASSERT(buffer != NULL_PTR);
  memcpy(buffer,text,len); //< TODO
  
  pos_queue_enq(console_queue,(void *)buffer,len);
  
  if(is_sending==0){
    tx_buffer = NULL_PTR;
    is_sending = 1;
    data_sent();
  }
  return POS_OK;
}

static void data_sent(void){
  size_t cnt;
  if(tx_buffer != NULL_PTR){
    pfree(tx_buffer);
    tx_buffer = NULL_PTR;
  }
  pos_queue_count(console_queue,&cnt);
  if(cnt){
    char * text = 0;
    size_t len;
    pos_queue_deq(console_queue,(void *)&text,&len);
    pos_console_print(text,len);
  }else
    is_sending = 0;
}

void pos_console_print(const char * text,uint8_t len){
  tx_buffer = (uint8_t *)text;
  bytes_cnt = 0;
  bytes_remained = len-1;
  uart_write(&pos_uart,tx_buffer[bytes_cnt++]);
}

void uart_isr(void){
  PosUartIntFlags flags;
  uart_get_int_type(&pos_uart,&flags);
  if(flags & POS_UART_INT_FLAG_DATA_SENT)
    if(bytes_remained){
      uart_write(&pos_uart,tx_buffer[bytes_cnt++]);
      bytes_remained--;
    }else
      data_sent();
  else if(flags & POS_UART_INT_FLAG_DATA_READY){
    uint8_t rx;
    if(recv_pid != POS_KERNEL_PID){
      uart_read(&pos_uart,&rx);
      pos_send_message(recv_pid,POS_TASK_CONSOLE_RX,rx);
    }
  }
}

#endif
