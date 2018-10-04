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
 *  @file    stm32f030x4_uart.c
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief Managing UART.
 *
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "pos_utility.h"
#include "pos_devices.h"
#include "../drivers/std/stm32f0xx_hal.h"
#include "../inc/stm32f030x4_uart.h"



#if defined(POS_UART)

void USART1_IRQHandler(void)
{
  uart_isr();
}


__weak void uart_isr(void){
  
}

void uart_get_int_type(pos_hw_uart_t * pos_uart,PosUartIntFlags * ints){
  USART_TypeDef * Ins = (USART_TypeDef *)(pos_uart->handler);
  if ((Ins->ISR & USART_ISR_TC) == USART_ISR_TC)
  {
    Ins->ICR |= USART_ICR_TCCF;
    *ints = POS_UART_INT_FLAG_DATA_SENT;
  }else if ((Ins->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
  {
    *ints = POS_UART_INT_FLAG_DATA_READY;
  }else if ((Ins->ISR & USART_ISR_TXE) == USART_ISR_TXE)
  {
    *ints = POS_UART_INT_FLAG_TX_BUFFER_EMPTY;
  }
}

void uart_clear_int_flag(pos_hw_uart_t * pos_uart,PosUartIntFlags * ints){
  USART_TypeDef * Ins = (USART_TypeDef *)(pos_uart->handler);
  if(POS_UART_INT_FLAG_DATA_SENT)
    Ins->ICR |= USART_ICR_TCCF;
}
                       
void uart_write(pos_hw_uart_t * uart_params,uint8_t data){
  while (!(((USART_TypeDef *)(uart_params->handler))->ISR & USART_ISR_TXE));
  ((USART_TypeDef *)(uart_params->handler))->TDR = data;
}

void uart_read(pos_hw_uart_t * uart_params,uint8_t * data){
  while (!(((USART_TypeDef *)(uart_params->handler))->ISR & USART_ISR_RXNE));
  *data = ((USART_TypeDef *)(uart_params->handler))->RDR;
}

void uart_init(pos_hw_uart_t * uart_params,PosUartList uart){
  GPIO_InitTypeDef  GPIO_InitStruct;
  USART_TypeDef * Instance;
  
  _STM32F_TX_PORT_CLK_ENABLE();
  _STM32F_RX_PORT_CLK_ENABLE();
  _STM32F_USART_CLK_ENABLE();

  
  GPIO_InitStruct.Pin       = USART1_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = USART1_TX_AF;
  HAL_GPIO_Init(USART1_TX_PORT, &GPIO_InitStruct);


  GPIO_InitStruct.Pin = USART1_RX_PIN;
  GPIO_InitStruct.Alternate = USART1_RX_AF;
  HAL_GPIO_Init(USART1_RX_PORT, &GPIO_InitStruct);
  
  if(uart == POS_UART1)
    uart_params->handler = (void *)USART1;
  Instance = (USART_TypeDef *)uart_params->handler;
  
  switch(uart_params->data_mode){
  case POS_UART_DATA_MODE_1B_START_8B_DATA_1B_STOP:

    break;
  case POS_UART_DATA_MODE_1B_START_9B_DATA_1B_STOP:
    Instance->CR1 |= USART_CR1_M0;
    break;
  case POS_UART_DATA_MODE_1B_START_8B_DATA_2B_STOP:
    Instance->CR2 |= USART_CR2_STOP_1;
    break;
  case POS_UART_DATA_MODE_1B_START_9B_DATA_2B_STOP:
    Instance->CR1 |= USART_CR1_M0;
    Instance->CR2 |= USART_CR2_STOP_1;
    break;
  }
  
  if(uart_params->int_flags & POS_UART_INT_FLAG_DATA_READY)
    Instance->CR1 |= USART_CR1_RXNEIE;

  if(uart_params->int_flags & POS_UART_INT_FLAG_DATA_SENT)
    Instance->CR1 |= USART_CR1_TCIE;

  if(uart_params->int_flags & POS_UART_INT_FLAG_TX_BUFFER_EMPTY)
    Instance->CR1 |= USART_CR1_TXEIE;

  switch(uart_params->parity_mode){
  case POS_UART_PARITY_MODE_NONE:
    break;
  case POS_UART_PARITY_MODE_ODD:
    Instance->CR1 |= USART_CR1_PS;
    Instance->CR1 |= USART_CR1_PCE;
    break;
  case POS_UART_PARITY_MODE_EVEN:
    Instance->CR1 |= USART_CR1_PCE;
    break;
  }
  
  Instance->BRR = 48000000/uart_params->baudrate;
  Instance->CR1 |= USART_CR1_TE | USART_CR1_RE;
  
  Instance->CR1 |= USART_CR1_UE;
  
  HAL_NVIC_SetPriority(USART1_IRQn, 0x02, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
}
#endif