/*
* @brief Memory Management
* @file pos_memory.h
* @author Hamid Reza Mehrabian
*/
#ifndef _POS_MEMORY_H_
#define _POS_MEMORY_H_


#define POS_MEMORY_HEAP_MAP_SIZE        ((POS_HEAP_SIZE/POS_MEMORY_BLOCK_CNT)>>3)
#define POS_MEMORY_HEAP_SIZE    ( POS_MEMORY_HEAP_MAP_SIZE * POS_MEMORY_BLOCK_SIZE )
#define POS_MEMORY_ALLOCATOR_START ((uint32_t)__heap_region)
#define POS_MEMORY_ALLOCATOR_SIZE POS_MEMORY_HEAP_MAP_SIZE
#define POS_MEMORY_REGION_START		 (POS_MEMORY_ALLOCATOR_SIZE+POS_MEMORY_ALLOCATOR_START)



#ifdef POS_MEMORY_BLOCK_SIZE_16
	#define POS_MEMORY_BLOCK_SIZE 16
	#define POS_MEMORY_BLOCK_CNT 2
	typedef uint16_t block_type;
#else
	#define POS_MEMORY_BLOCK_SIZE 32
	#define POS_MEMORY_BLOCK_CNT 4
	typedef uint32_t block_type;
#endif

#define POS_MEMORY_BLOCK(X) (((X)/POS_MEMORY_BLOCK_CNT)+((X%POS_MEMORY_BLOCK_CNT)?1:0))
#define POS_MEMORY_BLOCK_SIZE_LEN	4


void * pmalloc(size_t size);
void pfree(void * _a);
size_t pget_var_size(void * _a);
PosStatusType pmemcpy(void * dst,pos_t dst_offset,void * src,pos_t src_offset,size_t len);
PosStatusType pmemset(void * dst,pos_t dst_offset,size_t len,uint8_t value);
PosStatusType init_pos_memory(void);
size_t get_mem_used(void);
size_t get_mem_total(void);
uint8_t pos_is_heap(void * addr);
#endif

/**
@endcode
*/