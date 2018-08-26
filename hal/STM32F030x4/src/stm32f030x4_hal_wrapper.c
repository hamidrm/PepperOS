#include "../inc/stm32f030x4_hal_wrapper.h"
#include "../inc/stm32f030x4_init.h"
#include "pepper_os.h"


static PosStatusType   _to_PosStatusType(HAL_StatusTypeDef status);


void USART1_IRQHandler(void)
{
  uart_sent();
}


__weak void uart_sent(void){
  
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

uint8_t timer_a_get_elapsed_status(void){
  return (TIM3->SR & TIM_SR_UIF)==0?0:1;
}

void timer_a_reset_elapsed_status(void){
  TIM3->SR &= ~TIM_SR_UIF;
}

void timer_a_set_reload_val(uint32_t value){
  TIM3->ARR = value;
}


uint32_t timer_a_get_reload_val(void){
  return TIM3->ARR;
}

void timer_a_enable(void){
  TIM3->CR1 |= TIM_CR1_CEN ;
}

void timer_a_disable(void){
  TIM3->CR1 &= ~TIM_CR1_CEN ;
  TIM3->CNT = 0;
}


void timer_a_enable_evt(void){
  NVIC_enable_irq(TIM3_IRQn);
}

void timer_a_disable_evt(void){
  NVIC_disable_irq(TIM3_IRQn);
}


void NVIC_set_priority(IRQn_Type irq,uint32_t preempt_priority, uint32_t sub_priority){
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

void trig_int(uint32_t i){
  
}

void delay_ms(uint32_t ms){
  HAL_Delay(ms);
}

void led_toggle(void){
  HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_4);
}