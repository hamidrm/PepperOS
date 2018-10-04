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
 *  @file    pos_extint.c
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief pepper os external intrrupt controller
 *
 */

#include <stdint.h>
#include "pepper_os.h"

#include _DEV_HAL_HEADER

#if USE_EXT_INT == TRUE


static PosExtIntType extint_list[POS_MAX_EXT_INT_PINS];
static uint8_t ext_int_cnt;

void pos_extint_init(void){
  ext_int_cnt = 0;
}

PosStatusType pos_extint_add(PosPorts port,PosPinsNumber pin,PosExtIntMode mode,PosPullMode pull){
  if(ext_int_cnt == POS_MAX_EXT_INT_PINS){
    pos_error(POS_MEM_LOW_SPACE);
    return POS_ERROR;
  }
  enable_pin_int(port,pin,mode,pull);
  extint_list[ext_int_cnt].pid = _pos_get_current_pid();
  extint_list[ext_int_cnt].port = port;
  extint_list[ext_int_cnt].pin = pin;
  ext_int_cnt++;
  return POS_OK;
}

uint8_t extint_is_active(PosPorts port,PosPinsNumber pin){
  uint8_t i = 0;
  POS_BEGIN_KCRITICAL;
  while(i<ext_int_cnt){
    if(extint_list[i].port == port && extint_list[i].pin == pin){
      POS_END_KCRITICAL;
      return extint_list[i].is_active;
    }
    i++;
  }
  POS_END_KCRITICAL;
  return FALSE;
}

void extint_release(PosPorts port,PosPinsNumber pin){ 
  uint8_t i = 0;
  POS_BEGIN_KCRITICAL;
  while(i<ext_int_cnt){
    if(extint_list[i].is_active==TRUE && extint_list[i].port == port && extint_list[i].pin == pin){
      extint_list[i].is_active = FALSE;
      POS_END_KCRITICAL;
      return;
    }
    i++;
  }
  POS_END_KCRITICAL;
  POS_END_KCRITICAL;
}

void extint_occured(PosPorts port,PosPinsNumber pin){
  uint8_t i = 0;
  POS_BEGIN_KCRITICAL;
  while(i<ext_int_cnt){
    if(extint_list[i].is_active==FALSE && extint_list[i].port == port && extint_list[i].pin == pin){
      extint_list[i].is_active = TRUE;
      pos_send_message(extint_list[i].pid,POS_TASK_EXT_INT,(uint32_t)&extint_list[i]);
      POS_END_KCRITICAL;
      return;
    }
    i++;
  }
  POS_END_KCRITICAL;
}


#endif