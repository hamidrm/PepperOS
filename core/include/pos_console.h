#ifndef _POS_CONSOLE_
#define _POS_CONSOLE_

void pos_console_init(void);
PosStatusType print(const char * text,uint8_t len);
void pos_console_print(const char * text,uint8_t len);
void uart_sent(void);
void pos_console_rx_register(_PID pid);

static const char * pos_information = "PepperOS - Copyright (c) 2018 Hamid Reza Mehrabian (version 1.0.0)\n";
#endif