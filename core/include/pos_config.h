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

#define USE_CONSOLE             TRUE
#define CONSOLE_BAUDRATE        115200
#define PRINT_SPLASH            TRUE
#define UART_TX_BUFFER          16
/* Library Configuration */

#endif