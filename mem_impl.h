/*
Name: Kenneth Chen, David Luo
CSE 374

mem_impl.h -> Header file containing the structure definition for memory blocks, declarations 
  for global variables, and declarations for utility functions.
*/

#ifndef MEM_IMPL_H_
#define MEM_IMPL_H_

#include "mem.h"
#include <stdint.h>

// Definition of the block_header structure
typedef struct block_header {
  uintptr_t size;  // Size of the block
  struct block_header *next;  // Pointer to the next block in the free list
} block_header;

// Declaration of global variables
extern uintptr_t total_size;
extern uintptr_t total_free;
extern uintptr_t n_free_blocks;
extern block_header *free_list;

// Declaration of functions
void check_heap();
void freemem(void *p);
void *getmem(uintptr_t size);

#endif // MEM_IMPL_H_
