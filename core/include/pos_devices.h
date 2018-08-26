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

#else
  #error "POS -> Unknow Target Device!"
#endif

#define _ARCH_PATH     .\\arch
#define _FILES_PREFIX    pos
#define _HEADERS_SUFFIX    h
#define DEVICE_PREFIX_PATH      ..\\hal
#define ARCH_DEVICE_PREFIX_PATH ..\\..\\..\\hal
#define DEVICE_SUFFIX_INIT      _init.h
#define DEVICE_SUFFIX_DEFS      _defs.h
#define DEVICE_SUFFIX_HAL      _hal_wrapper.h


#define DEVICE_HAL_SUFFIX_RCC      _rcc.h

#define _DEV_INIT_HEADER   DEV_HEADER(DEVICE_SUFFIX_INIT)
#define _DEV_DEFS_HEADER   DEV_HEADER(DEVICE_SUFFIX_DEFS)

#define _DEV_HAL_HEADER    DEV_HEADER(DEVICE_SUFFIX_HAL)

#define _DEV_INIT_HEADER_ARCH   DEV_HEADER_ARCH(DEVICE_SUFFIX_INIT)
#define _DEV_DEFS_HEADER_ARCH   DEV_HEADER_ARCH(DEVICE_SUFFIX_DEFS)

#define _DEV_HAL_HEADER_ARCH    DEV_HEADER_ARCH(DEVICE_SUFFIX_HAL)


#define _ARCH_HEADER STRINGIFY(BUILD_HEADER_SELECTED_ARCH(DEVICE_ARCH_NAME))
#endif