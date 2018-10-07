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
 *  @file    stack.c
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief linked list stack
 *
 */

// Header Files
#include <stdint.h>
#include "pepper_os.h"
#include "stack.h"

PosStatusType pos_create_stack(pos_stack_t ** stack){
  *stack = (pos_stack_t * )pmalloc(sizeof(pos_stack_t));
  POS_ASSERT(*stack != NULL_PTR);
  (*stack)->sem = (pos_semaphore_t * )pmalloc(sizeof(pos_semaphore_t));
  (*stack)->head = NULL_PTR;
  POS_ASSERT((*stack)->sem != NULL_PTR);
  return pos_semaphore_init((*stack)->sem,1);
}

PosStatusType pos_stack_push(pos_stack_t * stack, void * data,size_t len){
  pos_stack_ll_t * temp;
  pos_stack_ll_t * new_item = (pos_stack_ll_t * )pmalloc(sizeof(pos_stack_ll_t));
  
  POS_ASSERT(stack != NULL_PTR);
  POS_ASSERT(new_item != NULL_PTR);
  
  temp = stack->head;
  new_item->elem_data = data;
  new_item->elem_len = len;
  new_item->next = NULL_PTR;


  while(temp!=NULL_PTR && temp->next!=NULL_PTR)
    temp = temp->next;
  
  if(temp == NULL_PTR)
    stack->head = new_item;
  else
    temp->next = new_item;
  
  return POS_OK;
}


PosStatusType pos_stack_pop(pos_stack_t * stack,void ** data,size_t * len){
  pos_stack_ll_t * temp;
  pos_stack_ll_t * last_item = NULL_PTR;
  
  POS_ASSERT(stack != NULL_PTR);
  POS_ASSERT(stack->head != NULL_PTR);
  
  temp = stack->head;
  
  while(temp!=NULL_PTR && temp->next!=NULL_PTR){
    last_item = temp;
    temp = temp->next;
  }
  
  if(last_item)
    last_item->next = NULL_PTR;
  *data = temp->elem_data;
  *len = temp->elem_len;
  pfree(temp);
  if(last_item == NULL_PTR)
    stack->head = 0;
  return POS_OK;
}

PosStatusType pos_stack_count(pos_stack_t * stack,size_t * cnt){
  pos_stack_ll_t * temp;
  *cnt = 0;
  POS_ASSERT(stack != NULL_PTR);
  POS_ASSERT(stack->head != NULL_PTR);
  POS_ASSERT(cnt != NULL_PTR);
  *cnt = 0;
  temp = stack->head;

  while(temp!=NULL_PTR){
    temp = temp->next;
    (*cnt)++;
  }
  
  return POS_OK;
}