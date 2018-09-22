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
 *  @file    stm32f030x4_gpio.h
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief GPIO header.
 *
 */

#ifndef _STM32F030X4_GPIO_H_
#define _STM32F030X4_GPIO_H_
#include "../drivers/std/stm32f0xx_hal.h"

#if defined(POS_GPIO)
typedef enum {
  PORTA = (uint32_t)GPIOA,
  PORTB = (uint32_t)GPIOB,
  PORTF = (uint32_t)GPIOF,
} PosPorts;


typedef enum {
  EXTINT_FALLING,
  EXTINT_RISING,
  EXTINT_BOTH,
} PosExtIntMode;


typedef enum {
  PULL_UP = GPIO_PULLUP,
  PULL_DOWN = GPIO_PULLDOWN,
  NO_PULL = GPIO_NOPULL,
} PosPullMode;


typedef enum {
  PIN0 = GPIO_PIN_0,
  PIN1 = GPIO_PIN_1,
  PIN2 = GPIO_PIN_2,
  PIN3 = GPIO_PIN_3,
  PIN4 = GPIO_PIN_4,
  PIN5 = GPIO_PIN_5,
  PIN6 = GPIO_PIN_6,
  PIN7 = GPIO_PIN_7,
  PIN8 = GPIO_PIN_8,
  PIN9 = GPIO_PIN_9,
  PIN10 = GPIO_PIN_10,
  PIN11 = GPIO_PIN_11,
  PIN12 = GPIO_PIN_12,
  PIN13 = GPIO_PIN_13,
  PIN14 = GPIO_PIN_14,
  PIN15 = GPIO_PIN_15,
} PosPinsNumber;

typedef enum {
  OUTPUT_PUSH_PULL = GPIO_MODE_OUTPUT_PP,
  OUTPUT_OPEN_DRAIN = GPIO_MODE_OUTPUT_OD,
  INPUT = GPIO_MODE_INPUT,
  PERIPHERAL_PUSH_PULL = GPIO_MODE_AF_PP,
  PERIPHERAL_OPEN_DRAIN = GPIO_MODE_AF_OD,
  ANALOG = GPIO_MODE_ANALOG,
  INTERRUPT_RISING = GPIO_MODE_IT_RISING,
  INTERRUPT_FALLING = GPIO_MODE_IT_FALLING,
  INTERRUPT_RISING_FALLING = GPIO_MODE_IT_RISING_FALLING,
} PosPinsMode;

typedef enum{
  PIN_LOW = 0,
  PIN_HIGH = 1,
}PosPinStatus;


void init_ports(void);
void set_pin(PosPorts port,PosPinsNumber pin);
void reset_pin(PosPorts port,PosPinsNumber pin);
void toggle_pin(PosPorts port,PosPinsNumber pin);
PosPinStatus read_pin(PosPorts port,PosPinsNumber pin);
void enable_pin_int(PosPorts port,PosPinsNumber pin,PosExtIntMode mode,PosPullMode pull);
void disable_pin_int(PosPorts port,PosPinsNumber pin);
void config_pin(PosPorts port,PosPinsNumber pin,PosPinsMode mode,PosPullMode pull);
void EXTI0_1_IRQHandler(void);
void EXTI2_3_IRQHandler(void);
void EXTI4_15_IRQHandler(void);
__weak void extint_occured(PosPorts port,PosPinsNumber pin);

#endif

#endif