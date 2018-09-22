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
 *  @file    stm32f030x4_timer.h
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief Timer header.
 *
 */

#ifndef _STM32F030X4_TIMER_H_
#define _STM32F030X4_TIMER_H_
#if defined(POS_TIMER)

#define  TIMER_MAX_VALUE (1<<16 - 1)

typedef enum {
    POS_TIMER0 = (uint32_t)TIM3,
    POS_TIMER1 = (uint32_t)TIM14,
    POS_TIMER2 = (uint32_t)TIM16,
    POS_TIMER3 = (uint32_t)TIM17,
} PosTimerList;

typedef void(*int_isr_t)(void);
void timer_init(PosTimerList timer,uint32_t prescale,int_isr_t isr);
uint8_t timer_get_elapsed_status(PosTimerList timer);
void timer_reset_elapsed_status(PosTimerList timer);
void timer_set_reload_val(PosTimerList timer,uint32_t value);
uint32_t timer_get_reload_val(PosTimerList timer);
void timer_enable(PosTimerList timer);
void timer_disable(PosTimerList timer);
void timer_enable_evt(PosTimerList timer);
void timer_disable_evt(PosTimerList timer);
void TIM3_IRQHandler(void);
void TIM6_IRQHandler(void);
void TIM14_IRQHandler(void);
void TIM15_IRQHandler(void);
void TIM16_IRQHandler(void);
void TIM17_IRQHandler(void);

#endif
#endif

