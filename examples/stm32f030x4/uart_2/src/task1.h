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
 *  @file    task1.h
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief LED Blinking project (Example projects)
 *      LED_RED -> PA5
 *      LED_GREEN -> PA6
 *      LED_BLUE -> PA7
 *
 */

#ifndef _TASK1_H_
#define _TASK1_H_

#define MAX_RX_BUFFER_LEN       32

#define LED_RED_PORT         PORTA
#define LED_GREEN_PORT       PORTA
#define LED_BLUE_PORT        PORTA

#define LED_RED_PIN         PIN7
#define LED_GREEN_PIN       PIN6
#define LED_BLUE_PIN        PIN5

void Task1_Main(pos_pid_type pid);
void Task1_Proc(pos_process_message_type,pos_process_message_content,pos_pid_type src);


#endif