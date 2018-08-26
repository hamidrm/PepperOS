#include "../inc/stm32f030x4_hal_wrapper.h"
#include "../inc/stm32f030x4_init.h"






PosStatusType clock_config(void){
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

#if EXT_CRYSTAL_STATUS == FALSE
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
#else
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
#endif
  
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLMUL = (((((SYS_FREQ)/(INTERNAL_OSC_FREQ))<<1) - 2) << RCC_CFGR_PLLMUL_POS);
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    pos_error(POS_SYS_INVALID_CLK);
    return POS_ERROR;
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK)
  {
    pos_error(POS_SYS_INVALID_CLK);
    return POS_ERROR;
  }
  return POS_OK;
}


void timer_a_init(void){

  ClockEnable(TIM3);
  
  TIM3->EGR |= TIM_EGR_UG;
  TIM3->PSC = TIM3_PRESCALE;
  
  TIM3->DIER = TIM_DIER_UIE;
  
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
  HAL_NVIC_SetPriority(TIM3_IRQn,0x00,0);
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
  
  HAL_NVIC_SetPriority(USART1_IRQn, 0x00, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
}




void led_init(void){
  GPIO_InitTypeDef GPIO_InitStruct;
  /* Enable GPIOA Clock  */
  ClockEnable(GPIOA);
  
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_MEDIUM;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
}

void stm32f030x4_init(void){
    //TODO-INITIALIZING uC
    HAL_Init();
    clock_config();
}