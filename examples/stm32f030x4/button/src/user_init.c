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
 *  @file    user_init.c
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief Initializing PepperOS (and other things that user wants) and creating needed tasks.
 *      In this example , PepperOS handles two task to blinks 3 LED and controling button.
 *
 */

#include "pepper_os.h"

#include "task1.h"
#include "task2.h"

uint8_t s1[512];
uint8_t s2[512];

void main(void){
  pos_init();
  pos_create_task(Task1_Main,Task1_Proc,s1,512,POS_TASK_HIGH_PRIORITY);
  pos_create_task(Task2_Main,Task2_Proc,s2,512,POS_TASK_HIGH_PRIORITY);
  pos_scheduler_start(); 
}