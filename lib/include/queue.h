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
 *  @brief linked list queue header
 *
 */
#ifndef _QUEUE_H_
#define _QUEUE_H_


PosStatusType pos_queue_enq(pos_queue_t * queue,void * data,size_t len);
PosStatusType pos_queue_deq(pos_queue_t * queue,void ** data,size_t * len);
PosStatusType pos_queue_count(pos_queue_t * queue,size_t * cnt);
PosStatusType pos_create_queue(pos_queue_t ** queue);

#endif