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
 *  @file    stm32f030x4_hal.h
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief Simple HAL header.
 *
 */


#ifndef _STM32F030X4_HAL_
#define _STM32F030X4_HAL_
#define HAL_MODULE_ENABLED


#include "../inc/stm32f030x4_defs.h"
#include "../inc/stm32f030x4_timer.h"
#include "../inc/stm32f030x4_uart.h"
#include "../inc/stm32f030x4_gpio.h"
#include "pepper_os.h"

#define RCC_CFGR_PLLMUL_POS     18
#define INTERNAL_OSC_FREQ        8 MHz
#define SCB_ICSR_PENDSVSET      (1UL << 28)
#define SCB_ICSR_PENDSVCLR      (1UL << 27)

#define SCB_ICSR_PENDSTSET      (1UL << 26)
#define SCB_ICSR_PENDSTCLR      (1UL << 25)


#define  __pos_clear_pending_cs() SCB->ICSR |= SCB_ICSR_PENDSVCLR  /* Clear pending PendSV */
#define  __pos_do_cs() SCB->ICSR |= SCB_ICSR_PENDSVSET  /* Trigger PendSV which performs the actual context switch: */
#define  __pos_clear_systick() SCB->ICSR |= SCB_ICSR_PENDSTCLR  /* Clear pending SysTick */
#define  __pos_get_stacking_size() 64
#define  __pos_cpu_sleep_ins()   __WFE()/* Standby CPU */



uint8_t clock_config(void);
void sys_tick_isr(void);
uint32_t sys_tick_get(void);
void NVIC_system_reset(void);
void NVIC_clear_pending_irq(IRQn_Type irq);
void NVIC_enable_irq(IRQn_Type irq);
void NVIC_disable_irq(IRQn_Type irq);
void NVIC_set_priority(IRQn_Type irq,uint32_t preempt_priority, uint32_t sub_priority);
void stm32f030x4_init(void);





#endif