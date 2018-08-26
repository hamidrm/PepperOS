#ifndef _STM32F030X4_HAL_WRAPPER_
#define _STM32F030X4_HAL_WRAPPER_
#define HAL_MODULE_ENABLED
#include "pos_devices.h"
#include "../drivers/std/stm32f0xx_hal.h"
#include "pepper_os.h"

#define POS_TIMER_A_EVENT       TIM3_IRQHandler
#define _STM32F_USART_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE()
#define _STM32F_TX_PORT_CLK_ENABLE()              __HAL_RCC_GPIOA_CLK_ENABLE()
#define _STM32F_RX_PORT_CLK_ENABLE()              __HAL_RCC_GPIOA_CLK_ENABLE()

/* Definition for USART1 Pins */
#define USART1_TX_PIN                    GPIO_PIN_9
#define USART1_TX_PORT              GPIOA
#define USART1_TX_AF                     GPIO_AF1_USART1
#define USART1_RX_PIN                    GPIO_PIN_10
#define USART1_RX_PORT              GPIOA
#define USART1_RX_AF                     GPIO_AF1_USART1


typedef enum {
  POS_UART_DATA_MODE_1B_START_8B_DATA_1B_STOP,
  POS_UART_DATA_MODE_1B_START_9B_DATA_1B_STOP,
  POS_UART_DATA_MODE_1B_START_8B_DATA_2B_STOP,
  POS_UART_DATA_MODE_1B_START_9B_DATA_2B_STOP
} PosUartDataMode;


typedef enum {
  POS_UART_PARITY_MODE_NONE,
  POS_UART_PARITY_MODE_ODD,
  POS_UART_PARITY_MODE_EVEN,
} PosUartParityMode;

typedef enum {
    POS_UART_INT_FLAG_DATA_READY = 1<<0,
    POS_UART_INT_FLAG_DATA_SENT = 1<<1,
    POS_UART_INT_FLAG_TX_BUFFER_EMPTY = 1<<2,
} PosUartIntFlags;

typedef enum {
    POS_UART1,
} PosUartList;

typedef struct {
  uint32_t      baudrate;
  PosUartDataMode       data_mode;
  PosUartParityMode     parity_mode;
  uint8_t       int_flags;
  void          *       handler;
}pos_hw_uart_t;


inline void trig_int(uint32_t i);
uint32_t sys_tick_get(void);
void sys_tick_isr(void);
uint8_t timer_a_get_elapsed_status(void);
void timer_a_reset_elapsed_status(void);
void timer_a_set_reload_val(uint32_t value);
uint32_t timer_a_get_reload_val(void);
void timer_a_set_reload_val(uint32_t value);
void timer_a_enable(void);
void timer_a_disable(void);
void timer_a_enable_evt(void);
void timer_a_disable_evt(void);
void NVIC_set_priority(IRQn_Type irq,uint32_t preempt_priority, uint32_t sub_priority);
void NVIC_disable_irq(IRQn_Type irq);
void NVIC_enable_irq(IRQn_Type irq);
void NVIC_clear_pending_irq(IRQn_Type irq);
void NVIC_system_reset(void);
void led_toggle(void);
void delay_ms(uint32_t ms);
void uart_write(pos_hw_uart_t * uart_params,uint8_t data);
void uart_clear_int_flag(pos_hw_uart_t * pos_uart,PosUartIntFlags * ints);
void uart_get_int_type(pos_hw_uart_t * pos_uart,PosUartIntFlags * ints);
void uart_read(pos_hw_uart_t * uart_params,uint8_t * data);
__weak void uart_sent(void);
#endif