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
 *  @file    stm32f030x4_hal.c
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief Simple HAL
 *
 */


#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../inc/stm32f030x4_hal.h"
#include "../drivers/std/stm32f0xx_hal.h"


void NVIC_set_priority(IRQn_Type irq,uint32_t preempt_priority, uint32_t sub_priority)
{
  HAL_NVIC_SetPriority(irq,preempt_priority,sub_priority);
}


void NVIC_disable_irq(IRQn_Type irq){
  HAL_NVIC_DisableIRQ(irq);
}

void NVIC_enable_irq(IRQn_Type irq){
  HAL_NVIC_EnableIRQ(irq);
}

void NVIC_clear_pending_irq(IRQn_Type irq){
  HAL_NVIC_ClearPendingIRQ(irq);
}

void NVIC_system_reset(void){
  HAL_NVIC_SystemReset();
}


uint32_t sys_tick_get(void){
  return HAL_GetTick();
}

void sys_tick_isr(void){
  HAL_IncTick();
}


uint8_t clock_config(void){
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

#if EXT_CRYSTAL_STATUS == FALSE
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
#else
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
#endif
  
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLMUL = (((((SYS_FREQ)/(INTERNAL_OSC_FREQ))<<1) - 2) << RCC_CFGR_PLLMUL_POS);
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    pos_error(POS_SYS_INVALID_CLK);
    return 1;
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK)
  {
    pos_error(POS_SYS_INVALID_CLK);
    return 1;
  }
  return 0;
}

void stm32f030x4_init(void){
    HAL_Init();
    clock_config();
    HAL_NVIC_SetPriority(PendSV_IRQn, 0xFF,0); /* Lowest possible priority */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0x00,0); /* Highest possible priority */
    HAL_NVIC_SetPriority(SVC_IRQn, 0xFF,0); /* High priority */
}