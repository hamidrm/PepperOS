#ifndef UTIL_H

void sleep(uint32_t time_us);
void __rbit(uint8_t * data,uint8_t len);


#define ClockEnable(x) __HAL_RCC_##x##_CLK_ENABLE()

#define Max(a,b)        (a>b)?a:b
#define Min(a,b)        (a<b)?a:b


#define __disable_interrupt()     __asm(" CPSID i")
#define __enable_interrupt()     __asm(" CPSIE i")


typedef enum {
  STATUS_SUCCESS,
  STATUS_INVALID_PARAMS,
  STATUS_INVALID_LEN,
  STATUS_INVALID_STATE,
  STATUS_MEMORY_FULL,
  STATUS_CRC_ERROR,
}RES_STATUS;



#endif