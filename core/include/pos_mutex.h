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
 *  @file    pos_mutex.h
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief pepper os mutex management header
 *
 */
#ifndef _POS_MUTEX_H
#define _POS_MUTEX_H

void pos_mutex_init(void);
void pos_mutex_create(pos_mutex_id_t * mutex);
void pos_mutex_free(pos_mutex_id_t * mutex);
void pos_mutex_acquire(pos_mutex_id_t * mutex);
void pos_mutex_release(pos_mutex_id_t * mutex);

#endif
