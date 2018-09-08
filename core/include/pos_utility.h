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
 *  @file    pos_utility.h
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief pepper os utility header
 *
 */
#ifndef _UTILITY_H_
#define _UTILITY_H_



#define STRINGIFY(X) STRINGIFY2(X)    
#define STRINGIFY2(X) #X
#define BSL \

#define CAT(X,Y) CAT2(X,Y)
#define CAT2(X,Y) X##Y
#define CAT_2 CAT
#define CAT_3(X,Y,Z) CAT_2(X,CAT_2(Y,Z))
#define CAT_4(A,X,Y,Z) CAT(A,CAT_3(X,Y,Z))
#define ADD_BSLASH(A)  \\A
#define ADD_DOT(A)  .A

#define BUILD_HEADER_PATH(DEV_NAME,FILE_SUFFIX)        CAT_4(DEVICE_PREFIX_PATH,ADD_BSLASH(DEV_NAME),ADD_BSLASH(inc),ADD_BSLASH(CAT_2(DEVICE_NAME,FILE_SUFFIX)))
#define BUILD_HEADER_PATH_ARCH(DEV_NAME,FILE_SUFFIX)        CAT_4(ARCH_DEVICE_PREFIX_PATH,ADD_BSLASH(DEV_NAME),ADD_BSLASH(inc),ADD_BSLASH(CAT_2(DEVICE_NAME,FILE_SUFFIX)))
#define BUILD_HEADER_SELECTED_ARCH(ARCH_NAME)            CAT_4(ADD_DOT(_ARCH_PATH),ADD_BSLASH(ARCH_NAME_PATH),ADD_BSLASH(inc),ADD_BSLASH(CAT_2(_FILES_PREFIX,ARCH_NAME_PREFIX)))

#define DEV_HEADER(LIB_SUFFIX)       STRINGIFY(BUILD_HEADER_PATH(DEVICE_NAME,LIB_SUFFIX))
#define DEV_HEADER_ARCH(LIB_SUFFIX)       STRINGIFY(BUILD_HEADER_PATH_ARCH(DEVICE_NAME,LIB_SUFFIX))



#define DEV_INIT        CAT_2(DEVICE_NAME,_init)
#define DEV_BL_INIT        CAT_2(DEVICE_NAME,_bootloader_init)
#define DEV_JMP_TO_POS        CAT_2(DEVICE_NAME,_jump_to_pos)
#endif