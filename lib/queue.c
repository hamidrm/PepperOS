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
 *  @file    queue.c
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief linked list queue
 *
 */

// Header Files
#include <stdint.h>
#include "pepper_os.h"
#include "queue.h"

PosStatusType pos_create_queue(pos_queue_t ** queue){
  *queue = (pos_queue_t * )pmalloc(sizeof(pos_queue_t));
  POS_ASSERT(*queue != NULL_PTR);
  (*queue)->sem = (pos_semaphore_t * )pmalloc(sizeof(pos_semaphore_t));
  (*queue)->head = NULL_PTR;
  POS_ASSERT((*queue)->sem != NULL_PTR);
  return pos_semaphore_init((*queue)->sem,1);
}

PosStatusType pos_queue_enq(pos_queue_t * queue, void * data,size_t len){
  pos_queue_ll_t * temp;
  pos_queue_ll_t * new_item = (pos_queue_ll_t * )pmalloc(sizeof(pos_queue_ll_t));
  
  POS_ASSERT(queue != NULL_PTR);
  POS_ASSERT(new_item != NULL_PTR);
  
  temp = queue->head;
  new_item->elem_data = data;
  new_item->elem_len = len;
  new_item->next = NULL_PTR;


  while(temp!=NULL_PTR && temp->next!=NULL_PTR)
    temp = temp->next;
  
  if(temp == NULL_PTR)
    queue->head = new_item;
  else
    temp->next = new_item;
  
  return POS_OK;
}


PosStatusType pos_queue_deq(pos_queue_t * queue,void ** data,size_t * len){
  pos_queue_ll_t * temp;
  pos_queue_ll_t * last_item = NULL_PTR;
  
  POS_ASSERT(queue != NULL_PTR);
  POS_ASSERT(queue->head != NULL_PTR);
  
  temp = queue->head;
  
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
    queue->head = 0;
  return POS_OK;
}

PosStatusType pos_queue_count(pos_queue_t * queue,size_t * cnt){
  pos_queue_ll_t * temp;
  *cnt = 0;
  POS_ASSERT(queue != NULL_PTR);
  POS_ASSERT(queue->head != NULL_PTR);
  POS_ASSERT(cnt != NULL_PTR);
  *cnt = 0;
  temp = queue->head;

  while(temp!=NULL_PTR){
    temp = temp->next;
    (*cnt)++;
  }
  
  return POS_OK;
}