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
 *  @file    stm32f030x4_gpio.c
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief Managing GPIO.
 *
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
//#include "pos_utility.h"
#include "pos_devices.h"
#include "../inc/stm32f030x4_gpio.h"


#if defined(POS_GPIO)


static uint8_t int_port_index[16];
static const PosPorts ports_name[3] = {PORTA,PORTB,PORTF};


void init_ports(void){
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
}


void set_pin(PosPorts port,PosPinsNumber pin){
  HAL_GPIO_WritePin((GPIO_TypeDef *)port,(uint16_t)pin,GPIO_PIN_SET);
}

void reset_pin(PosPorts port,PosPinsNumber pin){
  HAL_GPIO_WritePin((GPIO_TypeDef *)port,(uint16_t)pin,GPIO_PIN_RESET);
}

void toggle_pin(PosPorts port,PosPinsNumber pin){
  HAL_GPIO_TogglePin((GPIO_TypeDef *)port,(uint16_t)pin);
}

PosPinStatus read_pin(PosPorts port,PosPinsNumber pin){
  return (PosPinStatus)HAL_GPIO_ReadPin((GPIO_TypeDef *)port,(uint16_t)pin);
}

void enable_pin_int(PosPorts port,PosPinsNumber pin,PosExtIntMode mode,PosPullMode pull){
  uint8_t port_idx;
  
  if(mode == EXTINT_FALLING){
    config_pin(port,pin,INTERRUPT_FALLING,pull);
  }else if(mode == EXTINT_RISING){
    config_pin(port,pin,INTERRUPT_RISING,pull);
  }else if(mode == EXTINT_BOTH){
    config_pin(port,pin,INTERRUPT_RISING_FALLING,pull);
  }
  if(pin == PIN1 || pin == PIN0){
    HAL_NVIC_SetPriority(EXTI0_1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
  }else if(pin == PIN3 || pin == PIN2){
    HAL_NVIC_SetPriority(EXTI2_3_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);
  }else{
    HAL_NVIC_SetPriority(EXTI4_15_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
  }
  
  if(port == PORTA)
    port_idx=0;
  if(port == PORTB)
    port_idx=1;
  if(port == PORTF)
    port_idx=3;
  
  for(uint8_t i=0;i<16;i++)
    if(pin == 1<<i){
      int_port_index[i] = port_idx;
      break;
    }
}

void disable_pin_int(PosPorts port,PosPinsNumber pin){
  (void)port;
  if(pin == PIN1 || pin == PIN0){
    HAL_NVIC_DisableIRQ(EXTI0_1_IRQn);
  }else if(pin == PIN3 || pin == PIN2){
    HAL_NVIC_DisableIRQ(EXTI2_3_IRQn);
  }else{
    HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
  }
}

void config_pin(PosPorts port,PosPinsNumber pin,PosPinsMode mode,PosPullMode pull){
  GPIO_InitTypeDef  GPIO_InitStruct;
  GPIO_InitStruct.Pin       = pin;
  GPIO_InitStruct.Mode      = mode;
  GPIO_InitStruct.Pull      = pull;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init((GPIO_TypeDef *)port, &GPIO_InitStruct);
}


void EXTI0_1_IRQHandler(void)
{
  if(EXTI->PR & (1 << 0)) {
    EXTI->PR |= (1 << 0);
    extint_occured(ports_name[int_port_index[0]],PIN0);
  }
  if(EXTI->PR & (1 << 1)) {
    EXTI->PR |= (1 << 1);
    extint_occured(ports_name[int_port_index[1]],PIN1);
  }
}


void EXTI2_3_IRQHandler(void)
{
  if(EXTI->PR & (1 << 2)) {
    EXTI->PR |= (1 << 2);
    extint_occured(ports_name[int_port_index[2]],PIN2);
  }
  if(EXTI->PR & (1 << 3)) {
    EXTI->PR |= (1 << 3);
    extint_occured(ports_name[int_port_index[3]],PIN3);
  }
}


void EXTI4_15_IRQHandler(void)
{
  if(EXTI->PR & (1 << 4)) {
    EXTI->PR |= (1 << 4);
    extint_occured(ports_name[int_port_index[4]],PIN4);
  }
  if(EXTI->PR & (1 << 5)) {
    EXTI->PR |= (1 << 5);
    extint_occured(ports_name[int_port_index[5]],PIN5);
  }
  if(EXTI->PR & (1 << 6)) {
    EXTI->PR |= (1 << 6);
    extint_occured(ports_name[int_port_index[6]],PIN6);
  }
  if(EXTI->PR & (1 << 7)) {
    EXTI->PR |= (1 << 7);
    extint_occured(ports_name[int_port_index[7]],PIN7);
  }
  if(EXTI->PR & (1 << 8)) {
    EXTI->PR |= (1 << 8);
    extint_occured(ports_name[int_port_index[8]],PIN8);
  }
  if(EXTI->PR & (1 << 9)) {
    EXTI->PR |= (1 << 9);
    extint_occured(ports_name[int_port_index[9]],PIN9);
  }
  if(EXTI->PR & (1 << 10)) {
    EXTI->PR |= (1 << 10);
    extint_occured(ports_name[int_port_index[10]],PIN10);
  }
  if(EXTI->PR & (1 << 11)) {
    EXTI->PR |= (1 << 11);
    extint_occured(ports_name[int_port_index[11]],PIN11);
  }
  if(EXTI->PR & (1 << 12)) {
    EXTI->PR |= (1 << 12);
    extint_occured(ports_name[int_port_index[12]],PIN12);
  }
  if(EXTI->PR & (1 << 13)) {
    EXTI->PR |= (1 << 13);
    extint_occured(ports_name[int_port_index[13]],PIN13);
  }
  if(EXTI->PR & (1 << 14)) {
    EXTI->PR |= (1 << 14);
    extint_occured(ports_name[int_port_index[14]],PIN14);
  }
  if(EXTI->PR & (1 << 15)) {
    EXTI->PR |= (1 << 15);
    extint_occured(ports_name[int_port_index[15]],PIN15);
  }
}


__weak void extint_occured(PosPorts port,PosPinsNumber pin){
  
}

#endif