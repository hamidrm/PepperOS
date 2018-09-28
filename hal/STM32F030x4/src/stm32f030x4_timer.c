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
 *  @file    stm32f030x4_timer.c
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief Managing Timers.
 *
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "pos_utility.h"
#include "pos_devices.h"
#include "../drivers/std/stm32f0xx_hal.h"
#include "../inc/stm32f030x4_timer.h"


#if defined(POS_TIMER)

 
static int_isr_t int_isr[4];


void timer_init(PosTimerList timer,uint32_t prescale,int_isr_t isr){
  uint8_t tim_int_idx = 0;
  if(timer==POS_TIMER0){
    __HAL_RCC_TIM3_CLK_ENABLE();
    tim_int_idx = 0;
  }
  if(timer==POS_TIMER1){
    __HAL_RCC_TIM14_CLK_ENABLE();
    tim_int_idx = 1;
  }
  if(timer==POS_TIMER2){
    __HAL_RCC_TIM16_CLK_ENABLE();
    tim_int_idx = 2;
  }
  if(timer==POS_TIMER3){
    __HAL_RCC_TIM17_CLK_ENABLE();
    tim_int_idx = 3;
  }
  
//  if(isr != NULL)
//    ((TIM_TypeDef*)timer)->EGR |= TIM_EGR_UG;
  ((TIM_TypeDef*)timer)->PSC = prescale;
  
  if(isr != NULL)
    ((TIM_TypeDef*)timer)->DIER = TIM_DIER_UIE;
  
  int_isr[tim_int_idx] = isr;
  
  if(timer==POS_TIMER0){
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
    HAL_NVIC_SetPriority(TIM3_IRQn,0x01,0);
  }
  if(timer==POS_TIMER1){
    HAL_NVIC_EnableIRQ(TIM14_IRQn);
    HAL_NVIC_SetPriority(TIM14_IRQn,0x01,0);
  }
  if(timer==POS_TIMER2){
    HAL_NVIC_EnableIRQ(TIM16_IRQn);
    HAL_NVIC_SetPriority(TIM16_IRQn,0x01,0);
  }
  if(timer==POS_TIMER3){
    HAL_NVIC_EnableIRQ(TIM17_IRQn);
    HAL_NVIC_SetPriority(TIM17_IRQn,0x01,0);
  }

}


uint8_t timer_get_elapsed_status(PosTimerList timer){
  return (((TIM_TypeDef*)timer)->SR & TIM_SR_UIF)==0?0:1;
}

void timer_reset_elapsed_status(PosTimerList timer){
  ((TIM_TypeDef*)timer)->SR &= ~TIM_SR_UIF;
}

void timer_set_reload_val(PosTimerList timer,uint32_t value){
  ((TIM_TypeDef*)timer)->ARR = value;
  //((TIM_TypeDef*)timer)->CNT = 0;
}


uint32_t timer_get_reload_val(PosTimerList timer){
  return ((TIM_TypeDef*)timer)->ARR;
}

uint32_t timer_get_current_val(PosTimerList timer){
  return ((TIM_TypeDef*)timer)->CNT;
}

void timer_set_current_val(PosTimerList timer,uint32_t value){
  ((TIM_TypeDef*)timer)->CNT = value;
}

void timer_enable(PosTimerList timer){
  ((TIM_TypeDef*)timer)->CR1 |= TIM_CR1_CEN ;
}

void timer_disable(PosTimerList timer){
  ((TIM_TypeDef*)timer)->CR1 &= ~TIM_CR1_CEN ;
  ((TIM_TypeDef*)timer)->CNT = 0;
}


void timer_enable_evt(PosTimerList timer){
  if(timer==POS_TIMER0)
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
  if(timer==POS_TIMER1)
    HAL_NVIC_EnableIRQ(TIM14_IRQn);
  if(timer==POS_TIMER2)
    HAL_NVIC_EnableIRQ(TIM16_IRQn);
  if(timer==POS_TIMER3)
    HAL_NVIC_EnableIRQ(TIM17_IRQn);
}

void timer_disable_evt(PosTimerList timer){
  if(timer==POS_TIMER0)
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
  if(timer==POS_TIMER1)
    HAL_NVIC_DisableIRQ(TIM14_IRQn);
  if(timer==POS_TIMER2)
    HAL_NVIC_DisableIRQ(TIM16_IRQn);
  if(timer==POS_TIMER3)
    HAL_NVIC_DisableIRQ(TIM17_IRQn);
}

void TIM3_IRQHandler(void)
{
  if(int_isr[0])
    int_isr[0]((TIM3->SR & TIM_SR_UIF)==0?0:1);
}

void TIM14_IRQHandler(void)
{
  if(int_isr[1])
    int_isr[1]((TIM3->SR & TIM_SR_UIF)==0?0:1);
}

void TIM16_IRQHandler(void)
{
  if(int_isr[2])
    int_isr[2]((TIM3->SR & TIM_SR_UIF)==0?0:1);
}

void TIM17_IRQHandler(void)
{
  if(int_isr[3])
    int_isr[3]((TIM3->SR & TIM_SR_UIF)==0?0:1);
}


#endif