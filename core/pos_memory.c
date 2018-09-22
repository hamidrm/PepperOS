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
 *  @file    pos_memory.c
 *  @author  Hamid Reza Mehrabian
 *  @version 1.0
 *  
 *  @brief pepper os heap management
 *
 */

// Header Files
#include <stdint.h>
#include "pepper_os.h"
#include "string.h"
#include "pos_memory.h"
#include _DEV_HAL_HEADER
#include _ARCH_HEADER


#pragma section="CSTACK"

// Heap Region
static uint32_t * __heap_region = __sfe("CSTACK");

static uint32_t total_heap_used;



/**@brief   Function for clear heap map.
 *
 * @details This function will clear map of the heap region.
 *
 */
PosStatusType init_pos_memory(void){
  uint8_t i=0;
  total_heap_used = 0;
  uint32_t addr = POS_MEMORY_ALLOCATOR_START;
  // Clear Heap Map
  for(i=0;i<POS_MEMORY_HEAP_MAP_SIZE;i++)
    ((volatile uint8_t *)addr)[i] = 0UL;
  
  return POS_OK;
}


/**@brief   Function for create a heap.
 *
 * @details This function will create a heap pointer.
 *
 * @param[in]  size       Heap size
 */
void * pmalloc(size_t size){
	uint32_t	 bit_offset=0,offset=POS_MEMORY_ALLOCATOR_START,block_number=0,block_num_found,block_offset_found,start_filling_offset=0,start_filling_block=0;
	block_type c_data;
	size_t free_space=0,filled_bits=0;
        
	POS_BEGIN_KCRITICAL;
        
	size+=POS_MEMORY_BLOCK_SIZE_LEN;
        

	while(offset<(POS_MEMORY_ALLOCATOR_SIZE+POS_MEMORY_ALLOCATOR_START)){
		c_data = *(block_type *)(offset);
		for(bit_offset=0;bit_offset<POS_MEMORY_BLOCK_SIZE;bit_offset++){
			if(((c_data>>bit_offset) & 1UL)==0){
				if(free_space==0){
					block_num_found=block_number;
					block_offset_found=bit_offset;
				}
				free_space++;
			}else
				free_space=0;
			if(free_space==POS_MEMORY_BLOCK(size)){
				//Requerred space found
				start_filling_offset = block_offset_found;
				start_filling_block = block_num_found;
				filled_bits=0;
				do{
					for(bit_offset=start_filling_offset;filled_bits<POS_MEMORY_BLOCK(size) && bit_offset<POS_MEMORY_BLOCK_SIZE;bit_offset++,filled_bits++)
						*(volatile block_type *)(((start_filling_block*POS_MEMORY_BLOCK_CNT)+POS_MEMORY_ALLOCATOR_START)) |= 1UL<<bit_offset;
					start_filling_offset=0;
					start_filling_block++;
				}while(filled_bits!=POS_MEMORY_BLOCK(size));
				*(unsigned char *)(((block_num_found*POS_MEMORY_BLOCK_SIZE)+block_offset_found)*4+POS_MEMORY_REGION_START+0) = (size -POS_MEMORY_BLOCK_SIZE_LEN)&0xFF;
				*(unsigned char *)(((block_num_found*POS_MEMORY_BLOCK_SIZE)+block_offset_found)*4+POS_MEMORY_REGION_START+1) = ((size-POS_MEMORY_BLOCK_SIZE_LEN)>>8)&0xFF;
				*(unsigned char *)(((block_num_found*POS_MEMORY_BLOCK_SIZE)+block_offset_found)*4+POS_MEMORY_REGION_START+2) = ((size-POS_MEMORY_BLOCK_SIZE_LEN)>>16)&0xFF;
				*(unsigned char *)(((block_num_found*POS_MEMORY_BLOCK_SIZE)+block_offset_found)*4+POS_MEMORY_REGION_START+3) = ((size-POS_MEMORY_BLOCK_SIZE_LEN)>>24)&0xFF;                            
				
                                total_heap_used += size;
                                POS_END_KCRITICAL;
                                return (void *)(((block_num_found*POS_MEMORY_BLOCK_SIZE)+block_offset_found)*POS_MEMORY_BLOCK_CNT+POS_MEMORY_REGION_START+POS_MEMORY_BLOCK_SIZE_LEN);
			}
		}
		
		block_number++;
		offset+=POS_MEMORY_BLOCK_CNT;
	}
        POS_END_KCRITICAL;
        pos_error(POS_MEM_LOW_SPACE);
	return NULL;
}


/**@brief   Function for free heap.
 *
 * @details This function will release a heap pointer.
 *
 * @param[in]  _a          Memory address.
 */
void pfree(void * _a){
  POS_BEGIN_KCRITICAL;
  if(((uint32_t)(_a))>((uint32_t)(__heap_region) + POS_HEAP_SIZE) || ((uint32_t)(_a))<((uint32_t)(__heap_region))){
    pos_error(POS_MEM_INV_ADDR);
    return;
  }
  uint32_t bit_offset,c_alloc = (uint32_t)_a - POS_MEMORY_REGION_START - POS_MEMORY_BLOCK_SIZE_LEN;
  size_t c_size = (*((uint8_t *)(_a)-1))<<24 | 
                    (*((uint8_t *)(_a)-2))<<16 | 
                    (*((uint8_t *)(_a)-3))<<8 |
                    (*((uint8_t *)(_a)-4))+POS_MEMORY_BLOCK_SIZE_LEN;
  total_heap_used -= c_size;
  uint32_t block_num = (c_alloc/POS_MEMORY_BLOCK_CNT)/POS_MEMORY_BLOCK_SIZE;
  uint32_t block_offset = (c_alloc/POS_MEMORY_BLOCK_CNT)%POS_MEMORY_BLOCK_SIZE;
  size_t filled_bits=0;
  do{
          for(bit_offset=block_offset;filled_bits<POS_MEMORY_BLOCK(c_size) && bit_offset<POS_MEMORY_BLOCK_SIZE;bit_offset++,filled_bits++)
                  *(volatile block_type *)(((block_num*POS_MEMORY_BLOCK_CNT)+POS_MEMORY_ALLOCATOR_START)) &= ~(1UL<<bit_offset) & 0xFFFFFFFF;
          block_offset=0;
          block_num++;
  }while(filled_bits!=POS_MEMORY_BLOCK(c_size));
  POS_END_KCRITICAL;
}


/**@brief   Function for get used memory space.
 *
 * @details This function will return amount of allocated heap ( byte ).
 *
 */
size_t get_mem_used(void){
  return total_heap_used;
}

/**@brief   Function for get total memory space.
 *
 * @details This function will return size of memory ( byte ).
 *
 */
size_t get_mem_total(void){
  return POS_HEAP_SIZE - POS_MEMORY_HEAP_MAP_SIZE;
}


/**@brief   Function for get variable size.
 *
 * @details This function will return size of alloced heap.
 *
 * @param[in]  _a          Memory address.
 */
size_t pget_var_size(void * _a){
	return (*((uint8_t *)(_a)-1))<<24 |
               (*((uint8_t *)(_a)-2))<<16 | 
               (*((uint8_t *)(_a)-3))<<8 |
               (*((uint8_t *)(_a)-4))+POS_MEMORY_BLOCK_SIZE_LEN;
}


/**@brief   Function for copy a heap region.
 *
 * @details This function will copy a heap region to special destinition.
 *
 * @param[out]  dst          Destinition address.
 * @param[in]   dstoffset   Offset of destinition address.
 * @param[in]   src          Source address.
 * @param[in]   src_offset   Offset of destinition address.
 * @param[in]   len          Length of data to copy.
 */
PosStatusType pmemcpy(void * dst,uint32_t dst_offset,void * src,uint32_t src_offset,size_t len){
	size_t dst_size,src_size;
	size_t i=0;

        POS_ASSERT( dst != NULL_PTR );
        POS_ASSERT( src != NULL_PTR );
        
        if(pos_is_heap(dst) && pos_is_heap(src)){
          dst_size = pget_var_size(dst);
          src_size = pget_var_size(src);
          
          if(len>(dst_size-dst_offset)){
                  pos_error(POS_MEM_LOW_DST_SIZE);
                  return POS_ERROR;
          }
          if(len>(src_size-src_offset)){
                  pos_error(POS_MEM_LOW_SRC_SIZE);
                  return POS_ERROR;
          }
          while(i<len){
                  *((uint8_t *)(dst)+dst_offset+i) = *((uint8_t *)(src)+src_offset+i);
                  i++;
          }
        }else if(!pos_is_heap(dst) && !pos_is_heap(src)){
          memcpy(dst,src,len);
        }else {
          //TODO
        }
	return POS_OK;
}

/**@brief   Function for set a memory region to special value.
 *
 * @details This function fill a heap region with special value.
 *
 * @param[out]  dst          Destinition address.
 * @param[in]   dst_offset   Offset of destinition address.
 * @param[in]   len          Length of data.
 * @param[in]   value        Value to set.
 */
PosStatusType pmemset(void * dst,uint32_t dst_offset,size_t len,uint8_t value){
	size_t dst_size;
	size_t i=0;
      
        POS_ASSERT( dst != NULL_PTR );
                   
        
        if(pos_is_heap(dst)){
          dst_size = pget_var_size(dst);
          if(len>(dst_size-dst_offset)){
                  pos_error(POS_MEM_LOW_DST_SIZE);
                  return POS_ERROR;
          }
          while(i<len){
                  *((uint8_t *)(dst)+dst_offset+i) = value;
                  i++;
          }
        }else {
          memset(dst,value,len);
        }

	return POS_OK;
}


uint8_t pos_is_heap(void * addr){
  return (((uint32_t)(addr))<((uint32_t)(__heap_region) + POS_HEAP_SIZE) && ((uint32_t)(addr))>((uint32_t)(__heap_region)));
}
/**
@endcode
*/