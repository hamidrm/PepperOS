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
 *  @brief Send a string to UART project (Example projects)
 *      UART_TX -> PA9
 *      UART_RX -> PA10
 *
 */

#include "pepper_os.h"
#include "task1.h"
#include "shared.h"

pos_pid_type task1_pid;
pos_mutex_id_t test_mutex;

void Task1_Main(pos_pid_type pid){
  task1_pid = pid;
  pos_mutex_create(&test_mutex);
  pos_delay_ms(1000);
  while(1){
    pos_delay_ms(1000);
#if RUN_WITH_MUTEX == TRUE
    pos_mutex_acquire(&test_mutex);
#endif
    print("(T1):Started!\n",14);
    for(int i=0;i<0xFFFFFF;i++);
    print("(T1):Finished!\n",15);
#if RUN_WITH_MUTEX == TRUE
    pos_mutex_release(&test_mutex);
#endif
  }
}