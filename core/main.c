#include "main.h"
#include "nrf24l01.h"
#include "util.h"
#include "timer_manager.h"
#include "ble.h"

static void SystemClock_Config(void);
void Error_Handler(void);

void init_timer(void);


void RF_LL_Init();
void Ttest();
void Ttest2();

int main()
{
  HAL_Init();
  SystemClock_Config();
//  init_timer();
//  
//    /* Enable GPIOA Clock  */
//  ClockEnable(GPIOA);
//  
//  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pin = GPIO_PIN_4;
//  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_MEDIUM;
//  GPIO_InitStruct.Pull      = GPIO_NOPULL;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//  
//  add_timer(100,&id,(void *)Ttest,TIMER_MODE_PERIODICALLY);
//  start_timer(id);
//  
//  add_timer(10000,&id,(void *)Ttest2,TIMER_MODE_ONE_SHOT);
//  start_timer(id);
  
    Ble_Init();
    //RF_LL_Init();
    //Nrf24_Init(NULL);
    while (1)
    {

    }
}

uint8_t led_status = 0,uio=0;
void Ttest(){
  if(led_status){
    GPIOA->BSRR = GPIO_BSRR_BS_4;
    
    led_status = 0;
  }else{
    GPIOA->BRR = GPIO_BSRR_BS_4;
    
    led_status = 1;
  }
  if(uio){
    uio = 0;
  }
}

void Ttest2(){
  uio = 1;
}

void SysTick_Handler(void)
{
  HAL_IncTick();
  
}

static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void){
  while(1);
}