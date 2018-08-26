#ifndef _STM32F030X4_H_
#define _STM32F030X4_H_

#include "pepper_os.h"
#include "../drivers/std/stm32f0xx_hal.h"

#define ClockEnable(x) __HAL_RCC_##x##_CLK_ENABLE()

#define TIM3_PRESCALE (((SYS_FREQ)/1000) - 1)
#define  TIMER_MAX_INTERVAL (1<<16 - 1)

#define RCC_CFGR_PLLMUL_POS     18

#define INTERNAL_OSC_FREQ        8 MHz




/**
 * Bootloader Init
 */
static inline void stm32f030x4_bootloader_init(void){
  uint32_t      tmp;
  // Config RCC
  RCC->CR &= ~RCC_CR_PLLON; // Turn off PLL
  while(RCC->CR & RCC_CR_PLLRDY); // Wait for unlocking PLL
  tmp = RCC->CFGR;
  tmp &= ~RCC_CFGR_PLLMUL;
  tmp |= (((SYS_FREQ)/(8 MHz) - 2) << 18);
  tmp &= ~RCC_CFGR_PLLSRC;
  tmp |= RCC_CFGR_PLLSRC_HSI_DIV2;
  tmp &= ~RCC_CFGR_PPRE;
  tmp |= RCC_CFGR_PPRE_DIV1;
  tmp &= ~RCC_CFGR_HPRE;
  tmp |= RCC_CFGR_HPRE_DIV1;
  tmp &= ~RCC_CFGR_SW;
  tmp |= RCC_CFGR_SW_HSI; 
  RCC->CFGR = tmp;
  RCC->CR |= RCC_CR_HSION;
  while((RCC->CR & RCC_CR_HSIRDY) == 0); // Wait for stable oscillator 
  RCC->CR |= RCC_CR_PLLON; // Turn on PLL
  while((RCC->CR & RCC_CR_PLLRDY) == 0); // Wait for locking PLL  
}


/**
 * Jump to PepperOS
 */
static inline void stm32f030x4_jump_to_pos(void)
{
    // disable global interrupt
    __disable_irq();
 
    // Disable all peripheral interrupts
    NVIC_DisableIRQ(SysTick_IRQn);
    NVIC_DisableIRQ(USART2_IRQn);
 
    NVIC_DisableIRQ(WWDG_IRQn);
    NVIC_DisableIRQ(RTC_IRQn);
    NVIC_DisableIRQ(FLASH_IRQn);
    NVIC_DisableIRQ(RCC_IRQn);
    NVIC_DisableIRQ(EXTI0_1_IRQn);
    NVIC_DisableIRQ(EXTI2_3_IRQn);
    NVIC_DisableIRQ(EXTI4_15_IRQn);
    NVIC_DisableIRQ(DMA1_Channel1_IRQn);
    NVIC_DisableIRQ(DMA1_Channel2_3_IRQn);
    NVIC_DisableIRQ(DMA1_Channel4_5_IRQn);
    NVIC_DisableIRQ(ADC1_IRQn);
    NVIC_DisableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
    NVIC_DisableIRQ(TIM1_CC_IRQn);
    NVIC_DisableIRQ(TIM3_IRQn);
    NVIC_DisableIRQ(TIM6_IRQn);
    NVIC_DisableIRQ(TIM14_IRQn);
    NVIC_DisableIRQ(TIM15_IRQn);
    NVIC_DisableIRQ(TIM16_IRQn);
    NVIC_DisableIRQ(TIM17_IRQn);
    NVIC_DisableIRQ(I2C1_IRQn);
    NVIC_DisableIRQ(I2C2_IRQn);
    NVIC_DisableIRQ(SPI1_IRQn);
    NVIC_DisableIRQ(SPI2_IRQn);
    NVIC_DisableIRQ(USART1_IRQn);
    NVIC_DisableIRQ(USART2_IRQn);
 
    // main app start address defined in linker file
    extern const unsigned int _region_APP_start__;   
 
    uint32_t MemoryAddr = (unsigned int) (&_region_APP_start__);
    uint32_t *pMem = (uint32_t *)MemoryAddr;
 
    // First address is the stack pointer initial value
    __set_MSP(*pMem); // Set stack pointer
 
    // Now get main app entry point address
    pMem++;
    void (*pMainApp)(void) = (void (*)(void))(*pMem);
 
    // Jump to main application (0x0800 0004)
    pMainApp();
}

void stm32f030x4_init(void);
void timer_a_init(void);
void led_init(void);
PosStatusType clock_config(void);
void uart_init(pos_hw_uart_t * uart_params,PosUartList uart);
void USART1_IRQHandler(void);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle);
#endif