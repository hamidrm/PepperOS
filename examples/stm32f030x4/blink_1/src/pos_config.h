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
 *  @file    pos_config.h
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief pepper os configuration file
 *
 */


#ifndef _CONFIG_H_
#define _CONFIG_H_
#include "pos_types.h"

/* System Configuration */
#define SYS_FREQ                48 MHz
#define EXT_CRYSTAL_STATUS      TRUE
#define EXT_CRYSTAL_FREQ        8 MHz

/* POS Configuration */
#define MEM_HEAP_SIZE           2 KB
#define CHECK_STACK_OVER_FLOW   0
#define MEAN_QUANTUM_LENGTH          50 // ms
#define MAX_PROCCESS_NUM           2
#define MAX_IPC_MESSAGES_NUM    5
#define MAX_MUTEX_NUM           10 /* Multiple of 32 */

#define USE_CONSOLE             FALSE
#define CONSOLE_BAUDRATE        115200
#define PRINT_SPLASH            TRUE
#define UART_TX_BUFFER          16
/* Library Configuration */

#endif