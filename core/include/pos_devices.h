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
 *  @file    pos_devices.h
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief pepper os target devices header builder
 *
 */
#ifndef _DEVICES_H_
#define _DEVICES_H_

#include "pos_utility.h"

/* 
Supported Devices List
*/
//STM32F030x4
#define DEV_STM32F030x4     0




// Define Target Name
#if TARGET_DEVICE == 0
  #define STM32F030x4
  #define DEVICE_NAME   stm32f030x4
  #define CM0
  #define ARCH_NAME_PREFIX      _cm0.h
  #define ARCH_NAME_PATH      cm0
  #define DEV_TOTAL_RAM         4 KB
  #define DEV_RAM_START_ADD     0x20000000
  #define POS_TIMER
  #define POS_GPIO
  #define POS_UART
#else
  #error "POS -> Unknow Target Device!"
#endif

#define _ARCH_PATH     .\\arch
#define _FILES_PREFIX    pos
#define _HEADERS_SUFFIX    h
#define DEVICE_PREFIX_PATH      ..\\hal
#define DEVICE_PREFIX_PATH_2      ..\\..\\hal
#define ARCH_DEVICE_PREFIX_PATH ..\\..\\..\\hal

#define DEVICE_SUFFIX_DEFS      _defs.h
#define DEVICE_SUFFIX_HAL      _hal.h


#define DEVICE_HAL_SUFFIX_RCC      _rcc.h


#define _DEV_DEFS_HEADER   DEV_HEADER(DEVICE_SUFFIX_DEFS)

#define _DEV_HAL_HEADER    DEV_HEADER(DEVICE_SUFFIX_HAL)
#define _DEV_HAL_HEADER_2    DEV_HEADER_2(DEVICE_SUFFIX_HAL)

#define _DEV_INIT_HEADER_ARCH   DEV_HEADER_ARCH(DEVICE_SUFFIX_INIT)
#define _DEV_DEFS_HEADER_ARCH   DEV_HEADER_ARCH(DEVICE_SUFFIX_DEFS)

#define _DEV_HAL_HEADER_ARCH    DEV_HEADER_ARCH(DEVICE_SUFFIX_HAL)


#define _ARCH_HEADER STRINGIFY(BUILD_HEADER_SELECTED_ARCH(DEVICE_ARCH_NAME))
#endif